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


_piece_type = {
    0: PieceType.NUL,
    1: PieceType.GEN,
    2: PieceType.ADV,
    3: PieceType.ELE,
    4: PieceType.HOR,
    5: PieceType.CHA,
    6: PieceType.CAN,
    7: PieceType.SOL
}


class PieceColor(Enum):
    NUL = 0
    BLK = 1
    RED = -1


_castle_coords = {
    PieceColor.RED: [(rank, file) for rank in range(7, 10) for file in range(3, 6)],
    PieceColor.BLK: [(rank, file) for rank in range(0, 3) for file in range(3, 6)]

}


class BoardSpace(NamedTuple):
    rank: int
    file: int


class GamePiece(NamedTuple):
    color: PieceColor
    piece_type: PieceType


# class Move(NamedTuple):
#     start: Tuple[int, int]
#     end: Tuple[int, int]


# class ExecutedMove(NamedTuple):
#     move: Move
#     moving_piece: int
#     destination_piece: int


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
    _space_dt = np.dtype([("rank", int), ("file", int)])

    def __init__(self, board_map: np.array = None):
        if board_map is None:
            board_map = StartingBoardBuilder().build_initial_board()
        self._map = board_map

    @property
    def castles(self):
        return {
            PieceColor.BLK: self._map[:3, 3:6],
            PieceColor.RED: self._map[7:, 3:6]
        }

    def is_occupied(self, space: BoardSpace):
        return self._map[space] != 0

    def get_piece_type(self, space: BoardSpace):
        return _piece_type[abs(self._map[space])]

    def get_piece_color(self, space: NDArray[Shape["2"], Int]):
        return PieceColor(
            np.sign(self._map[space[0], space[1]]))

    @staticmethod
    def forward_direction(color: PieceColor):
        return np.array([color.value, 0])

    def get_slice(
            self,
            from_space: NDArray[Shape["2"], Int],
            direction: NDArray[Shape["2"], Int]):

        slice_axis = np.argwhere(direction != 0).item()
        slice_sign = np.sign(direction[slice_axis])
        slice_from = from_space[slice_axis]

        if slice_sign > 0:
            slice_axis_coords = np.arange(
                slice_from + 1, self._map.shape[slice_axis])
        else:
            slice_axis_coords = np.arange(slice_from - 1, -1, -1)

        if slice_axis == 0:
            space_coords = np.column_stack(
                (slice_axis_coords,
                 np.broadcast_to(from_space[1], slice_axis_coords.size)))
            pieces = np.take(self._map[:, from_space[1]], slice_axis_coords)
        else:
            space_coords = np.column_stack(
                ((np.broadcast_to(from_space[0], slice_axis_coords.size)),
                 slice_axis_coords))
            pieces = np.take(self._map[from_space[0], :], slice_axis_coords)

        return np.column_stack((space_coords, pieces))

    def execute_move(
            self, move: NDArray[Shape["4"], Int]) -> NDArray[Shape["6"], Int]:
        moving_piece = self._map[move[0], move[1]]
        destination_piece = self._map[move[2], move[3]]
        self._map[move[2], move[3]] = moving_piece
        self._map[move[0], move[1]] = PieceType.NUL.value

        executed_move = np.concatenate(
            (move, np.array([moving_piece, destination_piece])))

        return executed_move

    def undo_move(
            self,
            executed_move: NDArray[Shape["6"], Int]):

        self._map[executed_move[0], executed_move[1]] = executed_move[4]
        self._map[executed_move[2], executed_move[3]] = executed_move[5]

    def get_all_spaces_occupied_by(
            self, color: PieceColor) -> NDArray[Shape["*, 2"], Int]:
        return np.argwhere(np.sign(self._map) == color.value)

    def get_general_position(self, color: PieceColor) -> Tuple[int, int]:
        for space in _castle_coords[color]:
            if abs(self._map[space]) == 1:
                return space

    def get_general_position_b(self, color: PieceColor):
        return np.argwhere(self.castles[color] == 1)

    def is_in_homeland_of(
            self, color: PieceColor, space: BoardSpace) -> bool:
        if color == PieceColor.RED:
            return space.rank >= self._river_edge_red
        if color == PieceColor.BLK:
            return space.rank <= self._river_edge_black

    def search_spaces(
            self,
            from_space: NDArray[Shape["2"], Int],
            direction: NDArray[Shape["2"], Int],
            from_space_color: PieceColor):
        possible_spaces = self.get_slice(from_space, direction)
        colors_of_possible_spaces = np.sign(possible_spaces)
        pieces = np.argwhere(colors_of_possible_spaces != 0)
        first_encountered_piece = np.min(pieces)

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
            prelim_dest = np.expand_dims(
                from_position + self.forward_direction(color), axis=0)
            destinations = self.remove_off_board_spaces(prelim_dest)
        else:
            prelim_destinations = from_position + np.row_stack(
                (self.forward_direction(color), self._horizontal_vects))
            destinations = self.remove_off_board_spaces(
                prelim_destinations)

        return np.hstack(
            (np.broadcast_to(from_position, (destinations.shape[0], 2)),
             destinations))
