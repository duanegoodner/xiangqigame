import math
from typing import NamedTuple

import numpy as np
from nptyping import NDArray, Int, Shape
import xiangqigame.game_board_no_enums as gb_ne


board = gb_ne.GameBoard()


def slice_pos_in_cur_file(start: NDArray[Shape["2"], Int]):
    return board._map[(start[0] + 1):, start[1]]


def slice_neg_in_cur_file(start: NDArray[Shape["2"], Int]):
    return board._map[:start[0], start[1]]


def slice_pos_in_cur_rank(start: NDArray[Shape["2"], Int]):
    return board._map[start[0], (start[1] + 1):]


def slice_neg_in_cur_rank(start: NDArray[Shape["2"], Int]):
    return board._map[start[0], :start[1]]


slice_functs = {
    (1, 0): slice_pos_in_cur_file,
    (-1, 0): slice_neg_in_cur_file,
    (0, 1): slice_pos_in_cur_rank,
    (0, -1): slice_neg_in_cur_rank
}


def get_slice_in_direction(
        start: NDArray[Shape["2"], Int], direction: NDArray[Shape["2"], Int]):
    funct_key = tuple(direction)
    return slice_functs[funct_key](start)


def get_pieces_encountered(start: NDArray[Shape["2"], Int], direction: NDArray[Shape["2"], Int]):
    funct_key = tuple(direction)
    cur_slice = slice_functs[funct_key](start)
    if (direction < 0).any():
        cur_slice = np.flip(cur_slice)
    return cur_slice


def get_first_piece_encountered_in_direction(start: NDArray[Shape["2"], Int], direction: NDArray[Shape["2"], Int]):
    pieces_in_direction = get_pieces_encountered(start, direction)
    if (pieces_in_direction != 0).any():
        position = np.min(np.where(pieces_in_direction != 0))
        color = int(math.copysign(1, pieces_in_direction[position]))
        return color, position + 1


class FirstPiece(NamedTuple):
    steps: int
    color: int


def get_allowed_steps(spaces: NDArray[Shape["2"], Int], color: int):
    if (spaces == 0).all() or (spaces.size == 0):
        return spaces.size

    first_piece_index = np.min(np.where(spaces != 0))
    if np.sign(spaces[first_piece_index]) == color:
        return first_piece_index
    else:
        return first_piece_index + 1


def get_destinations(
        start: NDArray[Shape["2"], Int],
        direction: NDArray[Shape["2"], Int],
        num_steps: int):
    steps = np.arange(1, num_steps + 1)
    move_vectors = steps.reshape((-1, 1)) + direction.reshape((1, -1))
    destinations = start + move_vectors


def get_castle_moves(start: NDArray[Shape["2"], Int], color: int):

    slices_in_dir = {
        (0, 1): board._map[start[0], (start[1] + 1):],
        (0, -1): np.flip(board._map[start[0], :start[1]]),
        (1, 0): board._map[(start[0] + 1):, start[1]],
        (-1, 0): np.flip(board._map[:start[0], start[1]])
    }

    num_steps = {
        direction: get_allowed_steps(spaces, color) for direction, spaces in
        slices_in_dir.items()
    }

    move_cts_in_dir_items = num_steps.items()
    move_count_list = [counts for direction, counts in move_cts_in_dir_items]
    cum_sums = np.cumsum(move_count_list)
    total_moves = cum_sums[-1]
    prefix_sums = np.concatenate(([0], cum_sums))[:-1]
    moves = np.empty(shape=(total_moves, 2), dtype=int)

    block_idx = 0
    for (move_dir, move_count) in move_cts_in_dir_items:
        start_row = prefix_sums[block_idx]
        end_row = start_row + move_count_list[block_idx]

        dir_array = np.array(move_dir).reshape((1, -1))
        step_array = np.arange(1, move_count + 1).reshape((-1, 1))
        dest_block = start + dir_array * step_array

        moves[start_row:end_row, :] = dest_block
        block_idx += 1

    return moves


result = get_castle_moves(start=np.array([7, 1]), color=gb_ne.PCOLOR.BLK)
print(result)







