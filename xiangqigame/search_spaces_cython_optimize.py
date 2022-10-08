import time
from xiangqigame.game_board_new import GameBoard
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


from_space = (0, 8)
direction = (-1, 0)

@time_func_100
def search_spaces_cython_v4():
    cython_result = ss_v4.run_search_space(
        board_map=board._map,
        start_rank=from_space[0],
        start_file=from_space[1],
        dir_rank=direction[0],
        dir_file=direction[1])


@time_func_100
def search_spaces_reg():
    reg_result = board.search_spaces(from_space=from_space, direction=direction)


print(f"Time: regular = {search_spaces_reg()}"
      f"cython_v4 = {search_spaces_cython_v4()}")


reg_result = board.search_spaces(from_space=from_space, direction=direction)
cython_v4_result = ss_v4.run_search_space(
        board_map=board._map,
        start_rank=from_space[0],
        start_file=from_space[1],
        dir_rank=direction[0],
        dir_file=direction[1])

print(reg_result)
print(cython_v4_result)
