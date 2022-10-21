from xiangqigame.game_board_new import GameBoard
from xiangqigame.piece_definitions import PColor
import xiangqigame.cython_modules.cannon_moves as cm
import time



def time_func_100(func):
    def wrapper(*args, **kwargs):
        start = time.time()
        for i in range(100):
            func(*args, **kwargs)
        end = time.time()

        return end - start

    return wrapper


board = GameBoard()

from_space = (7, 1)


@time_func_100
def cannon_moves_reg():
    result = board.cannon_moves(from_position=from_space, color=PColor.RED)


# @time_func_100
def cannon_moves_cython():
    result = cm.run_cannon_moves(
        from_rank=from_space[0],
        from_file=from_space[1],
        board_map = board.map,
        color=PColor.RED)


print(cannon_moves_cython())

