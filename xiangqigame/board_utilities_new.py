import math
from xiangqigame.board_components_new import BoardVector, BoardSpace
from xiangqigame.board_layout import BoardLayout
from xiangqigame.piece_definitions import PColor


class BoardUtilities:

    opponent_of = {PColor.BLK: PColor.RED, PColor.RED: PColor.BLK}

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
        return (0 <= space.rank < BoardLayout.num_ranks) and (
                0 <= space.file < BoardLayout.num_files)

    @staticmethod
    def is_in_homeland_of(color: int, space: BoardSpace) -> bool:
        if color == PColor.RED:
            return space.rank >= BoardLayout.river_edges[PColor.RED]
        if color == PColor.BLK:
            return space.rank <= BoardLayout.river_edges[PColor.BLK]

    @staticmethod
    def is_in_castle_of(color: int, space: BoardSpace) -> bool:
        castle = BoardLayout.castle_edges[color]
        return (castle.min_rank <= space.rank <= castle.max_rank) and (
                castle.min_file <= space.file <= castle.max_file)

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

    all_orthogonal_unit_vects = [
        BoardVector(rank=0, file=1),
        BoardVector(rank=0, file=-1),
        BoardVector(rank=1, file=0),
        BoardVector(rank=-1, file=0)
    ]

    diag_directions = [(BoardVector(rank, file)) for rank in [-1, 1] for file
                       in [-1, 1]]


BOARD_UTILITIES = BoardUtilities()