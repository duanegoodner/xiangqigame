from collections import namedtuple
from typing import TypedDict
from game_rules import castles
from enums import PieceColor
from utilities import get_size
import timeit


BoardSpaceTup = namedtuple("BoardSpace", "rank file")
BoardVectorTup = namedtuple("BoardVector", "rank file")

#
# def add_board_spaces(space_a: BoardSpace, space_b: BoardSpace):
#     return BoardSpace(space_a.rank + space_b.rank, space_a.file + space_b.file)
#
#


def board_space_plus_vect(space: BoardSpaceTup, vector: BoardVectorTup):
    return BoardSpace(space.rank + vector.rank, space.file + vector.file)


class BoardVector:

    def __init__(self, rank: int, file: int):
        self._rank = rank
        self._file = file

    @property
    def rank(self):
        return self._rank

    @property
    def file(self):
        return self._file

    def __repr__(self):
        return f'BoardVector(rank={self.rank}, file={self.file})'

    def __add__(self, other):
        return BoardVector(rank=self.rank + other.rank,
                           file=self.file + other.file)


class BoardSpace(BoardVector):

    def __repr__(self):
        return f'BoardSpace(rank={self.rank}, file={self.file})'

    def __add__(self, other):
        raise Exception("Two BoardSpace objects cannot be added together")

    def add_board_vector(self, board_vector: BoardVector):
        return BoardSpace(rank=self.rank + board_vector.rank,
                          file=self.file + board_vector.file)

    def is_in_castle_of(self, color: PieceColor):
        castle = castles[color]
        return (castle.min_rank <= self.rank <= castle.max_rank) and\
               (castle.min_file <= self.file <= castle.max_file)


board_space_tup = BoardSpaceTup(1, 1)
board_vect_tup = BoardVectorTup(1, 1)
board_space_class = BoardSpace(1, 1)
standard_tup = (1, 1)

board_vect_class = BoardVector(1, 1)

def class_addition():
    return board_space_class.add_board_vector(board_vect_class)


def tup_addition():
    return board_space_plus_vect(board_space_tup, board_vect_tup)


print(get_size(standard_tup), get_size(board_space_tup), get_size(board_space_class))
print(timeit.Timer(class_addition).timeit(number=10))
print(timeit.Timer(tup_addition).timeit(number=10))

# Output
# 84 84 288
# 1.773499999999789e-05
# 9.310000000005425e-06
