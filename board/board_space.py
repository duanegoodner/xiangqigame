from collections import namedtuple
from typing import NamedTuple
from common.game_rules import castles
from common.enums import PieceColor
import timeit

from utilities import get_size


class BoardSpace(NamedTuple):
    rank: int
    file: int


class BoardVector(NamedTuple):
    rank: int
    file: int


class ComputableBoardSpace(BoardSpace):

    def add_vector(self, board_vector: BoardVector):
        return __class__(self.rank + board_vector.rank,
                         self.file + board_vector.file)


def board_space_plus_vector(space: BoardSpace, vector: BoardVector):
    return BoardSpace(space.rank + vector.rank, space.file + vector.file)


def is_in_castle_of(color: PieceColor, space: BoardSpace):
    castle = castles[color]
    return (castle.min_rank <= space.rank <= castle.max_rank) and \
           (castle.min_file <= space.file <= castle.max_file)


my_boring_space = BoardSpace(1, 2)
my_board_vector = BoardVector(2, 3)
my_computable_space = ComputableBoardSpace(rank=3, file=4)

my_sum = my_computable_space.add_vector(my_board_vector)

print(get_size(my_boring_space), get_size(my_board_vector),
      get_size(my_computable_space))

print(timeit.timeit(
    lambda: board_space_plus_vector(my_boring_space, my_board_vector),
    number=10))
print(timeit.timeit(
    lambda: my_computable_space.add_vector(my_board_vector),
    number=10))

# RESULT:
# 112 112 168
# 9.58899999999735e-06
# 8.635999999999366e-06
