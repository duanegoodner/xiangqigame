from collections import namedtuple
from typing import TypedDict


# BoardSpace = namedtuple("BoardSpace", "rank file")
# BoardVector = namedtuple("BoardVector", "rank file")
#
#
# def add_board_spaces(space_a: BoardSpace, space_b: BoardSpace):
#     return BoardSpace(space_a.rank + space_b.rank, space_a.file + space_b.file)
#
#
# def board_space_plus_vect(space: BoardSpace, vector: BoardVector):
#     return BoardSpace(space.rank + vector.rank, space.file + vector.file)


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


my_space = BoardSpace(rank=0, file=0)
my_other_space = BoardSpace(rank=2, file = 3)
my_board_vector = BoardVector(rank=1, file=1)
my_other_board_vector = BoardVector(rank=2, file=3)

my_space_list = [my_space, my_other_space]

my_double_list = my_space_list + my_space_list

print(my_double_list)
