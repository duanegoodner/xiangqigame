from typing import Callable, Any

from xiangqigame_core import (
    GameBoard,
    MinimaxMoveEvaluator64,
    MinimaxMoveEvaluator128,
    PieceColor,
    RandomMoveEvaluator,
)

from xiangqigame.command_input import (
    PlayerInput,
    XiangqiGameCommand,
)
from xiangqigame.players import AIPlayer, HumanPlayer


class SinglePlayerBuilder:

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
        return {
            RandomMoveEvaluator: {
                "evaluating_player": self._color,
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
            }
        }

    def _build_human_player(self):
        return self.player_input.player_type()

    def _build_ai_player(self):
        constructor_kwargs = self._move_evaluator_args[
            self.player_input.algo
        ]
        move_evaluator = self.player_input.algo(**constructor_kwargs)

        return AIPlayer(color=self._color, move_evaluator=move_evaluator)

    @property
    def _player_dispatch(self) -> dict[Callable, Any]:
        return {
            HumanPlayer: self._build_human_player,
            AIPlayer: self._build_ai_player,
        }

    def build(self):
        return self._player_dispatch[self.player_input.player_type]()


class RedAndBlackPlayersBuilder:

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
