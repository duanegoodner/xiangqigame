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

    def _build_row_of_int_pieces(
            self,
            pieces: Tuple[int, int, int, int, int, int, int, int, int],
            color: int):
        piece_list = [color * piece for piece in pieces]
        return np.array(piece_list)

    def _build_row_of_td_pieces(
            self,
            pieces: Tuple[int, int, int, int, int, int, int, int, int],
            color: int):
        row = [GamePiece(type=PTYPE.NUL, color=PCOLOR.NUL)] * self._num_files
        for file in range(self._num_files):
            if pieces[file] != PTYPE.NUL:
                row[file] = GamePiece(type=pieces[file], color=color)
        return np.array(row)

    def _build_row_of_custom_type(
            self,
            pieces: Tuple[int, int, int, int, int, int, int, int, int],
            color: int):
        row = []
        for file in range(self._num_files):
            if pieces[file] == PTYPE.NUL:
                row.append((pieces[file], PCOLOR.NUL))
            else:
                row.append((pieces[file], color))
        return np.array(row, dtype=[("type", int), ("color", int)])

    @property
    def _black_back_row(self):
        return self._build_row_of_int_pieces(
            pieces=self._back_row, color=PCOLOR.BLK)

    @property
    def _black_back_row_td(self):
        return self._build_row_of_td_pieces(
            pieces=self._back_row, color=PCOLOR.BLK)

    @property
    def _black_back_row_ct(self):
        return self._build_row_of_custom_type(
            pieces=self._back_row, color=PCOLOR.BLK)

    @property
    def _black_cannon_row(self):
        return self._build_row_of_int_pieces(
            pieces=self._cannon_row, color=PCOLOR.BLK)

    @property
    def _black_cannon_row_td(self):
        return self._build_row_of_td_pieces(
            pieces=self._cannon_row, color=PCOLOR.BLK)

    @property
    def _black_cannon_row_ct(self):
        return self._build_row_of_custom_type(
            pieces=self._cannon_row, color=PCOLOR.BLK)

    @property
    def _black_soldier_row(self):
        return self._build_row_of_int_pieces(
            pieces=self._soldier_row, color=PCOLOR.BLK)

    @property
    def _black_soldier_row_td(self):
        return self._build_row_of_td_pieces(
            pieces=self._soldier_row, color=PCOLOR.BLK)

    @property
    def _black_soldier_row_ct(self):
        return self._build_row_of_custom_type(
            pieces=self._soldier_row, color=PCOLOR.BLK)

    @property
    def _empty_row(self):
        return self._build_row_of_int_pieces(
            pieces=self._no_pieces_row, color=PCOLOR.NUL)

    @property
    def _empty_row_td(self):
        return self._build_row_of_td_pieces(
            pieces=self._no_pieces_row, color=PCOLOR.NUL)

    @property
    def _empty_row_ct(self):
        return self._build_row_of_custom_type(
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
    def _black_board_half_td(self):
        return np.vstack((
            self._black_back_row_td,
            self._empty_row_td,
            self._black_cannon_row_td,
            self._black_soldier_row_td,
            self._empty_row_td
        ))

    @property
    def _black_board_half_ct(self):
        return np.vstack((
            self._black_back_row_ct,
            self._empty_row_ct,
            self._black_cannon_row_ct,
            self._black_soldier_row_ct,
            self._empty_row_ct
        ))

    @property
    def _red_board_half(self):
        return -1 * np.flip(self._black_board_half, axis=0)

    @property
    def _red_board_half_td(self):
        red_half = np.flip(self._black_board_half_td, axis=0)
        for row in range(red_half.shape[0]):
            for col in range(red_half.shape[1]):
                if red_half[row, col]["color"] == PCOLOR.BLK:
                    red_half[row, col]["color"] = PCOLOR.RED
        return red_half

    @property
    def _red_board_half_ct(self):
        red_half = np.flip(self._black_board_half_ct, axis=0)
        for row in range(red_half.shape[0]):
            for col in range(red_half.shape[1]):
                if red_half[row, col]["color"] == PCOLOR.BLK:
                    red_half[row, col]["color"] = PCOLOR.RED
        return red_half

    def build(self):
        return np.vstack((self._black_board_half, self._red_board_half))

    def build_td(self):
        return np.vstack((self._black_board_half_td, self._red_board_half_td))

    def build_ct(self):
        return np.vstack((self._black_board_half_ct, self._red_board_half_ct))


class GameBoard:
    _index_array = np.array(
        [[BoardSpace(rank=row, file=col) for col in range(9)] for row in
         range(10)],
        dtype=[("rank", int), ("file", int)])

    def __init__(
            self,
            int_map: Union[NDArray[Shape["10, 9"], Int] | None] = None,
            td_map=None,
            ct_map=None

    ):
        if int_map is None:
            int_map = StartingBoardBuilder().build()
        if td_map is None:
            td_map = StartingBoardBuilder().build_td()
        if ct_map is None:
            ct_map = StartingBoardBuilder().build_ct()

        self._map_int = int_map
        self._map_td = td_map
        self._map_ct = ct_map

    def is_occupied_int(self, space: BoardSpace):
        return self._map_int[space] != 0

    def is_occupied_td(self, space: BoardSpace):
        return self._map_td[space]["type"] != PTYPE.NUL

    def is_occupied_ct(self, space: BoardSpace):
        return self._map_ct[space]["type"] != PTYPE.NUL

    def get_piece_type_int(self, space: BoardSpace):
        return abs(self._map_int[space])

    def get_piece_type_td(self, space: BoardSpace):
        return self._map_td[space]["type"]

    def get_piece_color_int(self, space: BoardSpace):
        # piece_val = self._map_int[space]
        # return PCOLOR.RED if piece_val < 0 else int(bool(piece_val))

        piece_val = self._map_int[space]
        color = 0 if piece_val == 0 else int(math.copysign(1, piece_val))
        return color

    @staticmethod
    def get_color(value: int):
        color = 0 if value == 0 else int(math.copysign(1, value))
        return color

    def get_piece_color_td(self, space: tuple):
        return self._map_td[space]["color"]

    def get_all_spaces_occupied_by_int(self, color: int):
        return np.argwhere(np.sign(self._map_int) == color)

    def get_all_spaces_occupied_by_td(self, color: int):
        color_array = np.array(
            [[self._map_td[row, col]["color"] for col in range(9)]
             for row in range(10)])
        return np.argwhere(color_array == color)

    def get_all_spaces_occupied_by_ct(self, color: int):
        return np.argwhere(self._map_ct["color"] == color)

