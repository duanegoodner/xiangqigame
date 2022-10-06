import json
import numpy as np
import pkgutil
import time

import xiangqigame.game_board as gb_orig
import xiangqigame.game_board_no_enums as gb_ne
import xiangqigame.tests.search_spaces_new as ssn


def time_func_10k(func):
    def wrapper(*args, **kwargs):
        start = time.time()
        for i in range(10000):
            func(*args, **kwargs)
        end = time.time()

        return end - start

    return wrapper


game_config = json.loads(
    pkgutil.get_data("xiangqigame.data", "game_start.json"))
orig_board = gb_orig.GameBoard(game_config["board_data"])
orig_space = gb_orig.BoardSpace(rank=3, file=2)

new_board = gb_ne.GameBoard()
new_space = np.array([3, 2])


@time_func_10k
def orig_chariot():
    result = orig_board.chariot_moves(
        from_position=orig_space,
        color=gb_orig.PieceColor.BLACK)


@time_func_10k
def new_chariot():
    result = new_board.chariot_moves(start=new_space, color=gb_ne.PCOLOR.BLK)



@time_func_10k
def ortho_pieces():
    result = ssn.orthogonal_pieces(array=new_board._map, cur_rank=3, cur_file=2, color=1)






print(f"orig chariot = {orig_chariot()}, "
      f"new chariot = {new_chariot()}, "
      f"ortho_pieces = {ortho_pieces()}"
      # f"old search_space = {old_search_spaces()}"
      # f"new allowed_steps = {new_get_allowed_steps()}"
      )
