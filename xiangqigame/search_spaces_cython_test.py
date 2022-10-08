import numpy as np
import time
from xiangqigame.game_board_new import GameBoard
import xiangqigame.starting_board_builder as bb
import xiangqigame.cython_modules.search_spaces_v1 as ss_v1
import xiangqigame.cython_modules.search_spaces_v2 as ss_v2
import xiangqigame.cython_modules.search_spaces_v3 as ss_v3
import xiangqigame.cython_modules.search_spaces_v4 as ss_v4


def time_func_100(func):
    def wrapper(*args, **kwargs):
        start = time.time()
        for i in range(100):
            func(*args, **kwargs)
        end = time.time()

        return end - start

    return wrapper


board = GameBoard()

start = np.array([5, 5], dtype=np.intc)
direction = np.array([0, 1], dtype=np.intc)


@time_func_100
def search_spaces_cython_v1():
    cython_result = ss_v1.run_search_space(
        board_map=board._map,
        start=start,
        direction=direction)


@time_func_100
def search_spaces_cython_v2():
    result = ss_v2.run_search_space(
        board_map=board._map,
        start_rank=5,
        start_file=5,
        dir_rank=0,
        dir_file=1)


@time_func_100
def search_spaces_cython_v3():
    cython_result = ss_v3.run_search_space(
        board_map=board._map,
        start=start,
        direction=direction)


# @time_func_100
def search_spaces_cython_v4():
    cython_result = ss_v4.run_search_space(
        board_map=board._map,
        start_rank=5,
        start_file=5,
        dir_rank=0,
        dir_file=1)
    print(cython_result)


@time_func_100
def search_spaces_reg():
    reg_result = board.search_spaces(from_space=(5, 5), direction=(0, 1))


print(f"regular = {search_spaces_reg()}, "
      # f"cython_v1 = {search_spaces_cython_v1()}, "
      # f"cython_v2 = {search_spaces_cython_v2()}, "
      # f"cython_v3 = {search_spaces_cython_v3()}, "
      f"cython_v4 = {search_spaces_cython_v4()}")


