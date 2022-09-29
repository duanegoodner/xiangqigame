import numpy as np
import time

import xiangqigame.game_board_no_enums as gb_ne


def time_func_10k(func):
    def wrapper(*args, **kwargs):
        start = time.time()
        for i in range(1000000):
            func(*args, **kwargs)
        end = time.time()

        return end - start

    return wrapper


board_space = gb_ne.BoardSpace(rank=5, file=4)
board = gb_ne.GameBoard()


@time_func_10k
def is_occupied_int():
    result = board.is_occupied_int(board_space)


@time_func_10k
def is_occupied_td():
    result = board.is_occupied_td(board_space)


@time_func_10k
def is_occupied_ct():
    result = board.is_occupied_ct(board_space)


# print(f"is_ocuppied:"
#       f"int = {is_occupied_int()},"
#       f"td = {is_occupied_td()},"
#       f"ct = {is_occupied_ct()}")


@time_func_10k
def get_type_int():
    result = board.get_piece_type_int(board_space)


@time_func_10k
def get_type_td():
    result = board.get_piece_type_td(board_space)


# print(f"get_piece_type: int = {get_type_int()}, td = {get_type_td()}")


@time_func_10k
def get_color_int():
    result = board.get_piece_color_int(board_space)


@time_func_10k
def get_color_td():
    result = board.get_piece_type_td(board_space)


# print(f"get_piece_color: int = {get_color_int()}, td = {get_color_td()}")


@time_func_10k
def get_all_spaces_occ_by_int():
    result = board.get_all_spaces_occupied_by_int(gb_ne.PCOLOR.RED)


@time_func_10k
def get_all_spaces_occ_by_int_lam():
    result = board.get_all_spaces_occupied_by_int_lambda(gb_ne.PCOLOR.RED)


@time_func_10k
def get_all_spaces_occ_by_int_vect():
    result = board.get_all_spaces_occupied_by_int_vect(gb_ne.PCOLOR.RED)


@time_func_10k
def get_all_spaces_occ_by_int_non_lam_dict():
    result = board.get_all_spaces_occupied_by_int_non_lam_dict(gb_ne.PCOLOR.RED)


@time_func_10k
def get_all_spaces_occ_by_td():
    result = board.get_all_spaces_occupied_by_td(gb_ne.PCOLOR.RED)


@time_func_10k
def get_all_spaces_occ_by_ct():
    result = board.get_all_spaces_occupied_by_ct(gb_ne.PCOLOR.RED)


print(f"get_all_spaces_occ_by: "
      # f"int = {get_all_spaces_occ_by_int()}, "
      f"int_lambda = {get_all_spaces_occ_by_int_lam()} "
      # f"int_vect = {get_all_spaces_occ_by_int_vect()} "
      f"non_lam_dict = {get_all_spaces_occ_by_int_non_lam_dict()} "
      # f"td = {get_all_spaces_occ_by_td()} "
      # f"ct = {get_all_spaces_occ_by_ct()}, "
      )
