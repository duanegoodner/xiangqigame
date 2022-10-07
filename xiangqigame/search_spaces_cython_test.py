
import numpy as np
import time
from xiangqigame.game_board_new import GameBoard
import xiangqigame.starting_board_builder as bb
import xiangqigame.cython_modules.search_spaces as ss


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
def search_spaces_cython():
    for row in range(board._map.shape[0]):
        for col in range(board._map.shape[1]):
            cython_resul_a = ss.run_search_space(
                board_map=board._map,
                start=np.array([row, col], dtype=np.intc),
                direction=np.array([0, 1], dtype=np.intc))


@time_func_100
def search_spaces_reg():
    for row in range(board._map.shape[0]):
        for col in range(board._map.shape[1]):
            reg_result = board.search_spaces(from_space=(row, col), direction=(0, 1))


print(f"cython = {search_spaces_cython()}, "
      f"regular = {search_spaces_reg()}")


