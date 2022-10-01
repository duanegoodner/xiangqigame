import math
from typing import NamedTuple

import numpy as np
from nptyping import NDArray, Int, Shape
import xiangqigame.game_board_no_enums as gb_ne


board = gb_ne.GameBoard()


def orthogonal_pieces(array, cur_rank: int, cur_file: int, color: int):
    piece_ranks, piece_files = np.nonzero(array)
    # cur_rank = space[0]
    # cur_file = space[1]

    files_above = piece_files[(piece_files > cur_file) & (piece_ranks == cur_rank)]
    above_cur_file = files_above[0] if files_above.any() else 8
    destination_files_above_cur = np.arange(cur_file + 1, above_cur_file + 1)
    destination_spaces_above_cur_file = np.column_stack((
        np.full(above_cur_file - cur_file, cur_rank), destination_files_above_cur))

    files_below = piece_files[(piece_files < cur_file) & (piece_ranks == cur_rank)]
    below_cur_file = files_below[-1] if files_below.any() else 0
    # destinations_files_below_cur_file = np.arange(below_cur_file, cur_file)
    # destination_spaces_below_cur_file = np.column_stack((
    #     np.full(cur_file - below_cur_file, cur_rank), destinations_files_below_cur_file
    # ))

    ranks_above = piece_ranks[(piece_ranks > cur_rank) & (piece_files == cur_file)]
    above_cur_rank = ranks_above[0] if ranks_above.any() else 9
    # destination_ranks_above_cur = np.arange(cur_rank + 1, above_cur_rank + 1)
    # destination_spaces_above_cur_rank = np.column_stack((
    #     destination_ranks_above_cur, np.full(above_cur_rank - cur_rank, cur_file)
    # ))

    ranks_below = piece_ranks[(piece_ranks < cur_rank) & (piece_files == cur_file)]
    below_cur_rank = ranks_below[-1] if ranks_below.any() else 0
    # destination_ranks_below_cur = np.arange(below_cur_rank, cur_rank)
    # destination_spaces_below_cur_rank = np.column_stack((
    #     destination_ranks_below_cur, np.full(cur_rank - below_cur_rank, cur_file)
    # ))


orthogonal_pieces(array=board._map, cur_rank=3, cur_file=1, color=1)






# def slice_pos_in_cur_file(start: NDArray[Shape["2"], Int]):
#     return board._map[(start[0] + 1):, start[1]]
#
#
# def slice_neg_in_cur_file(start: NDArray[Shape["2"], Int]):
#     return board._map[:start[0], start[1]]
#
#
# def slice_pos_in_cur_rank(start: NDArray[Shape["2"], Int]):
#     return board._map[start[0], (start[1] + 1):]
#
#
# def slice_neg_in_cur_rank(start: NDArray[Shape["2"], Int]):
#     return board._map[start[0], :start[1]]
#
#
# slice_functs = {
#     (1, 0): slice_pos_in_cur_file,
#     (-1, 0): slice_neg_in_cur_file,
#     (0, 1): slice_pos_in_cur_rank,
#     (0, -1): slice_neg_in_cur_rank
# }
#
#
# def get_slice_in_direction(
#         start: NDArray[Shape["2"], Int], direction: NDArray[Shape["2"], Int]):
#     funct_key = tuple(direction)
#     return slice_functs[funct_key](start)
#
#
# def get_pieces_encountered(start: NDArray[Shape["2"], Int], direction: NDArray[Shape["2"], Int]):
#     funct_key = tuple(direction)
#     cur_slice = slice_functs[funct_key](start)
#     if (direction < 0).any():
#         cur_slice = np.flip(cur_slice)
#     return cur_slice
#
#
# def get_first_piece_encountered_in_direction(start: NDArray[Shape["2"], Int], direction: NDArray[Shape["2"], Int]):
#     pieces_in_direction = get_pieces_encountered(start, direction)
#     if (pieces_in_direction != 0).any():
#         position = np.min(np.where(pieces_in_direction != 0))
#         color = int(math.copysign(1, pieces_in_direction[position]))
#         return color, position + 1
#
#
# class FirstPiece(NamedTuple):
#     steps: int
#     color: int
#
#
# def get_allowed_steps(spaces: NDArray[Shape["*, 2"], Int], color: int):
#     if (spaces == 0).all() or (spaces.size == 0):
#         return spaces.size
#
#     first_piece_index = np.min(np.where(spaces != 0))
#     if np.sign(spaces[first_piece_index]) == color:
#         return first_piece_index
#     else:
#         return first_piece_index + 1
#
#
# def get_castle_moves(start: NDArray[Shape["2"], Int], color: int):
#
#     slices_in_dir = {
#         (0, 1): board._map[start[0], (start[1] + 1):],
#         (0, -1): np.flip(board._map[start[0], :start[1]]),
#         (1, 0): board._map[(start[0] + 1):, start[1]],
#         (-1, 0): np.flip(board._map[:start[0], start[1]])
#     }
#
#     # num_steps = {
#     #     direction: get_allowed_steps(spaces, color) for direction, spaces in
#     #     slices_in_dir.items()
#     # }
#
#     # move_cts_in_dir_items = num_steps.items()
#
#     move_cts_in_dir_items = [
#         (direction, get_allowed_steps(spaces, color)) for direction, spaces in
#         slices_in_dir.items()
#     ]
#
#     move_count_list = np.array([counts for direction, counts in move_cts_in_dir_items])
#     cum_sums = np.cumsum(move_count_list)
#     total_moves = cum_sums[-1]
#     prefix_sums = np.concatenate(([0], cum_sums))[:-1]
#     moves = np.empty(shape=(total_moves, 2), dtype=int)
#
#     block_idx = 0
#     for (move_dir, move_count) in move_cts_in_dir_items:
#         start_row = prefix_sums[block_idx]
#         end_row = start_row + move_count_list[block_idx]
#
#         dir_array = np.array(move_dir).reshape((1, -1))
#         step_array = np.arange(1, move_count + 1).reshape((-1, 1))
#
#         moves[start_row:end_row, :] = start + dir_array * step_array
#         block_idx += 1
#
#     return moves
#
#
# result = get_castle_moves(start=np.array([7, 1]), color=gb_ne.PCOLOR.BLK)
# print(result)
#
#
#
#



