"""
@file core_dataclass_mirrors.py

Contains classes that mirror the structure of some core C++ classes,
primarily to facilitate easy IO of a GameSummary with msgspec.
"""

import datetime
from enum import Enum
from dataclasses import dataclass
from typing import Dict, List, TypeAlias

import numpy as np
import xiangqi_bindings as bindings


class PointsTypeDeterminer:
    """
    Determines the integer type used for Points int the C++ core, and provides
    a numpy integer type with the same size and signed-ness.
    """

    _dtype_map = {
        (True, 1): np.int8,
        (True, 2): np.int16,
        (True, 4): np.int32,
        (True, 8): np.int64,
        (False, 1): np.uint8,
        (False, 2): np.uint16,
        (False, 4): np.uint32,
        (False, 8): np.uint64,
    }

    def get_points_type(self) -> type:
        dtype_key = (
            bindings.is_signed_points_type(),
            bindings.size_of_points_type(),
        )
        return self._dtype_map[dtype_key]


PointsT: TypeAlias = PointsTypeDeterminer().get_points_type()


class MinimaxResultTypePy(Enum):
    """
    Enum indicating the type of result obtained from Minimax analysis
    of a single node.
    """

    Unknown = int(bindings.MinimaxResultType.Unknown)
    TrTableHit = int(bindings.MinimaxResultType.TrTableHit)
    EvaluatorLoses = int(bindings.MinimaxResultType.EvaluatorLoses)
    EvaluatorWins = int(bindings.MinimaxResultType.EvaluatorWins)
    FullyEvaluatedNode = int(bindings.MinimaxResultType.FullyEvaluatedNode)
    StandardLeaf = int(bindings.MinimaxResultType.StandardLeaf)
    AlphaPrune = int(bindings.MinimaxResultType.AlphaPrune)
    BetaPrune = int(bindings.MinimaxResultType.BetaPrune)


@dataclass
class GamePiece:
    """
    A Python GamePiece.
    """

    piece_color: bindings.PieceColor
    piece_type: bindings.PieceType

    @classmethod
    def from_core_game_piece(cls, core_game_piece: bindings.GamePiece):
        return cls(
            piece_color=core_game_piece.piece_color,
            piece_type=core_game_piece.piece_type,
        )


@dataclass
class BoardSpace:
    """
    A Python BoardSpace.
    """

    rank: int
    file: int

    @classmethod
    def from_core_board_space(cls, core_board_space: bindings.BoardSpace):
        return cls(rank=core_board_space.rank, file=core_board_space.file)


@dataclass
class Move:
    """
    A Python Move.
    """

    start: BoardSpace
    end: BoardSpace

    @classmethod
    def from_core_move(cls, core_move: bindings.Move):
        start = BoardSpace.from_core_board_space(core_move.start)
        end = BoardSpace.from_core_board_space(core_move.end)
        return cls(start=start, end=end)


@dataclass
class MoveCollection:
    """
    A Python MoveCollection.
    """

    moves: List[Move]

    @classmethod
    def from_core_move_collection(
        cls, core_move_collection: bindings.MoveCollection
    ):
        moves = [
            Move.from_core_move(core_move=core_move)
            for core_move in core_move_collection.moves
        ]
        return cls(moves=moves)

    @property
    def size(self) -> int:
        return len(self.moves)


@dataclass
class EqualScoreMoves:
    """
    A Ptyhon EqualScoreMoves.
    """

    shared_score: PointsT
    similar_moves: MoveCollection

    @classmethod
    def from_core_similar_moves(
        cls, core_similar_moves: bindings.EqualScoreMoves
    ):
        return cls(
            shared_score=core_similar_moves.shared_score,
            similar_moves=MoveCollection.from_core_move_collection(
                core_move_collection=core_similar_moves.similar_moves
            ),
        )


@dataclass
class TranspositionTableSize:
    num_entries: int | None
    num_states: int | None

    @classmethod
    def from_core_transposition_table_size(cls, core_transposition_table_size):
        return cls(
            num_entries=core_transposition_table_size.num_entries,
            num_states=core_transposition_table_size.num_states,
        )


@dataclass
class TranspositionTableSizesAtEvents:
    first_collision: TranspositionTableSize | None
    end_game: TranspositionTableSize | None


@dataclass
class ExecutedMove:
    """
    A Python ExecutedMove
    """

    moving_piece: GamePiece
    destination_piece: GamePiece
    spaces: Move

    @classmethod
    def from_core_executed_move(
        cls, core_executed_move: bindings.ExecutedMove
    ):
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
    """
    A Python SearchSummary.
    """

    num_nodes: int
    time: datetime.timedelta
    result_depth_counts: np.ndarray
    transposition_table_hits: np.ndarray
    similar_moves: EqualScoreMoves
    selected_move: Move
    returned_illegal_move: bool
    num_collisions: int
    tr_table_size_initial: TranspositionTableSize
    tr_table_size_final: TranspositionTableSize

    @classmethod
    def from_core_search_summary(
        cls, core_search_summary: bindings.SearchSummary
    ):
        return cls(
            num_nodes=core_search_summary.num_nodes,
            time=core_search_summary.time,
            result_depth_counts=np.array(
                core_search_summary.get_result_depth_counts()
            ),
            transposition_table_hits=np.array(
                core_search_summary.get_transposition_table_hits()
            ),
            similar_moves=EqualScoreMoves.from_core_similar_moves(
                core_similar_moves=core_search_summary.similar_moves
            ),
            selected_move=Move.from_core_move(
                core_move=core_search_summary.selected_move
            ),
            returned_illegal_move=core_search_summary.returned_illegal_move,
            num_collisions=core_search_summary.num_collisions,
            tr_table_size_initial=TranspositionTableSize.from_core_transposition_table_size(
                core_transposition_table_size=core_search_summary.tr_table_size_initial
            ),
            tr_table_size_final=TranspositionTableSize.from_core_transposition_table_size(
                core_transposition_table_size=core_search_summary.tr_table_size_final
            ),
        )

    @property
    def mean_time_per_node_ns(self) -> float:
        return 1000000 * self.time.total_seconds() / self.num_nodes


@dataclass
class SearchSummaries:
    """
    A Python SearchSummaries.
    """

    first_searches: List[SearchSummary]
    extra_searches: Dict[int, SearchSummary]

    @classmethod
    def from_core_search_summaries(
        cls, core_search_summaries: bindings.SearchSummaries
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
