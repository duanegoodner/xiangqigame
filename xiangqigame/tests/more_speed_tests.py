import time
from typing import NamedTuple

import numpy as np
from enum import Enum
from dataclasses import dataclass


# reg_start = time.time()
# for i in range(10000):
#     res = abs(100)
# reg_end = time.time()
#
#
# np_start = time.time()
# for i in range(10000):
#     res = np.sign(100)
# np_end = time.time()
#
# print(reg_end - reg_start)
# print(np_end - np_start)

# my_array = np.arange(90).reshape((10, 9))
#
# my_list = [list(item) for item in list(my_array)]
#
# array_start = time.time()
# for i in range(10000):
#     result = np.argwhere(my_array == 12)
# array_end = time.time()
#
#
# list_start = time.time()
# for i in range(10000):
#     result = [(row, col) for row in range(10) for col in range(9) if my_list[row][col] == 12]
# list_end = time.time()
#
#
# print(array_end - array_start)
# print(list_end - list_start)


@dataclass
class PieceColor:
    red: int = -1
    black: int = 1


RED = -1
BLACK = 1


class Thing(Enum):
    item_a = 1
    item_b = -1


_lookup = {
    Thing.item_a: 1,
    Thing.item_b: -1
}

c = -1
# v1_start = time.time()
# for i in range(10000):
#     result = c == 1
# v1_end = time.time()
#
# print(v1_end - v1_start)
#
# v2_start = time.time()
# for i in range(10000):
#     result = Thing(c) == Thing.item_a
# v2_end = time.time()
#
# print(v2_end - v2_start)
#
# v3_start = time.time()
# for i in range(10000):
#     result = _lookup[Thing.item_a] == -1
# v3_end = time.time()


def time_func_10k(func):
    def wrapper(*args, **kwargs):
        start = time.time()
        for i in range(10000):
            func(*args, **kwargs)
        end = time.time()

        return end - start

    return wrapper


@time_func_10k
def check_color():
    result = c == 1


@time_func_10k
def check_color_2():
    result = Thing(c) == Thing.item_a


@time_func_10k
def check_color_3():
    result = c == RED


@time_func_10k
def check_color_4():
    result = _lookup[Thing.item_a] == 1


colors = PieceColor()


@time_func_10k
def check_color_5():
    result = c == colors.red


class MyColors(NamedTuple):
    red: int
    black: int
    null: int


my_colors = MyColors(red=-1, black=1, null=0)

@time_func_10k
def check_color_6():
    result = c == my_colors.red


_lookup_colors = {
    "red": -1,
    "black": 1,
    "null": 0
}


@time_func_10k
def check_color_7():
    result = c == _lookup_colors["red"]


class MoreColors(Enum):
    RED = -1
    BLACK = 1
    NULL = 0


_lookup_enum = {
    MoreColors.RED: -1,
    MoreColors.BLACK: 1,
    MoreColors.NULL: 0
}


@time_func_10k
def check_color_8():
    result = c == _lookup_enum[MoreColors.RED]


v1_time = check_color()
print(v1_time)

v2_time = check_color_2()
print(v2_time)

v3_time = check_color_3()
print(v3_time)

v4_time = check_color_4()
print(v4_time)

v5_time = check_color_5()
print(v5_time)

v6_time = check_color_6()
print(v6_time)

v7_time = check_color_7()
print(v7_time)

v8_time = check_color_8()
print(v8_time)
