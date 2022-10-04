from typing import NamedTuple
from xiangqigame.board_components_new import BoardSpace
from xiangqigame.piece_definitions import PColor


class CastleEdges(NamedTuple):
    min_rank: int
    max_rank: int
    min_file: int
    max_file: int


class BoardLayout:
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

    castle_coords = {
        PColor.BLK: tuple(
            BoardSpace(rank=rank, file=file) for rank in range(0, 3)
            for file in range(3, 6)),
        PColor.RED: tuple(BoardSpace(rank=rank, file=file) for rank in
                          range(7, num_ranks) for file in range(3, 6))
    }

    opponent_of: {PColor.BLK: PColor.RED, PColor.RED: PColor.BLK}
