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
class BasicRules:
    forward_direction: Dict[PieceColor, int] = field(
        default_factory=lambda: {PieceColor.RED: 1, PieceColor.BLACK: -1})
    river_edges: Dict[PieceColor, int] = field(
        default_factory=lambda: {PieceColor.RED: 4, PieceColor.BLACK: 5})
    castles: Dict[PieceColor, Castle] = field(
        default_factory=lambda: {
            PieceColor.RED: Castle(
                min_rank=0, max_rank=2, min_file=3, max_file=5),
            PieceColor.BLACK: Castle(
                min_rank=7, max_rank=9, min_file=3, max_file=5)
        }
    ),
    board_dim: BoardDim = BoardDim(num_files=9, num_ranks=10)


BOARD_RULES = BasicRules()










