import datetime
from dataclasses import dataclass
from typing import Dict, List

import numpy as np
import pandas as pd
import xiangqigame_core as core


@dataclass
class GamePiece:
    piece_color: core.PieceColor
    piece_type: core.PieceType

    @classmethod
    def from_core_game_piece(cls, core_game_piece: core.GamePiece):
        return cls(
            piece_color=core_game_piece.piece_color,
            piece_type=core_game_piece.piece_type,
        )


@dataclass
class BoardSpace:
    rank: int
    file: int

    @classmethod
    def from_core_board_space(cls, core_board_space: core.BoardSpace):
        return cls(rank=core_board_space.rank, file=core_board_space.file)


@dataclass
class Move:
    start: BoardSpace
    end: BoardSpace

    @classmethod
    def from_core_move(cls, core_move: core.Move):
        start = BoardSpace.from_core_board_space(core_move.start)
        end = BoardSpace.from_core_board_space(core_move.end)
        return cls(start=start, end=end)


@dataclass
class ExecutedMove:
    moving_piece: GamePiece
    destination_piece: GamePiece
    spaces: Move

    @classmethod
    def from_core_executed_move(cls, core_executed_move: core.ExecutedMove):
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
    def from_core_search_summary(cls, core_search_summary: core.SearchSummary):
        return cls(
            num_nodes=core_search_summary.num_nodes,
            time=core_search_summary.time,
            result_depth_counts=core_search_summary.result_depth_counts,
        )

    @property
    def mean_time_per_node_ns(self) -> float:
        return 1000000 * self.time.total_seconds() / self.num_nodes


@dataclass
class SearchSummaries:
    first_searches: List[SearchSummary]
    extra_searches: Dict[int, SearchSummary]

    @classmethod
    def from_core_search_summaries(
        cls, core_search_summaries: core.SearchSummaries
    ):
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

    @property
    def first_searches_by_type_and_depth(
        self,
    ) -> Dict[core.MinimaxResultType, List[List[int]]]:
        result = {}

        for enum_val in core.MinimaxResultType.__members__.values():
            result[enum_val] = [
                search_summary.result_depth_counts[int(enum_val)]
                for search_summary in self.first_searches
            ]

        return result

    @property
    def first_searches_by_type(
        self,
    ) -> Dict[core.MinimaxResultType, List[int]]:
        result = {}
        for enum_val in core.MinimaxResultType.__members__.values():
            result[enum_val] = [
                sum(search_summary.result_depth_counts[int(enum_val)])
                for search_summary in self.first_searches
            ]
        return result

    @property
    def first_searches_mean_time_per_node_ns(self) -> List[float]:
        return [
            search_summary.mean_time_per_node_ns
            for search_summary in self.first_searches
        ]

    @property
    def first_searches_total_nodes(self) -> List[int]:
        return [
            search_summary.num_nodes for search_summary in self.first_searches
        ]

    @property
    def first_searches_time_s(self) -> List[float]:
        return [search_summary.time.total_seconds() for search_summary in self.first_searches]
