from collections import namedtuple


BoardSpace = namedtuple("BoardSpace", "rank file")
BoardVector = namedtuple("BoardVector", "rank file")


def add_board_spaces(space_a: BoardSpace, space_b: BoardSpace):
    return BoardSpace(space_a.rank + space_b.rank, space_a.file + space_b.file)


def board_space_plus_vect(space: BoardSpace, vector: BoardVector):
    return BoardSpace(space.rank + vector.rank, space.file + vector.file)



