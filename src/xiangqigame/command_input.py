import argparse
from dataclasses import dataclass
from typing import Callable
from xiangqigame.players import AIPlayer, HumanPlayer, Player
from xiangqigame_core import MinimaxMoveEvaluator64, MinimaxMoveEvaluator128, RandomMoveEvaluator


@dataclass
class PlayerInput:
    player_type: Callable[..., Player]
    algo: Callable[..., RandomMoveEvaluator | MinimaxMoveEvaluator64 | MinimaxMoveEvaluator128]
    strength: int


@dataclass
class XiangqiGameCommand:
    red_player_input: PlayerInput
    black_player_input: PlayerInput


class XiangqiGameCommandInterpreter:
    _player_type_dispatch = {"ai": AIPlayer, "person": HumanPlayer, None: None}

    _move_evaluator_dispatch = {
        "random": RandomMoveEvaluator,
        "minimax": MinimaxMoveEvaluator64,
        None: None,
    }

    def __init__(self, command_input: argparse.Namespace):
        self._command_input = command_input

    def interpret_command(self) -> XiangqiGameCommand:
        red_player_input = PlayerInput(
            player_type=self._player_type_dispatch[
                self._command_input.red_player_type
            ],
            algo=self._move_evaluator_dispatch[self._command_input.red_algo],
            strength=self._command_input.red_strength,
        )

        black_player_input = PlayerInput(
            player_type=self._player_type_dispatch[
                self._command_input.black_player_type
            ],
            algo=self._move_evaluator_dispatch[self._command_input.black_algo],
            strength=self._command_input.black_strength,
        )

        return XiangqiGameCommand(
            red_player_input=red_player_input,
            black_player_input=black_player_input,
        )


class XiangqiGameCommandLine:

    def __init__(self):
        self._parser = argparse.ArgumentParser(
            prog="play_xiangqi",
            description="A Xiangqi (a.k.a. Chinese Chess) game that can be "
            "played as Human vs. Human, AI vs. AI, or Human vs. AI",
            epilog="Note: If all default parameters are used, the red player "
            "will be an AI using a Minimax algorithm with search depth "
            "= 1, and the black player will be an AI using Minimax with "
            "search depth = 2.",
        )

    def _attach_args(self):
        self._parser.add_argument(
            "-rt",
            "--red_player_type",
            choices=["person", "ai"],
            help="Can be 'person', or 'ai'. Default is 'ai'.",
        )

        self._parser.add_argument(
            "-ra",
            "--red_algo",
            choices=["random", "minimax"],
            required=False,
            help="Search algorithm to use for red player (if player type is "
            "'ai'). Can be 'random' or 'minimax'. Default is minimax.",
        )

        self._parser.add_argument(
            "-rst",
            "--red_strength",
            type=int,
            choices=range(1, 10),
            required=False,
            help="Search depth to user for red player when red is 'ai' with "
            "'minimax.' Default is 1.",
        )

        self._parser.add_argument(
            "-bt",
            "--black_player_type",
            choices=["person", "ai"],
            help="Can be 'person', or 'ai'. Default is 'ai'.",
        )

        self._parser.add_argument(
            "-ba",
            "--black_algo",
            choices=["random", "minimax"],
            required=False,
            help="Search algorithm to use for black player (if player type is "
            "'ai'). Can be 'random' or 'minimax'. Default is minimax.",
        )

        self._parser.add_argument(
            "-bst",
            "--black_strength",
            type=int,
            choices=range(1, 10),
            required=False,
            help="Search depth to user for red player when red is 'ai' with "
            "'minimax.' Default is 2.",
        )

    def get_args(self, *args) -> XiangqiGameCommand:
        self._attach_args()
        args_namespace = self._parser.parse_args(*args)
        interpreted_command = XiangqiGameCommandInterpreter(
            args_namespace
        ).interpret_command()

        return interpreted_command


def main() ->XiangqiGameCommand:
    command_retriever = XiangqiGameCommandLine()
    my_command = command_retriever.get_args()
    return my_command


if __name__ == "__main__":
    result = main()
    print(result)
