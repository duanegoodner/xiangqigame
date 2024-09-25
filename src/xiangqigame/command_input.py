import argparse
from dataclasses import dataclass
from enum import Enum, auto
from typing import Callable, Any
from xiangqigame.enums import EvaluatorType, PlayerType
from xiangqigame.players import AIPlayer, HumanPlayer, Player
from xiangqigame_core import (
    MinimaxMoveEvaluator64,
    MinimaxMoveEvaluator128,
    RandomMoveEvaluator,
)

@dataclass
class PlayerInput:
    player_type: PlayerType
    algo: EvaluatorType
    strength: int
    key_size: int


class PlayerCommandInterpreter:
    _player_input_dispatch = {
        "ai": PlayerType.AI,
        "person": PlayerType.HUMAN,
        None: PlayerType.AI,
    }

    _minimax_key_size_dispatch = {
        64: MinimaxMoveEvaluator64,
        128: MinimaxMoveEvaluator128,
    }

    _default_key_size = 64

    _default_strength = 4

    def __init__(
        self,
        player_input: str,
        algo_input: str,
        strength_input: int,
        key_size_input: int,
    ):
        self.player_input = player_input
        self.algo_input = algo_input
        self.strength_input = strength_input
        self.key_size_input = key_size_input

    def _get_key_size(self) -> int:
        if self.key_size_input is None:
            return self._default_key_size
        else:
            return self.key_size_input

    def _get_strength(self) -> int:
        if self.strength_input is None:
            return self._default_strength
        else:
            return self.strength_input

    def interpret_command(self) -> PlayerInput:
        player_type = self._player_input_dispatch[self.player_input]
        if player_type == PlayerType.HUMAN:
            algo = strength = key_size = None
        else:
            if self.algo_input == "random":
                algo = EvaluatorType.RANDOM
                strength = key_size = None
            else:
                key_size = self._get_key_size()
                algo = EvaluatorType.MINIMAX
                strength = self._get_strength()
        return PlayerInput(
            player_type=player_type,
            algo=algo,
            strength=strength,
            key_size=key_size,
        )


@dataclass
class XiangqiGameCommand:
    red_player_input: PlayerInput
    black_player_input: PlayerInput
    save_summary: bool = False


@dataclass
class RunKwargsInterpreter:
    run_kwargs: dict[str, Any]

    def interpret_command(self) -> XiangqiGameCommand:
        red_interpreter = PlayerCommandInterpreter(
            player_input=self.run_kwargs["red_player_type"],
            algo_input=self.run_kwargs["red_algo"],
            strength_input=self.run_kwargs["red_strength"],
            key_size_input=self.run_kwargs["red_key_size"],
        )
        black_interpreter = PlayerCommandInterpreter(
            player_input=self.run_kwargs["black_player_type"],
            algo_input=self.run_kwargs["black_algo"],
            strength_input=self.run_kwargs["black_strength"],
            key_size_input=self.run_kwargs["black_key_size"],
        )

        return XiangqiGameCommand(
            red_player_input=red_interpreter.interpret_command(),
            black_player_input=black_interpreter.interpret_command(),
            save_summary=self.run_kwargs["save_summary"]
        )

class XiangqiGameCommandInterpreter:
    _player_type_dispatch = {
        "ai": PlayerType.AI,
        "person": PlayerType.HUMAN,
        None: PlayerType.AI,
    }

    _move_evaluator_dispatch = {
        "random": EvaluatorType.RANDOM,
        "minimax": EvaluatorType.MINIMAX,
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
            key_size=self._command_input.red_key_size,
        )

        black_player_input = PlayerInput(
            player_type=self._player_type_dispatch[
                self._command_input.black_player_type
            ],
            algo=self._move_evaluator_dispatch[self._command_input.black_algo],
            strength=self._command_input.black_strength,
            key_size=self._command_input.black_key_size,
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
            help="Search depth to user for red AI player with minimax algo"
                 "Default is 4.",
        )

        self._parser.add_argument(
            "-rk",
            "--red_key_size",
            type=int,
            choices=[64, 128],
            required=False,
            help="Key size (in bits) used for red AI player Zobrist hashing",
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
            help="Search depth to user for black AI player with minimax algo"
                 "Default is 4.",
        )

        self._parser.add_argument(
            "-bst",
            "--black_strength",
            type=int,
            choices=range(1, 10),
            required=False,
            help="Search depth to user for red player when red is 'ai' with "
            "'minimax.' Default is 4.",
        )

        self._parser.add_argument(
            "-bk",
            "--black_key_size",
            type=int,
            choices=[64, 128],
            required=False,
            help="Key size (in bits) used for black AI player Zobrist hashing",
        )
        
        self._parser.add_argument(
            "--save_summary", "-s", action="store_true", help="Save GameSummary as .json"
        )

    def get_args(self) -> dict[str, Any]:
        self._attach_args()
        args_namespace = self._parser.parse_args()
        return vars(args_namespace)

def main() -> XiangqiGameCommand:
    command_retriever = XiangqiGameCommandLine()
    my_command = command_retriever.get_args()
    return my_command


if __name__ == "__main__":
    result = main()
    print(result)
