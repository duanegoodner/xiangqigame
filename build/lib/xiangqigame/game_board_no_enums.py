import math
import numpy as np
from nptyping import NDArray, Int, Shape
from typing import NamedTuple, Tuple, Union, TypedDict


class PType(NamedTuple):
    NUL: int = 0
    GEN: int = 1
    ADV: int = 2
    ELE: int = 3
    HOR: int = 4
    CHA: int = 5
    CAN: int = 6
    SOL: int = 7


PTYPE = PType()


class PColor(NamedTuple):
    RED: int = -1
    NUL: int = 0
    BLK: int = 1


PCOLOR = PColor()


class BoardSpace(NamedTuple):
    rank: int
    file: int


class GamePiece(TypedDict):
    type: int
    color: int


class StartingBoardBuilder:
    _num_ranks = 10
    _num_files = 9

    _back_row = (
        PTYPE.CHA, PTYPE.HOR, PTYPE.ELE, PTYPE.ADV, PTYPE.GEN,
        PTYPE.ADV, PTYPE.ELE, PTYPE.HOR, PTYPE.CHA)
    _no_pieces_row = tuple([PTYPE.NUL] * 9)
    _cannon_row = (
        PTYPE.NUL, PTYPE.CAN, PTYPE.NUL, PTYPE.NUL, PTYPE.NUL,
        PTYPE.NUL, PTYPE.NUL, PTYPE.CAN, PTYPE.NUL)
    _soldier_row = (
        PTYPE.SOL, PTYPE.NUL, PTYPE.SOL, PTYPE.NUL, PTYPE.SOL,
        PTYPE.NUL, PTYPE.SOL, PTYPE.NUL, PTYPE.SOL)

    @staticmethod
    def _build_row_of_int_pieces(
            pieces: Tuple[int, int, int, int, int, int, int, int, int],
            color: int):
        piece_list = [color * piece for piece in pieces]
        return np.array(piece_list)

    @property
    def _black_back_row(self):
        return self._build_row_of_int_pieces(
            pieces=self._back_row, color=PCOLOR.BLK)

    @property
    def _black_cannon_row(self):
        return self._build_row_of_int_pieces(
            pieces=self._cannon_row, color=PCOLOR.BLK)

    @property
    def _black_soldier_row(self):
        return self._build_row_of_int_pieces(
            pieces=self._soldier_row, color=PCOLOR.BLK)

    @property
    def _empty_row(self):
        return self._build_row_of_int_pieces(
            pieces=self._no_pieces_row, color=PCOLOR.NUL)

    @property
    def _black_board_half(self):
        return np.vstack((
            self._black_back_row,
            self._empty_row,
            self._black_cannon_row,
            self._black_soldier_row,
            self._empty_row))

    @property
    def _red_board_half(self):
        return -1 * np.flip(self._black_board_half, axis=0)

    def build(self):
        return np.vstack((self._black_board_half, self._red_board_half))


class CastleLocation(NamedTuple):
    slices: Tuple[slice, slice]
    offset: NDArray[Shape["1, 2"], Int]


class BoardUtilities:

    @staticmethod
    def get_piece_color(piece: int):
        return 0 if piece == 0 else int(math.copysign(1, piece))

    def get_forward_direction(self, piece: int):
        return self.get_forward_direction(piece)

    @staticmethod
    def _is_black(piece: int):
        return piece > 0

    @staticmethod
    def _is_red(piece: int):
        return piece < 0

    @staticmethod
    def _is_null(piece: int):
        return piece == 0

    is_color = {
        PCOLOR.BLK: _is_black,
        PCOLOR.RED: _is_red,
        PCOLOR.NUL: _is_null
    }

    castle_loc = {
        PCOLOR.BLK: CastleLocation(
            slices=(slice(0, 3), slice(3, 6)),
            offset=np.array([[0, 3]])),
        PCOLOR.RED: CastleLocation(
            slices=(slice(7, 10), slice(3, 6)),
            offset=np.array([[7, 3]]))
    }

    river_edge = {
        PCOLOR.BLK: 4,
        PCOLOR.RED: 5
    }


class GameBoard:
    _num_ranks = 10
    _num_files = 9

    def __init__(
            self,
            int_map: Union[NDArray[Shape["10, 9"], Int] | None] = None):
        if int_map is None:
            int_map = StartingBoardBuilder().build()
        self._map = int_map

    def is_occupied(self, space: NDArray[Shape["2"], Int]):
        return self._map[space[0], space[1]] != 0

    def get_piece_type(self, space: NDArray[Shape["2"], Int]):
        return abs(self._map[space[0], space[1]])

    def get_piece_color(self, space: NDArray[Shape["2"], Int]):
        piece_val = self._map[space[0], space[1]]
        return 0 if piece_val == 0 else int(math.copysign(1, piece_val))

    def get_all_spaces_occupied_by(self, color: int):
        return np.argwhere(BoardUtilities.is_color[color](self._map))

    def get_general_position(self, color: int):
        castle = BoardUtilities.castle_loc[color]
        return np.argwhere(self._map[castle.slices] == color) + castle.offset

    @staticmethod
    def is_in_homeland_of(color: int, space: NDArray[Shape["2"], Int]):
        if color == PCOLOR.RED:
            return space[0] >= BoardUtilities.river_edge[PCOLOR.RED]
        if color == PCOLOR.BLK:
            return space[0] <= BoardUtilities.river_edge[PCOLOR.BLK]




