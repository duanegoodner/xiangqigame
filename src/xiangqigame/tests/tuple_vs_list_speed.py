import time
import numpy as np
import xiangqigame.game_board_new as gbn


def time_func_10k(func):
    def wrapper(*args, **kwargs):
        start = time.time()
        for i in range(10000):
            func(*args, **kwargs)
        end = time.time()

        return end - start

    return wrapper


board = gbn.GameBoard()
list_map = board._map.tolist()


nt_board_space = gbn.BoardSpace(rank=3, file=2)
list_board_space = [3, 2]
arr_board_space = np.array([3, 2])


@time_func_10k
def get_piece_nt():
    result = board._map[nt_board_space.rank, nt_board_space.file]


@time_func_10k
def get_piece_list():
    result = board._map[list_board_space[0], list_board_space[1]]


@time_func_10k
def get_piece_arr():
    result = board._map[arr_board_space[0], arr_board_space[1]]


print(f"git with tuple = {get_piece_nt()}, "
      f"get with list = {get_piece_list()}, "
      f"get with arr = {get_piece_arr()}")


def index_change(start):
    for i in range(8):
        start[0] += 1
    for i in range(8):
        start[0] -= 1


@time_func_10k
def list_index_change():
    index_change(start=list_board_space)


@time_func_10k
def arr_index_change():
    index_change(start=arr_board_space)


@time_func_10k
def create_new_tuple(start):
    for i in range(8):
        start = (start[0] + 1, start[1])
    for i in range(8):
        start = (start[0] - 1, start[1])


@time_func_10k
def create_new_named_tuple(start):
    for i in range(8):
        start = gbn.BoardSpace(rank=start.rank + 1, file=start.file)
    for i in range(8):
        start = gbn.BoardSpace(rank=start.rank - 1, file=start.file)


print(f"list index change = {list_index_change()}, "
      f"arr index change = {arr_index_change()}, "
      f"create new tuple = {create_new_tuple((3, 2))}, "
      f"create new named tuple = {create_new_named_tuple(start=nt_board_space)}")


@time_func_10k
def get_piece_from_arr():
    result = board._map[list_board_space[0], list_board_space[1]]


@time_func_10k
def get_piece_from_list():
    result = list_map[list_board_space[0]][list_board_space[1]]


print(f"get from arr = {get_piece_from_arr()}, "
      f"get from list = {get_piece_from_list()}")


@time_func_10k
def list_initialization():
    result = [0, 0]


@time_func_10k
def tuple_initialization():
    result = (0, 0)


print(f"list init = {list_initialization()}, "
      f"tuple init = {tuple_initialization()}")


@time_func_10k
def create_nested_tuple():
    result = ((1, 1), (2, 2))


@time_func_10k
def create_nested_list():
    result = [[1, 1], [2, 2]]


print(f"create nested tuple = {create_nested_tuple()}, "
      f"create nested list = {create_nested_list()}")

















