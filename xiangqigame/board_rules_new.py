from typing import NamedTuple


class PieceType(NamedTuple):
    NUL: int = 0
    GEN: int = 1
    ADV: int = 2
    ELE: int = 3
    HOR: int = 4
    CHA: int = 5
    CAN: int = 6
    SOL: int = 7


PType = PieceType()


class PieceColor(NamedTuple):
    RED: int = -1
    NUL: int = 0
    BLK: int = 1


PColor = PieceColor()


class CastleEdges(NamedTuple):
    min_rank: int
    max_rank: int
    min_file: int
    max_file: int


class BoardRules:

    num_ranks = 10
    num_files = 9
    river_edges = {PColor.BLK: 4, PColor.RED: 5}
    castle_edges = {
        PColor.BLK: CastleEdges(min_rank=0, max_rank=2, min_file=3, max_file=5),
        PColor.RED: CastleEdges(min_rank=7, max_rank=9, min_file=3, max_file=5)
    }
    castle_slices = {
        PColor.BLK: (slice(0, 3), slice(3, 6)),
        PColor.RED: (slice(7, num_ranks), slice(3, 6))}
    opponent_of: {PColor.BLK: PColor.RED, PColor.RED: PColor.BLK}


RULES = BoardRules()


