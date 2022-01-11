from typing import NamedTuple
from common.game_rules import castles
from common.enums import PieceColor


class UnComputableBoardSpace(NamedTuple):
    rank: int
    file: int


class BoardVector(NamedTuple):
    rank: int
    file: int


class BoardSpace(UnComputableBoardSpace):

    def add_board_vector(self, board_vector: BoardVector):
        return __class__(self.rank + board_vector.rank,
                         self.file + board_vector.file)

    def is_in_castle_of(self, color: PieceColor):
        castle = castles[color]
        return (castle.min_rank <= self.rank <= castle.max_rank) and \
               (castle.min_file <= self.file <= castle.max_file)
