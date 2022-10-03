import math

from xiangqigame.board_components_new import BoardVector, BoardSpace
from xiangqigame.board_rules_new import PType, PColor, RULES


class BoardUtilities:

    @staticmethod
    def get_piece_color(piece: int):
        return 0 if piece == 0 else int(math.copysign(1, piece))

    @staticmethod
    def _is_black(piece: int):
        return piece > 0

    @staticmethod
    def _is_red(piece: int):
        return piece < 0

    @staticmethod
    def _is_null(piece: int):
        return piece == 0

    is_color = {
        PColor.BLK: _is_black,
        PColor.RED: _is_red,
        PColor.NUL: _is_null
    }

    @staticmethod
    def is_on_board(space: BoardSpace):
        return (0 <= space.rank < RULES.num_ranks) and (
                0 <= space.file < RULES.num_files)

    @staticmethod
    def is_in_homeland_of(color: int, space: BoardSpace) -> bool:
        if color == PColor.RED:
            return space.rank >= RULES.river_edges[PColor.RED]
        if color == PColor.BLK:
            return space.rank <= RULES.river_edges[PColor.BLK]

    @staticmethod
    def fwd_unit_vect(color: int):
        return BoardVector(rank=color, file=0)

    @staticmethod
    def rev_unit_vect(color: int):
        return BoardVector(rank=(-1 * color), file=0)

    sideways_unit_vect = (
        BoardVector(rank=0, file=-1), BoardVector(rank=0, file=1))

    horse_paths = {
        BoardVector(1, 0): (BoardVector(1, 1), BoardVector(1, -1)),
        BoardVector(-1, 0): (BoardVector(-1, 1), BoardVector(-1, -1)),
        BoardVector(0, 1): (BoardVector(1, 1), BoardVector(-1, 1)),
        BoardVector(0, -1): (BoardVector(1, -1), BoardVector(-1, -1))
    }

    diag_directions = [(BoardVector(rank, file)) for rank in [-1, 1] for file
                       in [-1, 1]]

    @property
    def castle_slices(self):
        black_edges = RULES.castle_slices[PColor.BLK]
        red_edges = RULES.castle_slices[PColor.RED]
        return {
            PColor.BLK: (
                slice(black_edges.min_rank, black_edges.max_rank),
                slice(black_edges.min_file, black_edges.max_file)),
            PColor.RED: (
                slice(red_edges.min_rank, red_edges.max_rank),
                slice(black_edges.min_file, black_edges.max_file))
        }

    @property
    def castle_coords(self):
        black_edges = RULES.castle_slices[PColor.BLK]
        red_edges = RULES.castle_slices[PColor.RED]
        return {
            PColor.BLK: [[(rank, file)] for rank in range(0, black_edges.max_rank + 1)
            for file in range(black_edges.min_file, black_edges.max_file + 1)],
            PColor.RED: [[(rank, file)] for rank in range(0, red_edges.max_rank + 1)
            for file in range(red_edges.min_file, red_edges.max_file + 1)]
        }

    all_orthogonal_unit_vects = [
        BoardVector(rank=0, file=1),
        BoardVector(rank=0, file=-1),
        BoardVector(rank=1, file=0),
        BoardVector(rank=-1, file=0)
    ]







