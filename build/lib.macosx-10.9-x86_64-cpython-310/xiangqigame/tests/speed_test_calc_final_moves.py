import cProfile
import json
import pkgutil
import pstats
import time
import xiangqigame.game_board_new as gbn

# game_config = json.loads(pkgutil.get_data("xiangqigame.data", "game_start.json"))
new_board = gbn.GameBoard()


def time_func_100(func):
    def wrapper(*args, **kwargs):
        start = time.time()
        for i in range(100):
            func(*args, **kwargs)
        end = time.time()

        return end - start

    return wrapper


@time_func_100
def new_board_final_moves():
    # for i in range(100):
    result = new_board.calc_final_moves_of(gbn.PColor.BLK)


print(f"calc_final_moves_of: new = {new_board_final_moves()}")

# cProfile.run("new_board_final_moves()", filename="new_board_profile_100")







