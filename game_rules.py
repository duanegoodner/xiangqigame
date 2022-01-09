from enums import PieceColor
from collections import namedtuple


board_dim = {
    'num_files': 9,
    'num_ranks': 10
}


opponent_of = {
    PieceColor.RED: PieceColor.BLACK,
    PieceColor.BLACK: PieceColor.RED
}

forward_direction = {
    PieceColor.RED: 1,
    PieceColor.BLACK: -1
}

CastleEdges = namedtuple("CastleEdges", "min_rank max_rank min_file max_file")

castles = {
    PieceColor.RED: CastleEdges(min_rank=0, max_rank=2,
                                min_file=3, max_file=5),
    PieceColor.BLACK: CastleEdges(min_rank=7, max_rank=9,
                                  min_file=3, max_file=5)
}


river_edges = {
    PieceColor.RED: 4,
    PieceColor.BLACK: 5
}
