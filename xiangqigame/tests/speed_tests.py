import timeit
import random
import numpy as np
import time


# start = time.perf_counter()
# for i in range(10000):
#     my_space = BoardSpace(rank=5, file=9)
# stop = time.perf_counter()
# print(stop - start)
#
#
# start_2 = time.perf_counter()
# for j in range(10000):
#     my_array = np.array([5, 9])
# stop_2 = time.perf_counter()
# print(stop_2 - start_2)


random_list = [random.randint(0, 1000) for item in range(100000)]
random_array = np.array(random_list)

list_start = time.time()
random_list.sort()
list_end = time.time()

array_start = time.time()
np.sort(random_array)
array_end = time.time()

print(list_end - list_start)
print(array_end - array_start)

