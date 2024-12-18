"""
Python abstract classes used by a Game.
"""

import abc
from dataclasses import dataclass
from typing import Dict, List

import numpy as np
import pandas as pd
import xiangqi_bindings as bindings

import xiangqipy.core_dataclass_mirrors as cdm
from xiangqipy.enums import GameState, PlayerType, EvaluatorType
from xiangqipy.player_summary import PlayerSummary


class Player(abc.ABC):
    """
    Can take a turn in a Game.
    """

    def __init__(
        self,
        color: bindings.PieceColor,
        player_type: PlayerType,
        evaluator_type: EvaluatorType = EvaluatorType.NULL,
    ) -> None:
        self._color = color
        self._player_type = player_type
        self._evaluator_type = evaluator_type

    @abc.abstractmethod
    def propose_move(
        self, game_board: bindings.GameBoard, cur_moves: List[bindings.Move]
    ) -> bindings.Move:
        pass

    @abc.abstractmethod
    def illegal_move_notice_response(
        self,
        illegal_move: bindings.Move,
        game_board: bindings.GameBoard,
        cur_moves: List[bindings.Move],
    ):
        pass

    @property
    def player_type(self) -> PlayerType:
        return self._player_type

    @property
    def move_evaluator_type(self) -> EvaluatorType:
        return self._evaluator_type

    @property
    def max_search_depth(self) -> int | None:
        if self._evaluator_type == EvaluatorType.MINIMAX:
            return self._move_evaluator.search_depth()

    @property
    def zkeys_seed(self) -> int | None:
        if self._evaluator_type == EvaluatorType.MINIMAX:
            return self._move_evaluator.zkeys_seed

    @property
    def zobrist_key_size(self) -> int | None:
        if self._evaluator_type == EvaluatorType.MINIMAX:
            return self._move_evaluator.zobrist_key_size_bits()

    @property
    def search_summaries(self) -> cdm.SearchSummaries | None:
        if self._evaluator_type == EvaluatorType.MINIMAX:
            return cdm.SearchSummaries.from_core_search_summaries(
                core_search_summaries=self._move_evaluator.search_summaries
            )

    @property
    def summary(self) -> PlayerSummary:
        return PlayerSummary(
            color=self._color,
            player_type=self.player_type,
            move_evaluator_type=self.move_evaluator_type,
            max_search_depth=self.max_search_depth,
            zobrist_key_size=self.zobrist_key_size,
            zkeys_seed=self.zkeys_seed,
            search_summaries=self.search_summaries,
        )


class MoveReporter(abc.ABC):
    """
    Reports details of a Game.
    """

    @abc.abstractmethod
    def report_game_info(
        self,
        red_player_summary: PlayerSummary,
        black_player_summary: PlayerSummary,
        game_state: GameState,
        game_board: bindings.GameBoard,
        whose_turn: bindings.PieceColor,
        is_in_check: bool,
        move_count: int,
        prev_move: bindings.Move = None,
    ):
        pass
