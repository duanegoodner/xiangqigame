import time
import json
import pkgutil
import numpy as np
from typing import TypedDict
import xiangqigame.game_board as gb
import xiangqigame.game_board_new as gbn
import xiangqigame.game_board_no_enums as gb_ne


def time_func_10k(func):
    def wrapper(*args, **kwargs):
        start = time.time()
        for i in range(10000):
            func(*args, **kwargs)
        end = time.time()

        return end - start

    return wrapper


game_config = json.loads(pkgutil.get_data("xiangqigame.data", "game_start.json"))
old_board = gb.GameBoard(game_config["board_data"])
new_board = gbn.GameBoard()
newest_board = gb_ne.GameBoard()

space_old = gb.BoardSpace(rank=5, file=4)
space_new = gbn.BoardSpace(rank=5, file=4)
space_newest = np.array([5, 4])


@time_func_10k
def get_type_and_color_old():
    piece = old_board._map[space_old]
    p_type = piece["type"]
    p_color = piece["color"]


@time_func_10k
def get_type_and_color_new():
    result = divmod(new_board._map[space_new], 7)


print(f"get_type_and_color: old = {get_type_and_color_old()}, "
      f"new = {get_type_and_color_new()}")


def get_color_old(space: tuple):
    return old_board._map[space]["color"]


class BoardSpace(TypedDict):
    rank: int
    file: int


def get_piece_color_old_td(board_space: BoardSpace):
    return old_board._map[board_space["rank"], board_space["file"]]["color"]


get_piece_color_old_td_vect = np.vectorize(get_piece_color_old_td)


print(f"piece_color_old_td = {get_piece_color_old_td(BoardSpace(rank=3, file=2))}")



my_spaces = np.array([[BoardSpace(rank=row, file=col) for col in range(9)]
                      for row in range(10)])

print(f"vectorized result = {get_piece_color_old_td_vect(my_spaces)}")


@time_func_10k
def vectorized_time_td():
    result = get_piece_color_old_td_vect(my_spaces)


print(f"vecotrized get color 2 spaces = {vectorized_time_td()}")


new_board_sign = np.sign(new_board._map)
new_board_ptype = abs(new_board._map)

print(new_board_sign)
print(new_board_ptype)



