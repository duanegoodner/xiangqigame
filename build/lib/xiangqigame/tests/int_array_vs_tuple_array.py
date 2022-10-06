
import numpy as np
import time

from typing import NamedTuple


def time_func_10k(func):
    def wrapper(*args, **kwargs):
        start = time.time()
        for i in range(10000):
            func(*args, **kwargs)
        end = time.time()

        return end - start

    return wrapper


class PieceType(NamedTuple):
    NUL: int = 0
    GEN: int = 1
    ADV: int = 2
    ELE: int = 3
    HOR: int = 4
    CHA: int = 5
    CAN: int = 6
    SOL: int = 7


class PieceColor(NamedTuple):
    RED: int = -1
    NUL: int = 0
    BLK: int = 1


class BoardSpace(NamedTuple):
    rank: int
    file: int



