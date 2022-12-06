import argparse
from cpp_modules.game_board_py import PieceColor
from xiangqigame.move_selectors import MinimaxMoveSelector, RandomMoveSelector
from xiangqigame.players import HumanPlayer, AIPlayer
from xiangqigame.minimax_evaluators import DEFAULT_MINIMAX_EVALUATOR


class SinglePlayerBuilder:

    _player_type_dispatch = {
        "ai": AIPlayer,
        "person": HumanPlayer
    }

    _move_selector_dispatch = {
        "random": RandomMoveSelector,
        "minimax": MinimaxMoveSelector
    }

    def __init__(
            self,
            color: PieceColor,
            player_type: str,
            algo_type: str,
            strength: int):
        self._color = color
        self._player_constructor = self._player_type_dispatch[player_type]
        if algo_type:
            self._move_selector_constructor = self._move_selector_dispatch[algo_type]
        else:
            self._move_selector_constructor = None
        self._strength = strength

    @property
    def move_selector_args(self):
        return {
            RandomMoveSelector: {},
            MinimaxMoveSelector: {
                "evaluator": DEFAULT_MINIMAX_EVALUATOR,
                "search_depth": self._strength
            }
        }

    def build(self):
        if self._player_constructor == HumanPlayer:
            return HumanPlayer()
        else:
            constructor_kwargs = self.move_selector_args[self._move_selector_constructor]
            move_selector = self._move_selector_constructor(
                **constructor_kwargs)
            return AIPlayer(color=self._color, move_selector=move_selector)


class RedAndBlackPlayersBuilder:

    def __init__(self, command_args: argparse.Namespace):
        self._command_args = command_args

    def build(self):
        red_player = SinglePlayerBuilder(
            color=PieceColor.kRed,
            player_type=self._command_args.red_player_type,
            algo_type=self._command_args.red_algo,
            strength=self._command_args.red_strength).build()

        black_player = SinglePlayerBuilder(
            color=PieceColor.kBlk,
            player_type=self._command_args.black_player_type,
            algo_type=self._command_args.black_algo,
            strength=self._command_args.black_strength).build()

        return {PieceColor.kRed: red_player, PieceColor.kBlk: black_player}
