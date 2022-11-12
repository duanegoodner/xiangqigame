import time
from xiangqigame.game_board_new import GameBoard
import xiangqigame.cython_modules.cython_board_utilities as cbu


def time_func_100(func):
    def wrapper(*args, **kwargs):
        start = time.time()
        for i in range(100):
            func(*args, **kwargs)
        end = time.time()

        return end - start

    return wrapper


board = GameBoard()
from_space = (0, 0)
direction = (1, 0)


@time_func_100
def current_search_space():
    result = board.search_spaces(from_space=from_space, direction=direction)
    # print(result)

@time_func_100
def search_space_2():
    result = cbu.run_search_space_2(
        board_map=board.map,
        start_rank=from_space[0],
        start_file=from_space[1],
        search_dir_rank=direction[0],
        search_dir_file=direction[1]
    )
    # print(result)


print(current_search_space())
print(search_space_2())

