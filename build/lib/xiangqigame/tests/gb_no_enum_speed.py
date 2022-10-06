import numpy as np
import time

import xiangqigame.game_board_no_enums as gb_ne


def time_func_10k(func):
    def wrapper(*args, **kwargs):
        start = time.time()
        for i in range(10000):
            func(*args, **kwargs)
        end = time.time()

        return end - start

    return wrapper


board_space = gb_ne.BoardSpace(rank=5, file=4)
array_space = np.array([5, 4])
board = gb_ne.GameBoard()


@time_func_10k
def is_occupied():
    result = board.is_occupied(array_space)


print(f"is_ocuppied:"
      f"default = {is_occupied()}, "
      )


@time_func_10k
def get_type():
    result = board.get_piece_type(array_space)


print(f"get_piece_type: default = {get_type()} "
      )


@time_func_10k
def get_color():
    result = board.get_piece_color(array_space)


print(f"get_piece_color: default = {get_color()}")


@time_func_10k
def get_all_spaces_occ_by():
    result = board.get_all_spaces_occupied_by(gb_ne.PCOLOR.RED)


print(f"get_all_spaces_occ_by: "
      f"default = {get_all_spaces_occ_by()} ")
