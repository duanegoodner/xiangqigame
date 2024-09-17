import abc
import numpy as np
from dataclasses import dataclass
import xiangqigame_core as core
import xiangqigame.core_dataclass_mirrors as cdm
from typing import List, Dict
from xiangqigame.enums import GameState


@dataclass
class PlayerSummary:
    player_type: str
    move_evaluator_type: str = None
    max_search_depth: int = None
    zobrist_key_size: int = None
    search_summaries: cdm.SearchSummaries = None

    @property
    def first_searches(self) -> List[cdm.SearchSummary]:
        if self.search_summaries is not None:
            return self.search_summaries.first_searches

    @property
    def extra_searches(self) -> Dict[int, cdm.SearchSummary]:
        if self.search_summaries is not None:
            return self.search_summaries.extra_searches

    @property
    def first_searches_by_type_and_depth(
        self,
    ) -> Dict[core.MinimaxResultType, List[List[int]]]:
        if self.search_summaries is not None:
            return self.search_summaries.first_searches_by_type_and_depth

    @property
    def first_searches_by_type(self) -> Dict[core.MinimaxResultType, List[int]]:
        if self.search_summaries is not None:
            return self.search_summaries.first_searches_by_type

    @property
    def first_searches_mean_time_per_node_ns(self) -> List[float]:
        if self.search_summaries is not None:
            return self.search_summaries.first_searches_mean_time_per_node_ns

    @property
    def first_searches_total_nodes(self) -> List[int]:
        if self.search_summaries is not None:
            return self.search_summaries.first_searches_total_nodes

    @property
    def first_searches_time_s(self) -> List[float]:
        if self.search_summaries is not None:
            return self.search_summaries.first_searches_time_s

    @property
    def first_searches_eval_scores(self) -> np.array:
        if self.search_summaries is not None:
            return self.search_summaries.first_searches_eval_scores


@dataclass
class PlayerBasicInfo:
    player_type: str
    move_evaluator_type: str = None
    max_search_depth: int = None
    zobrist_key_size: int = None

    @classmethod
    def from_player_summary(cls, player_summary: PlayerSummary):
        return cls(
            player_type=player_summary.player_type,
            move_evaluator_type=player_summary.move_evaluator_type,
            max_search_depth=player_summary.max_search_depth,
            zobrist_key_size=player_summary.zobrist_key_size,
        )


class Player(abc.ABC):

    def __init__(self, color: core.PieceColor):
        self._color = color

    @abc.abstractmethod
    def propose_move(
        self, game_board: core.GameBoard, cur_moves: List[core.Move]
    ) -> core.Move:
        pass

    @abc.abstractmethod
    def illegal_move_notice_response(
        self,
        illegal_move: core.Move,
        game_board: core.GameBoard,
        cur_moves: List[core.Move],
    ):
        pass

    @property
    def player_type(self) -> str:
        return type(self).__name__

    @property
    def move_evaluator_type(self) -> str | None:
        if hasattr(self, "_move_evaluator"):
            return type(self._move_evaluator).__name__

    @property
    def max_search_depth(self) -> int | None:
        if self.move_evaluator_type in [
            "MinimaxMoveEvaluator64",
            "MinimaxMoveEvaluator128",
        ]:
            return self._move_evaluator.starting_search_depth()

    @property
    def zobrist_key_size(self) -> int | None:
        if self.move_evaluator_type in [
            "MinimaxMoveEvaluator64",
            "MinimaxMoveEvaluator128",
        ]:
            return self._move_evaluator.zobrist_key_size_bits()

    @property
    def search_summaries(self) -> cdm.SearchSummaries | None:
        if self.move_evaluator_type in [
            "MinimaxMoveEvaluator64",
            "MinimaxMoveEvaluator128",
        ]:
            return cdm.SearchSummaries.from_core_search_summaries(
                core_search_summaries=self._move_evaluator.get_search_summaries()
            )

    @property
    def summary(self) -> PlayerSummary:
        return PlayerSummary(
            player_type=self.player_type,
            move_evaluator_type=self.move_evaluator_type,
            max_search_depth=self.max_search_depth,
            zobrist_key_size=self.zobrist_key_size,
            search_summaries=self.search_summaries,
        )


class GameStatusReporter(abc.ABC):

    @abc.abstractmethod
    def report_game_info(
        self,
        game_state: GameState,
        game_board: core.GameBoard,
        whose_turn: core.PieceColor,
        is_in_check: bool,
        move_count: int,
        prev_move: core.Move = None,
    ):
        pass
