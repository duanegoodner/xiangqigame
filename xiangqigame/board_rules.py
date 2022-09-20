from dataclasses import dataclass, field
from typing import NamedTuple, Dict
from xiangqigame.enums import PieceColor


class BoardDim(NamedTuple):
    num_files: int
    num_ranks: int


class Castle(NamedTuple):
    min_rank: int
    max_rank: int
    min_file: int
    max_file: int


@dataclass
class BoardRules:
    forward_direction: Dict[PieceColor, int] = field(
        default_factory=lambda: {PieceColor.BLACK: 1, PieceColor.RED: -1})
    river_edges: Dict[PieceColor, int] = field(
        default_factory=lambda: {PieceColor.BLACK: 4, PieceColor.RED: 5})
    castles: Dict[PieceColor, Castle] = field(
        default_factory=lambda: {
            PieceColor.BLACK: Castle(
                min_rank=0, max_rank=2, min_file=3, max_file=5),
            PieceColor.RED: Castle(
                min_rank=7, max_rank=9, min_file=3, max_file=5)
        })
    opponent_of: Dict[PieceColor, PieceColor] = field(
        default_factory=lambda: {
            PieceColor.RED: PieceColor.BLACK,
            PieceColor.BLACK: PieceColor.RED
            }
    )
    num_ranks: int = 10
    num_files: int = 9

    # board_dim: BoardDim = BoardDim(num_files=9, num_ranks=10),


BOARD_RULES = BoardRules()
