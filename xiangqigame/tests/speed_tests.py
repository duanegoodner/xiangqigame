import timeit
import random
import numpy as np
import json
import pkgutil

import time
import xiangqigame.game_board as gb
import xiangqigame.game_board_new as gbn
import xiangqigame.game_board_no_enums as gb_ne

game_config = json.loads(pkgutil.get_data("xiangqigame.data", "game_start.json"))
old_board = gb.GameBoard(game_config["board_data"])
new_board = gbn.GameBoard()
newest_board = gb_ne.GameBoard()

old_move = gb.Move(start=gb.BoardSpace(rank=6, file=2), end=gb.BoardSpace(rank=5, file=2))
new_move = np.array([6, 2, 5, 2])


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
space_newest = np.array([5, 4])

@time_func_10k
def get_general_pos_old():
    gen_pos = old_board.get_general_position(gb.PieceColor.RED)


@time_func_10k
def get_general_pos_new():
    gen_pos = new_board.get_general_position(gbn.PieceColor.RED)


@time_func_10k
def get_general_newest():
    gen_position = newest_board.get_general_position(gb_ne.PCOLOR.RED)


print(f"get_general_pos: old = {get_general_pos_old()},"
      f"new = {get_general_pos_new()} "
      f"newest = {get_general_newest()}")


@time_func_10k
def is_occ_old():
    is_occ = old_board.is_occupied(space_old)


@time_func_10k
def is_occ_new():
    is_occ = new_board.is_occupied(space_new)


@time_func_10k
def is_occ_newest():
    is_occ = newest_board.is_occupied(space_newest)


print(f"is_occupied: old = {is_occ_old()}, "
      f"new = {is_occ_new()}, "
      f"newest = {is_occ_newest()}")


@time_func_10k
def get_piece_type_old():
    res = old_board.get_type(space_old)


@time_func_10k
def get_piece_type_new():
    res = new_board.get_piece_type(space_new)


@time_func_10k
def get_piece_type_newest():
    res = newest_board.get_piece_type(space_newest)


print(f"get_piece_type: old = {get_piece_type_old()}, "
      f"new = {get_piece_type_new()}, "
      f"newest = {get_piece_type_newest()}")


@time_func_10k
def get_piece_color_old():
    res = old_board.get_color(space_old)


@time_func_10k
def get_piece_color_new():
    res = new_board.get_piece_color(space_newest)


@time_func_10k
def get_piece_color_newest():
    res = newest_board.get_piece_color(space_newest)


print(f"get_piece_color: old = {get_piece_color_old()}, "
      f"new = {get_piece_color_new()}, "
      f"newest = {get_piece_color_newest()}")


@time_func_10k
def get_all_spaces_occ_by_old():
    res = old_board.get_all_spaces_occupied_by(gb.PieceColor.RED)


@time_func_10k
def get_all_spaces_occ_by_new():
    res = new_board.get_all_spaces_occupied_by(gbn.PieceColor.RED)


@time_func_10k
def get_all_spaces_occ_by_newest():
    res = newest_board.get_all_spaces_occupied_by(gb_ne.PCOLOR.RED)


print(f"get_all_spaces_occ_by: old = {get_all_spaces_occ_by_old()}, "
      f"new = {get_all_spaces_occ_by_new()}, "
      f"newest = {get_all_spaces_occ_by_newest()}")


@time_func_10k
def execute_move_old():
    cur_move = old_board.execute_move(old_move)
    old_board.undo_move(cur_move)


@time_func_10k
def execute_move_new():
    cur_move = new_board.execute_move(new_move)
    new_board.undo_move(cur_move)


print(f"new_move: old = {execute_move_old()}, "
      f"new = {execute_move_new()}, ")


@time_func_10k
def soldier_moves_old():
    result = old_board.soldier_moves(space_old, color=gb.PieceColor.BLACK)


@time_func_10k
def soldier_moves_new():
    result = new_board.soldier_moves(space_newest, color=gbn.PieceColor.BLK)


print(f"soldier_moves: old = {soldier_moves_old()}, "
      f"new = {soldier_moves_new()}")














