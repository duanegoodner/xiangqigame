from game_rules import board_dim, river_edges, forward_direction
from enums import PieceColor
from board_space import BoardSpace
import numpy as np
from collections import namedtuple


def in_same_rank(curr_position: namedtuple, dest_position: namedtuple):
    return curr_position.rank == dest_position.rank


def in_same_file(curr_position: namedtuple, dest_position: namedtuple):
    return curr_position.file == dest_position.file


def add_board_spaces(space_a: BoardSpace, space_b: BoardSpace):
    return BoardSpace(space_a.rank + space_b.rank, space_a.file + space_b.file)


def is_on_board(space: BoardSpace):
    return all(index >= 0 for index in space) and \
           (space.rank < board_dim['num_ranks'] and
            space.file < board_dim['num_files'])


def is_in_homeland_of(piece_color: PieceColor, board_space: BoardSpace):
    return board_space.rank <= forward_direction[piece_color] * \
           river_edges[piece_color]


def get_adjacent_spaces(board_space: BoardSpace):
    unit_deltas = (0, 1), (0, -1), (1, 0), (-1, 0)
    return {add_board_spaces(board_space, BoardSpace(*delta)) for
            delta in unit_deltas if
            is_on_board(add_board_spaces(board_space, BoardSpace(*delta)))}


def get_side_adjacent_spaces(board_space: BoardSpace):
    unit_deltas = (0, 1), (0, -1)
    return {add_board_spaces(board_space, BoardSpace(*delta)) for
            delta in unit_deltas if
            is_on_board(add_board_spaces(board_space, BoardSpace(*delta)))}



