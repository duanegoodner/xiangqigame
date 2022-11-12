import math
from typing import List, Union, Tuple
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
    def is_on_board(space: Union[List[int], Tuple[int, int]]):
        return (0 <= space[0] < BoardLayout.num_ranks) and (
                0 <= space[1] < BoardLayout.num_files)

    @staticmethod
    def is_in_homeland_of(
            color: int, space: Union[List[int], Tuple[int, int]]) -> bool:
        if color == PColor.RED:
            return space[0] >= BoardLayout.river_edges[PColor.RED]
        if color == PColor.BLK:
            return space[0] <= BoardLayout.river_edges[PColor.BLK]

    @staticmethod
    def is_in_castle_of(
            color: int, space: Union[List[int], Tuple[int, int]]) -> bool:
        castle = BoardLayout.castle_edges[color]
        return (castle[0] <= space[0] <= castle[1]) and (
                castle[2] <= space[1] <= castle[3])

    @staticmethod
    def fwd_unit_vect(color: int) -> Tuple[int, int]:
        return color, 0

    @staticmethod
    def rev_unit_vect(color: int) -> Tuple[int, int]:
        return -color, 0

    sideways_unit_vect = ((0, -1), (0, 1))

    horse_paths = {
        (1, 0): ((1, 1), (1, -1)),
        (-1, 0): ((-1, 1), (-1, -1)),
        (0, 1): ((1, 1), (-1, 1)),
        (0, -1): ((1, -1), (-1, -1))
    }

    all_orthogonal_unit_vects = (
        (0, 1), (0, -1), (1, 0), (-1, 0)
    )

    @staticmethod
    def all_orthogonal_unit_vects_from(space: Tuple[int, int]):
        orthogonal_unit_vects = []
        if space[0] != 0:
            orthogonal_unit_vects.append((-1, 0))
        if space[1] != 0:
            orthogonal_unit_vects.append((0, -1))
        if space[0] < BoardLayout.num_ranks - 1:
            orthogonal_unit_vects.append((1, 0))
        if space[1] < BoardLayout.num_files - 1:
            orthogonal_unit_vects.append((0, 1))

        return tuple(orthogonal_unit_vects)

    diag_directions = tuple(
        (rank, file) for rank in (-1, 1) for file in (-1, 1))

    @staticmethod
    def space_plus_vect(space: Tuple[int, int], vect: Tuple[int, int]):
        return space[0] + vect[0], space[1] + vect[1]


BOARD_UTILITIES = BoardUtilities()
