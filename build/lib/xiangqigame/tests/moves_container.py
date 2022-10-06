from xiangqigame.game_board import GameBoard, PieceColor, BoardSpace, Move
import json
import numpy as np
import pkgutil
import time


game_config = json.loads(pkgutil.get_data("xiangqigame.data", "game_start.json"))
old_board = GameBoard(game_config["board_data"])


def time_func_10k(func):
    def wrapper(*args, **kwargs):
        start = time.time()
        for i in range(10000):
            func(*args, **kwargs)
        end = time.time()

        return end - start

    return wrapper


moves_set = old_board.calc_final_moves_of(PieceColor.BLACK)
moves_list = list(moves_set)
moves_array = np.array(moves_list)

destinations_set = {move.end for move in moves_set}
destinations_list = [move.end for move in moves_list]
destination_array = np.array(destinations_list)


@time_func_10k
def convert_set_to_list():
    result = list(destinations_set)


@time_func_10k
def convert_list_to_array():
    result = np.array(destinations_list)


@time_func_10k
def convert_list_to_set():
    result = set(destinations_list)


print(f"set to list conversion = {convert_set_to_list()}, "
      f"list to array = {convert_list_to_array()}, "
      f"list to set = {convert_list_to_set()}")


@time_func_10k
def dest_extract_set():
    result = {move.end for move in moves_set}


@time_func_10k
def dest_extract_list():
    result = [move.end for move in moves_list]


print(f"set dest extraction = {dest_extract_set()}, "
      f"list dest extraction = {dest_extract_list()}")





@time_func_10k
def set_container():
    # black_destinations = {move.end for move in old_board.calc_final_moves_of(PieceColor.BLACK)}
    red_gen_position = old_board.get_general_position(PieceColor.RED)
    result = red_gen_position in destinations_set


@time_func_10k
def list_container():
    # black_destinations = [move.end for move in
    #                       old_board.calc_final_moves_of(PieceColor.BLACK)]
    red_gen_position = old_board.get_general_position(PieceColor.RED)
    result = red_gen_position in destinations_list


@time_func_10k
def array_container():
    red_gen_position = old_board.get_general_position(PieceColor.RED)
    result = moves_array[
        (moves_array[:, :, 0] == red_gen_position.rank) &
        (moves_array[:, :, 1] == red_gen_position.file)
    ]


print(f"search set = {set_container()}, "
      f"search list = {list_container()}, "
      f"search array = {array_container()}")


moves_list = [
    Move(start=BoardSpace(rank=i, file=j), end=BoardSpace(rank=i+1, file=j+1))
    for i in range(8) for j in range(9)]

@time_func_10k
def set_construction():
    moves = set()
    for move in moves_list:
        moves.update(move)


@time_func_10k
def list_construction():
    moves = []
    for move in moves_list:
        moves.append(move)


print(f"set construction = {set_construction()}, "
      f"list construction = {list_construction()}")