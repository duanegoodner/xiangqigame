from typing import NamedTuple
from nptyping import NDArray, Int, Shape

import numpy as np
from enum import Enum


class PieceType(Enum):
    NUL = 0
    GEN = 1
    ADV = 2
    ELE = 3
    HOR = 4
    CHA = 5
    CAN = 6
    SOL = 7


class PieceColor(Enum):
    NUL = 0
    BLK = 1
    RED = -1


class BoardSpace(NamedTuple):
    rank: int
    file: int


class GamePiece(NamedTuple):
    color: PieceColor
    piece_type: PieceType


class Move(NamedTuple):
    start: BoardSpace
    end: BoardSpace


class ExecutedMove(NamedTuple):
    spaces: NDArray[Shape["2, 2"], Int]
    moving_piece: GamePiece
    destination_piece: GamePiece


class StartingBoardBuilder:

    @property
    def back_row(self):
        return np.array(
            [piece.value for piece in
             [PieceType.CHA, PieceType.HOR, PieceType.ELE, PieceType.ADV,
              PieceType.GEN,
              PieceType.ADV, PieceType.ELE, PieceType.HOR, PieceType.CHA]])

    @property
    def cannon_row(self):
        return np.array([0, PieceType.CAN.value, 0, 0, 0, 0, 0, PieceType.CAN.value, 0])

    @property
    def soldier_row(self):
        return np.array([PieceType.SOL.value, 0, PieceType.SOL.value, 0, PieceType.SOL.value, 0,
                         PieceType.SOL.value, 0, PieceType.SOL.value])

    @property
    def starting_block(self):
        return np.vstack((
            self.back_row,
            np.zeros(9, dtype=int),
            self.cannon_row,
            self.soldier_row,
            np.zeros(9, dtype=int)))

    def build_initial_board(self):

        black_half = PieceColor.BLK.value * self.starting_block
        red_half = PieceColor.RED.value * np.flip(self.starting_block, axis=0)
        return np.vstack((black_half, red_half))


class GameBoard:

    _river_edge_black = 4
    _river_edge_red = 5
    _num_ranks = 10
    _num_files = 9
    _adjacent_vects = np.array([[1, 0], [0, 1], [-1, 0], [0, -1]])

    def __init__(self, board_map: np.array = None):
        if board_map is None:
            board_map = StartingBoardBuilder().build_initial_board()
        self._map = board_map

    def is_occupied(self, space: NDArray[Shape["2, "], Int]):
        return self._map[space[0], space[1]] != 0

    def get_piece_type(self, space: NDArray[Shape["2, "], Int]):
        return PieceType(self._map[space[0], space[1]])

    def get_piece_color(self, space: NDArray[Shape["2, "], Int]):
        return PieceColor(
            np.sign(self._map[space[0], space[1]]))

    # TODO consider using lower level array manip instead of set_occupant
    def set_occupant(self, space: NDArray[Shape["2, "], Int], piece: GamePiece):
        self._map[space[0], space[1]] = (
                piece.color.value * piece.piece_type.value
        )

    def execute_move(self, move: NDArray[Shape["2, 2"], Int]) -> ExecutedMove:
        moving_piece = self._map[move[0, 0], move[0, 1]]
        destination_piece = self._map[move[1, 0], move[1, 1]]

        self.set_occupant(space=move[1, :], piece=moving_piece)
        self.set_occupant(
            space=move[0, :],
            piece=GamePiece(piece_type=PieceType.NUL, color=PieceColor.NUL))

        return ExecutedMove(spaces=move, moving_piece=moving_piece,
                            destination_piece=destination_piece)

    def undo_move(self, move: ExecutedMove):
        self.set_occupant(move.spaces[0, :], move.moving_piece)
        self.set_occupant(move.spaces[1, :], move.destination_piece)

    def get_all_spaces_occupied_by(
            self, color: PieceColor) -> NDArray[Shape["*, 2"]]:
        return np.argwhere(np.sign(self._map) == color.value)

    def get_general_position(self, color: PieceColor):
        return np.argwhere(self._map == color.value * PieceType.GEN.value)

    def is_in_homeland_of(
            self, color: PieceColor, space: NDArray[Shape["2, "]]) -> bool:
        if color == PieceColor.RED:
            return space[1] >= self._river_edge_red
        if color == PieceColor.BLK:
            return space[1] <= self._river_edge_black

    def remove_off_board_spaces(self, spaces: NDArray[Shape["2, *"]]):
        return spaces[
            (spaces[:, 0] >= 0) &
            (spaces[:, 1] >= 0) &
            (spaces[:, 0] < self._num_ranks) &
            (spaces[:, 1] < self._num_files)
        ]

    def soldier_moves(
            self,
            from_position: NDArray[Shape["2, "]],
            color: PieceColor):
        if self.is_in_homeland_of(color, from_position):
            destinations = self.remove_off_board_spaces(
                from_position + np.array([color.value, 0]))
        else:
            destinations = self.remove_off_board_spaces(
                from_position + self._adjacent_vects)











starting_board = StartingBoardBuilder().build_initial_board()
print(starting_board)
print(np.sign(starting_board))

