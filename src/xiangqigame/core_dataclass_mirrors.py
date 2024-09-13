import datetime
import numpy as np
import pandas as pd

from dataclasses import dataclass
from typing import Dict, List
from xiangqigame_core import (
    BoardSpace,
    GamePiece,
    ExecutedMove,
    Move,
    PieceColor,
    PieceType,
    SearchSummary,
    SearchSummaries,
)


@dataclass
class GamePiece:
    piece_color: PieceColor
    piece_type: PieceType

    @classmethod
    def from_core_game_piece(cls, core_game_piece: GamePiece):
        return cls(
            piece_color=core_game_piece.piece_color,
            piece_type=core_game_piece.piece_type,
        )


@dataclass
class BoardSpace:
    rank: int
    file: int

    @classmethod
    def from_core_board_space(cls, core_board_space: BoardSpace):
        return cls(rank=core_board_space.rank, file=core_board_space.file)


@dataclass
class Move:
    start: BoardSpace
    end: BoardSpace

    @classmethod
    def from_core_move(cls, core_move: Move):
        start = BoardSpace.from_core_board_space(core_move.start)
        end = BoardSpace.from_core_board_space(core_move.end)
        return cls(start=start, end=end)


@dataclass
class ExecutedMove:
    moving_piece: GamePiece
    destination_piece: GamePiece
    spaces: Move

    @classmethod
    def from_core_executed_move(cls, core_executed_move: ExecutedMove):
        return cls(
            moving_piece=GamePiece.from_core_game_piece(
                core_game_piece=core_executed_move.moving_piece
            ),
            destination_piece=GamePiece.from_core_game_piece(
                core_game_piece=core_executed_move.destination_piece
            ),
            spaces=Move.from_core_move(core_move=core_executed_move.spaces),
        )


@dataclass
class SearchSummary:
    num_nodes: int
    time: datetime.timedelta
    result_depth_counts: List[List[int]]

    @classmethod
    def from_core_search_summary(cls, core_search_summary: SearchSummary):
        return cls(
            num_nodes=core_search_summary.num_nodes,
            time=core_search_summary.time,
            result_depth_counts=core_search_summary.result_depth_counts,
        )
    
    @property
    def flattened_rd_counts(self) -> pd.Series:
        flattened_array = np.array(self.result_depth_counts).ravel()
        return pd.Series(flattened_array)


@dataclass
class SearchSummaries:
    first_searches: List[SearchSummary]
    extra_searches: Dict[int, SearchSummary]

    @classmethod
    def from_core_search_summaries(cls, core_search_summaries: SearchSummaries):
        return cls(
            first_searches=[
                SearchSummary.from_core_search_summary(item)
                for item in core_search_summaries.first_searches
            ],
            extra_searches={
                key: SearchSummary.from_core_search_summary(val)
                for key, val in core_search_summaries.extra_searches.items()
            },
        )

