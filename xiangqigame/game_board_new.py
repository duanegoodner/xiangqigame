from typing import NamedTuple, Tuple
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
    start: Tuple[int, int]
    end: Tuple[int, int]


class ExecutedMove(NamedTuple):
    move: Move
    moving_piece: int
    destination_piece: int


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
        return np.array(
            [0, PieceType.CAN.value, 0, 0, 0, 0, 0, PieceType.CAN.value, 0])

    @property
    def soldier_row(self):
        return np.array([PieceType.SOL.value, 0, PieceType.SOL.value, 0,
                         PieceType.SOL.value, 0,
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
    """
    Gameboard._map: 10 x 9 array of ints
        rows -> ranks, cols -> file, value -> int corresponding to a piece

    space: [2, ] -> [rank, file] (int)
    move: [4, ] -> [rank_start, file_start, rank_end, file_end] (int)
    executed_move: [6, ] ->
        [rank_start, file_start, rank_end, file_end, moving_piece, dest_piece]


    """

    _river_edge_black = 4
    _river_edge_red = 5
    _num_ranks = 10
    _num_files = 9
    _horizontal_vects = np.array([[0, 1], [0, -1]])

    def __init__(self, board_map: np.array = None):
        if board_map is None:
            board_map = StartingBoardBuilder().build_initial_board()
        self._map = board_map

    def is_occupied(self, space: Tuple[int, int]):
        return self._map[space] != 0

    def get_piece_type(self, space: Tuple[int, int]):
        return PieceType(np.abs(self._map[space]))

    def get_piece_color(self, space: Tuple[int, int]):
        return PieceColor(
            np.sign(self._map[space]))

    @staticmethod
    def forward_direction(color: PieceColor):
        return np.array([color.value, 0])

    def get_slice(
            self,
            from_space: Tuple[int, int],
            direction: Tuple[int, int]):

        from_array = np.array(from_space)
        direction_array = np.array(direction)

        slice_axis = np.argwhere(direction_array != 0).item()
        slice_sign = np.sign(direction_array[slice_axis])
        slice_from = from_array[slice_axis]

        if slice_sign > 0:
            slice_obj = slice(slice_from + 1, self._map.shape[slice_axis])
        else:
            slice_obj = slice(0, slice_from)

        if slice_axis == 0:
            return self._map[slice_obj, from_space[1]]
        else:
            return self._map[from_space[0], slice_obj]

    def execute_move(
            self,
            start: Tuple[int, int],
            end: Tuple[int, int],
    ) -> Tuple[Tuple[int, int], Tuple[int, int], int, int]:
        moving_piece = self._map[start]
        destination_piece = self._map[end]
        self._map[end] = moving_piece
        self._map[start] = PieceType.NUL.value

        executed_move = (start, end, moving_piece, destination_piece)

        return executed_move

    def undo_move(
            self,
            executed_move: Tuple[Tuple[int, int], Tuple[int, int], int, int]):

        self._map[executed_move[0]] = executed_move[2]
        self._map[executed_move[1]] = executed_move[3]

    def get_all_spaces_occupied_by(
            self, color: PieceColor) -> NDArray[Shape["*, 2"], Int]:
        return np.argwhere(np.sign(self._map) == color.value)

    def get_general_position(self, color: PieceColor):
        return np.argwhere(self._map == color.value * PieceType.GEN.value)

    def is_in_homeland_of(
            self, color: PieceColor, space: NDArray[Shape["2"], Int]) -> bool:
        if color == PieceColor.RED:
            return space[0] >= self._river_edge_red
        if color == PieceColor.BLK:
            return space[0] <= self._river_edge_black

    def search_spaces(
            self,
            from_space: NDArray[Shape["2"], Int],
            direction: NDArray[Shape["2"], Int]):
        search_axis = np.argwhere(direction != 0).item()
        non_search_axis = int(not search_axis)
        search_sign = np.sign(direction[search_axis])

    def remove_off_board_spaces(
            self,
            spaces: NDArray[Shape["2, *"], Int]):
        return spaces[
            (spaces[:, 0] >= 0) &
            (spaces[:, 1] >= 0) &
            (spaces[:, 0] < self._num_ranks) &
            (spaces[:, 1] < self._num_files)
            ]

    def soldier_moves(
            self,
            from_position: NDArray[Shape["2"], Int],
            color: PieceColor):
        if self.is_in_homeland_of(color, from_position):
            prelim_dest = np.expand_dims(from_position + self.forward_direction(color),  axis=0)
            destinations = self.remove_off_board_spaces(prelim_dest)
        else:
            prelim_destinations = from_position + np.row_stack(
                (self.forward_direction(color), self._horizontal_vects))
            destinations = self.remove_off_board_spaces(
                prelim_destinations)

        return np.hstack(
            (np.broadcast_to(from_position, (destinations.shape[0], 2)),
             destinations))


