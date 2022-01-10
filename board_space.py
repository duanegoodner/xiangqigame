from collections import namedtuple
from game_rules import castles
from enums import PieceColor


BoardSpace = namedtuple("BoardSpace", "rank file")
BoardVector = namedtuple("BoardVector", "rank file")


def board_space_plus_vector(space: BoardSpace, vector: BoardVector):
    return BoardSpace(space.rank + vector.rank, space.file + vector.file)


def is_in_castle_of(color: PieceColor, space: BoardSpace):
    castle = castles[color]
    return (castle.min_rank <= space.rank <= castle.max_rank) and\
           (castle.min_file <= space.file <= castle.max_file)
