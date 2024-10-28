"""
@file player_builder.py

Classes for building Player objects.
"""

from typing import Callable, Any, Dict, Tuple

import numpy as np
from xiangqi_bindings import (
    GameBoard,
    MinimaxMoveEvaluator32,
    MinimaxMoveEvaluator64,
    MinimaxMoveEvaluator128,
    PieceColor,
    RandomMoveEvaluator,
)

from xiangqipy.command_input import (
    PlayerInput,
    XiangqiGameCommand,
    PlayerType,
    EvaluatorType,
)
from xiangqipy.players import AIPlayer, HumanPlayer


class SinglePlayerBuilder:
    """
    Builds a Player object of specific color and GameBoard.
    """

    def __init__(
        self,
        player_input: PlayerInput,
        color: PieceColor,
        game_board: GameBoard,
    ):

        self.player_input = player_input
        self._color = color
        self._game_board = game_board

    @property
    def _move_evaluator_args(self) -> dict[Callable, Any]:
        dispatch_table = {
            RandomMoveEvaluator: {
                "evaluating_player": self._color,
                "game_board": self._game_board,
            },
            MinimaxMoveEvaluator32: {
                "evaluating_player": self._color,
                "starting_search_depth": self.player_input.strength,
                "game_board": self._game_board,
            },
            MinimaxMoveEvaluator64: {
                "evaluating_player": self._color,
                "starting_search_depth": self.player_input.strength,
                "game_board": self._game_board,
            },
            MinimaxMoveEvaluator128: {
                "evaluating_player": self._color,
                "starting_search_depth": self.player_input.strength,
                "game_board": self._game_board,
            },
        }

        if self.player_input.zkeys_seed is not None:
            for constructor in [
                MinimaxMoveEvaluator32,
                MinimaxMoveEvaluator64,
                MinimaxMoveEvaluator128,
            ]:
                dispatch_table[constructor][
                    "zkeys_seed"
                ] = np.uint32(self.player_input.zkeys_seed)
        return dispatch_table

    @property
    def evaluator_constructor_dispatch(self) -> Dict[Tuple, Callable]:
        return {
            (EvaluatorType.RANDOM, None): RandomMoveEvaluator,
            (EvaluatorType.MINIMAX, 32): MinimaxMoveEvaluator32,
            (EvaluatorType.MINIMAX, 64): MinimaxMoveEvaluator64,
            (EvaluatorType.MINIMAX, 128): MinimaxMoveEvaluator128,
        }

    def _build_human_player(self):
        return HumanPlayer(
            color=self._color, player_type=self.player_input.player_type
        )
        # return self.player_input.player_type(color=self._color)

    def _build_ai_player(self):
        player_constructor = self.evaluator_constructor_dispatch[
            (self.player_input.algo, self.player_input.key_size)
        ]

        constructor_kwargs = self._move_evaluator_args[player_constructor]
        move_evaluator = player_constructor(**constructor_kwargs)

        return AIPlayer(
            color=self._color,
            player_type=self.player_input.player_type,
            evaluator_type=self.player_input.algo,
            move_evaluator=move_evaluator,
        )

    @property
    def _player_dispatch(self) -> dict[PlayerType, Any]:
        return {
            PlayerType.HUMAN: self._build_human_player,
            PlayerType.AI: self._build_ai_player,
        }

    def build(self):
        return self._player_dispatch[self.player_input.player_type]()


class RedAndBlackPlayersBuilder:
    """
    Builds two Player objects for a GameBoard based on XiangqiGameCommand.
    """

    def __init__(
        self, xiangqi_command: XiangqiGameCommand, game_board: GameBoard
    ):
        self._command = xiangqi_command
        self._board = game_board

    def build(self):
        red_player = SinglePlayerBuilder(
            color=PieceColor.kRed,
            player_input=self._command.red_player_input,
            game_board=self._board,
        ).build()

        black_player = SinglePlayerBuilder(
            color=PieceColor.kBlk,
            player_input=self._command.black_player_input,
            game_board=self._board,
        ).build()

        return {PieceColor.kRed: red_player, PieceColor.kBlk: black_player}
