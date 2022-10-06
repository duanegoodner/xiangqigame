from typing import NamedTuple
from xiangqigame.board_components_new import BoardSpace
from xiangqigame.piece_definitions import PColor


# class CastleEdges(NamedTuple):
#     min_rank: int
#     max_rank: int
#     min_file: int
#     max_file: int


class BoardLayout:
    num_ranks = 10
    num_files = 9
    river_edges = {PColor.BLK: 4, PColor.RED: 5}
    castle_edges = {
        # (min_rank, max_rank, min_file, max_file)
        PColor.BLK: (0, 2, 3, 5),
        PColor.RED: (7, 9, 3, 5)
    }

    castle_slices = {
        PColor.BLK: (slice(0, 3), slice(3, 6)),
        PColor.RED: (slice(7, num_ranks), slice(3, 6))}

    castle_coords = {
        PColor.BLK: tuple((rank, file) for rank in range(0, 3)
                          for file in range(3, 6)),
        PColor.RED: tuple((rank, file) for rank in range(7, num_ranks)
                          for file in range(3, 6))
    }

    opponent_of: {PColor.BLK: PColor.RED, PColor.RED: PColor.BLK}
