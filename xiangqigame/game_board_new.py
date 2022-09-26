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


# class Move(NamedTuple):
#     start: BoardSpace
#     end: BoardSpace


# class ExecutedMove(NamedTuple):
#     spaces: NDArray[Shape["2, 2"], Int]
#     moving_piece: GamePiece
#     destination_piece: GamePiece


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

    def is_occupied(self, space: NDArray[Shape["2"], Int]):
        return self._map[tuple(space)] != 0

    def get_piece_type(self, space: NDArray[Shape["2"], Int]):
        return PieceType(self._map[space[0], space[1]])

    def get_piece_color(self, space: NDArray[Shape["2"], Int]):
        return PieceColor(
            np.sign(self._map[tuple(space)]))

    @staticmethod
    def forward_direction(color: PieceColor):
        return np.array([color.value, 0])

    # TODO consider using lower level array manip instead of set_occupant
    def set_occupant(self, space: NDArray[Shape["2"], Int], piece: GamePiece):
        self._map[space[0], space[1]] = piece
                # piece.color.value * piece.piece_type.value

    def execute_move(
            self,
            move: NDArray[Shape["4"], Int]) -> NDArray[Shape["6"], Int]:
        start = move[:2]
        end = move[2:]
        moving_piece = self._map[tuple(start)]
        destination_piece = self._map[tuple(end)]
        self._map[tuple(end)] = moving_piece
        self._map[tuple(start)] = PieceType.NUL.value

        executed_move = np.concatenate(
            (move, np.array([moving_piece, destination_piece]))
        )

        return executed_move

    def undo_move(self, executed_move: NDArray[Shape["6"], Int]):

        self.set_occupant(executed_move[:2], executed_move[4])
        self.set_occupant(executed_move[2:4], executed_move[5])

    def get_all_spaces_occupied_by(
            self, color: PieceColor) -> NDArray[Shape["*, 2"], Int]:
        return np.argwhere(np.sign(self._map) == color.value)

    def get_general_position(self, color: PieceColor):
        return np.argwhere(self._map == color.value * PieceType.GEN.value)

    def is_in_homeland_of(
            self, color: PieceColor, space: NDArray[Shape["2"], Int]) -> bool:
        if color == PieceColor.RED:
            return space[1] >= self._river_edge_red
        if color == PieceColor.BLK:
            return space[1] <= self._river_edge_black

    def remove_off_board_spaces(self, spaces: NDArray[Shape["2, *"], Int]):
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


# starting_board = StartingBoardBuilder().build_initial_board()
# print(starting_board)
# print(np.sign(starting_board))

game_board = GameBoard()
result = game_board.soldier_moves(np.array([6, 2]), color=PieceColor.RED)

print(game_board._map)

move = np.array([6, 2, 5, 2])
game_board.execute_move(move)
print()
print(game_board._map)