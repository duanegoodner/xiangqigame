# import numpy as np
# from xiangqigame.move import Move
#
# board = np.array([[0, -1, -1, 0, 0, 1, 1, -1, 0],
#                   [1, 0, 1, 0, 0, 0, -1, -1, 0],
#                   [1, 0, 0, 0, 1, 1, 0, 0, 0],
#                   [0, -1, 0, 0, 1, 1, 0, 1, 0],
#                   [-1, 0, 0, 0, 0, 0, 0, 0, 0],
#                   [1, 0, 0, 0, 1, 0, -1, 0, 1],
#                   [-1, -1, 1, 0, 0, 1, 1, 0, 0],
#                   [0, 1, 0, 0, 0, 0, 1, -1, 1],
#                   [0, -1, 0, 1, 0, 0, -1, -1, -1],
#                   [0, 0, 0, 0, 0, 1, 0, -1, 1]])
#
#
# def spaces_in_direction_from(row: int, col: int, cur_board: np.array):
#     return {
#         (1, 0):  cur_board[row, (col + 1):],
#         (-1, 0): np.flip(cur_board[row, :col]),
#         (0, 1): cur_board[(row + 1):, col],
#         (0, -1): np.flip(cur_board[:row, col])
#     }
#
#
# def prefix_sums_in_direction_from(row: int, col: int, cur_board: np.array):
#
#     spaces_in_direction = spaces_in_direction_from(row, col, cur_board)
#     prefix_sums_in_direction = {direction: np.cumsum(spaces) for
#                                 direction, spaces in spaces_in_direction.items()}
#
#     return prefix_sums_in_direction
#
#
# def steps_to_non_zero_prefix_sums(row: int, col: int, cur_board: np.array):
#     prefix_sums_in_direction = prefix_sums_in_direction_from(row, col, cur_board)
#     dist_to_non_zero_prefix_sums = {
#         direction: np.argwhere(prefix_sums_in_direction[direction] != 0) for direction in
#         prefix_sums_in_direction.keys()
#     }
#     return dist_to_non_zero_prefix_sums
#
#
# def allowed_steps_to_move(row: int, col: int, cur_board: np.array):
#     spaces_in_direction = spaces_in_direction_from(row, col, cur_board)
#     prefix_sums_in_direction = {direction: np.cumsum(spaces) for
#                                 direction, spaces in
#                                 spaces_in_direction.items()}
#     dist_to_non_zero_prefix_sums = {
#         direction: np.argwhere(prefix_sums_in_direction[direction] != 0) for
#         direction in
#         prefix_sums_in_direction.keys()
#     }
#
#     allowed_destinations = {}
#     for direction in dist_to_non_zero_prefix_sums:
#         if dist_to_non_zero_prefix_sums[direction].size == 0:
#             for step in range(1, spaces_in_direction[direction].size):
#
#
#
#
# spaces_from_5_4 = spaces_in_direction_from(row=5, col=4, cur_board=board)
# prefix_sums_from_5_4 = prefix_sums_in_direction_from(row=5, col=4, cur_board=board)
# steps_to_non_zero_ps = steps_to_non_zero_prefix_sums(row=5, col=4, cur_board=board)
#
