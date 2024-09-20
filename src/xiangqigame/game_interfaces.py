import abc
from dataclasses import dataclass
from typing import Dict, List

import numpy as np
import pandas as pd
import xiangqigame_core as core

import xiangqigame.core_dataclass_mirrors as cdm
from xiangqigame.enums import GameState
from xiangqigame.player_summary import PlayerSummary


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
            color=self._color,
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
