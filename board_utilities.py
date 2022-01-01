from game_rules import board_dim, river_edges, forward_direction
from enums import PieceColor
import numpy as np


def in_same_rank(curr_position: tuple, dest_position: tuple):
    return curr_position[0] == dest_position[0]


def in_same_file(curr_position: tuple, dest_position: tuple):
    return curr_position[1] == dest_position[1]


def is_on_board(space: tuple):
    return all(index >= 0 for index in space) and \
           (space[0] < board_dim['num_ranks'] and
            space[1] < board_dim['num_files'])


def is_in_homeland_of(piece_color: PieceColor, board_space: tuple):
    return board_space[1] <= forward_direction[piece_color] * \
           river_edges[piece_color]


def get_adjacent_spaces(space: tuple):
    unit_deltas = (0, 1), (0, -1), (1, 0), (-1, 0)
    return {tuple(np.add(space, delta)) for delta in unit_deltas
            if is_on_board(tuple(np.add(space, delta)))}



