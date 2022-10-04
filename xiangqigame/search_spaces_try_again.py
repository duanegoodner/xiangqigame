import numpy as np
import time
import xiangqigame.game_board_new as gbn

board = gbn.GameBoard()


def time_func_10k(func):
    def wrapper(*args, **kwargs):
        start = time.time()
        for i in range(10000):
            func(*args, **kwargs)
        end = time.time()

        return end - start

    return wrapper


def my_search(from_space: gbn.BoardSpace, search_axis: int,
              search_direction: int):

    num_empty_spaces = 0
    empty_spaces = []
    range_obj = None
    first_occupied_space = None

    if search_direction == 1:
        search_slice = slice(
            from_space[search_axis] + 1, board._map.shape[search_axis])
    else:
        search_slice = slice(0, from_space[search_axis])

    if search_axis == 0:
        spaces_in_dir = board._map[search_slice, from_space[1]]
    else:
        spaces_in_dir = board._map[from_space[0], search_slice]

    indices_of_nonzero_spaces = np.where(spaces_in_dir != 0)[0]
    if indices_of_nonzero_spaces.size == 0:
        num_empty_spaces = spaces_in_dir.size
    elif search_direction == 1:
        num_empty_spaces = indices_of_nonzero_spaces[0]
    elif search_direction == -1:
        num_empty_spaces = spaces_in_dir.size - indices_of_nonzero_spaces[-1] - 1

    if search_direction == 1:
        range_obj = range(
            from_space[search_axis] + 1,
            from_space[search_axis] + num_empty_spaces + 1)
    if search_direction == -1:
        range_obj = range(from_space[search_axis] - num_empty_spaces,
                          from_space[search_axis])

    for step in range_obj:
        if search_axis == 0:
            new_dest = gbn.BoardSpace(rank=step, file=from_space.file)
            empty_spaces.append(new_dest)
        if search_axis == 1:
            new_dest = gbn.BoardSpace(rank=from_space.rank, file=step)
            empty_spaces.append(new_dest)

    return empty_spaces


# print(my_search(
#     from_space=gbn.BoardSpace(rank=8, file=0),
#     search_axis=1,
#     search_direction=1
#     ))

search_from = gbn.BoardSpace(rank=0, file=0)
search_direction = gbn.BoardVector(rank=1, file=0)


@time_func_10k
def standard_search_spaces():
    board.search_spaces(from_space=search_from, direction=search_direction)


@time_func_10k
def new_search_spaces():
    my_search(from_space=search_from, search_axis=0, search_direction=1)


print(f"standard = {standard_search_spaces()}, "
      f"new = {new_search_spaces()}")
