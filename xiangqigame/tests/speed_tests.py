import timeit
from xiangqigame.game_board_new import BoardSpace
import numpy as np
import time


start = time.perf_counter()
for i in range(10000):
    my_space = BoardSpace(rank=5, file=9)
stop = time.perf_counter()
print(stop - start)


start_2 = time.perf_counter()
for j in range(10000):
    my_array = np.array([5, 9])
stop_2 = time.perf_counter()
print(stop_2 - start_2)
