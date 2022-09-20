from typing import NamedTuple
from xiangqigame.board_rules import BOARD_RULES as br
from xiangqigame.enums import PieceColor


# class CastleEdges(NamedTuple):
#     min_rank: int
#     max_rank: int
#     min_file: int
#     max_file: int


# castles = {
#     PieceColor.RED: CastleEdges(min_rank=0, max_rank=2,
#                                 min_file=3, max_file=5),
#     PieceColor.BLACK: CastleEdges(min_rank=7, max_rank=9,
#                                   min_file=3, max_file=5)
# }


# river_edges = {
#     PieceColor.RED: 4,
#     PieceColor.BLACK: 5
# }


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
        castle = br.castles[color]
        return (castle.min_rank <= self.rank <= castle.max_rank) and \
               (castle.min_file <= self.file <= castle.max_file)
