import abc
from dataclasses import dataclass
from typing import Dict, List

import numpy as np
import pandas as pd
import xiangqigame_core as core

import xiangqigame.core_dataclass_mirrors as cdm
from xiangqigame.enums import GameState, PlayerType, EvaluatorType
from xiangqigame.player_summary import PlayerSummary


class Player(abc.ABC):

    def __init__(
        self,
        color: core.PieceColor,
        player_type: PlayerType,
        evaluator_type: EvaluatorType = EvaluatorType.NULL,
    ) -> None:
        self._color = color
        self._player_type = player_type
        self._evaluator_type = evaluator_type

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
    def player_type(self) -> PlayerType:
        return self._player_type

    @property
    def move_evaluator_type(self) -> EvaluatorType:
        return self._evaluator_type

    @property
    def max_search_depth(self) -> int | None:
        if self._evaluator_type == EvaluatorType.MINIMAX:
            return self._move_evaluator.starting_search_depth()

    @property
    def zobrist_key_size(self) -> int | None:
        if self._evaluator_type == EvaluatorType.MINIMAX:
            return self._move_evaluator.zobrist_key_size_bits()

    @property
    def search_summaries(self) -> cdm.SearchSummaries | None:
        if self._evaluator_type == EvaluatorType.MINIMAX:
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
        red_player_summary: PlayerSummary,
        black_player_summary: PlayerSummary,
        game_state: GameState,
        game_board: core.GameBoard,
        whose_turn: core.PieceColor,
        is_in_check: bool,
        move_count: int,
        prev_move: core.Move = None,
    ):
        pass
