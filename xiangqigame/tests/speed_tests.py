import timeit
import random
import numpy as np
import json
import pkgutil

import time
import xiangqigame.utilities.board_utilities as bu
import xiangqigame.game_board as gb
import xiangqigame.game_board_new as gbn
from xiangqigame.tests.fixtures import starting_game_board

game_config = json.loads(pkgutil.get_data("xiangqigame.data", "game_start.json"))
old_board = gb.GameBoard(game_config["board_data"])
new_board = gbn.GameBoard()


def time_func_10k(func):
    def wrapper(*args, **kwargs):
        start = time.time()
        for i in range(10000):
            func(*args, **kwargs)
        end = time.time()

        return end - start

    return wrapper


space_old = gb.BoardSpace(rank=5, file=4)
space_new = gbn.BoardSpace(rank=5, file=4)

@time_func_10k
def get_general_pos_old():
    gen_pos = old_board.get_general_position(gb.PieceColor.RED)


@time_func_10k
def get_general_pos_new():
    gen_pos = new_board.get_general_position(gbn.PieceColor.RED)


print(f"get_general_pos: old = {get_general_pos_old()},"
      f"new = {get_general_pos_new()}")


@time_func_10k
def is_occ_old():
    is_occ = old_board.is_occupied(space_old)


@time_func_10k
def is_occ_new():
    is_occ = new_board.is_occupied(space_new)


print(f"is_occupied: old = {is_occ_old()}, new = {is_occ_new()}")











