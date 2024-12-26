"""
Contains classes for collecting command line input, and converting to a form
that will be convenient for instantiating all of the objects needed to play a
Game.
"""

import argparse
from dataclasses import dataclass
from typing import Any, Literal

import xiangqi_bindings as xb

from xiangqipy.enums import EvaluatorType, PlayerType


@dataclass
class RawPlayerInput:
    color: xb.PieceColor
    player_type: Literal[None, "ai", "person"]
    algo: Literal[None, "minimax", "random"]
    strength: int = None
    key_size: int = None
    number_zobrist_states: int = None
    zkeys_seed: int = None

    @property
    def evaluator_type(self) -> xb.EvaluatorType:
        if self.player_type in [None, "ai"]:
            if self.algo in ["minimax", None]:
                return xb.EvaluatorType.kMinimax
            if self.algo == "random":
                return xb.EvaluatorType.kRandom
        else:
            return xb.EvaluatorType.kHuman

    @property
    def minimax_search_depth(self) -> int:
        if self.evaluator_type in [
            xb.EvaluatorType.kRandom,
            xb.EvaluatorType.kHuman,
        ]:
            return 0
        elif self.evaluator_type == xb.EvaluatorType.kMinimax:
            if self.strength is None:
                return 4
            else:
                return self.strength

    @property
    def zobrist_key_size_bits(self) -> int:
        if self.evaluator_type in [
            xb.EvaluatorType.kRandom,
            xb.EvaluatorType.kHuman,
        ]:
            return 0
        elif self.evaluator_type == xb.EvaluatorType.kMinimax:
            return self.key_size if self.key_size else 64

    @property
    def zobrist_calculator_count(self) -> int:
        if self.evaluator_type in [
            xb.EvaluatorType.kRandom,
            xb.EvaluatorType.kHuman,
        ]:
            return 0
        elif self.evaluator_type == xb.EvaluatorType.kMinimax:
            return (
                self.number_zobrist_states if self.number_zobrist_states else 2
            )

    def to_player_spec(self) -> xb.PlayerSpec:
        return xb.PlayerSpec(
            color=self.color,
            evaluator_type=self.evaluator_type,
            zobrist_key_size_bits=self.zobrist_key_size_bits,
            zobrist_calculator_count=self.zobrist_calculator_count,
            minimax_search_depth=self.minimax_search_depth,
        )

def build_player_spec(
    run_kwargs: dict[str, Any], color_enum: xb.PieceColor, color_prefix: str
) -> xb.PlayerSpec:
    player_info = {"color": color_enum} | {
        key.removeprefix(color_prefix): val
        for key, val in run_kwargs.items()
        if key.startswith(color_prefix)
    }
    return RawPlayerInput(**player_info).to_player_spec()

def build_game_runner(run_kwargs: dict[str, Any]) -> xb.GameRunner:
    red_player_spec = build_player_spec(
        run_kwargs=run_kwargs,
        color_enum=xb.PieceColor.kRed,
        color_prefix="red_",
    )
    black_player_spec = build_player_spec(
        run_kwargs=run_kwargs,
        color_enum=xb.PieceColor.kBlk,
        color_prefix="black_",
    )

    game_runner = xb.GameRunner(
        red_player_spec=red_player_spec, black_player_spec=black_player_spec
    )

    return game_runner


@dataclass
class PlayerInput:
    """
    Container for info collected from command line for specific player.
    """

    player_type: PlayerType
    algo: EvaluatorType
    strength: int
    key_size: int
    num_zobrist_states: int
    zkeys_seed: int | None = None


class PlayerCommandInterpreter:
    """
    Converts command line input related to a player into PlayerInput object.
    """

    _player_input_dispatch = {
        "ai": PlayerType.AI,
        "person": PlayerType.HUMAN,
        None: PlayerType.AI,
    }

    _minimax_key_size_dispatch = {
        32: xb.MinimaxMoveEvaluator32,
        64: xb.MinimaxMoveEvaluator64,
        128: xb.MinimaxMoveEvaluator128,
    }

    _default_key_size = 64

    _default_strength = 4

    _default_number_zobrist_states = 1

    def __init__(
        self,
        player_input: str,
        algo_input: str,
        strength_input: int,
        key_size_input: int,
        zkeys_seed: int,
        number_zobrist_states: int,
    ):
        self.player_input = player_input
        self.algo_input = algo_input
        self.strength_input = strength_input
        self.key_size_input = key_size_input
        self.zkeys_seed = zkeys_seed
        self.number_zobrist_states = number_zobrist_states

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

    def _get_number_zobrist_states(self) -> int:
        if self.number_zobrist_states is None:
            return self._default_number_zobrist_states
        else:
            return self.number_zobrist_states

    def interpret_command(self) -> PlayerInput:
        player_type = self._player_input_dispatch[self.player_input]
        if player_type == PlayerType.HUMAN:
            algo = strength = key_size = num_zobrist_states = None
        else:
            if self.algo_input == "random":
                algo = EvaluatorType.RANDOM
                strength = key_size = num_zobrist_states = None
            else:
                key_size = self._get_key_size()
                algo = EvaluatorType.MINIMAX
                strength = self._get_strength()
                num_zobrist_states = self._get_number_zobrist_states()
        return PlayerInput(
            player_type=player_type,
            algo=algo,
            strength=strength,
            key_size=key_size,
            zkeys_seed=self.zkeys_seed,
            num_zobrist_states=num_zobrist_states,
        )


@dataclass
class XiangqiGameCommand:
    """
    Data container with all the info needed to instantiate a Game.
    """

    red_player_input: PlayerInput
    black_player_input: PlayerInput
    save_summary: bool = False
    output_dir_suffix: str = ""


@dataclass
class RunKwargsInterpreter:
    """
    Converts dictionary output by XiangqiGameCommandLine into a
    XiangqiGameCommand.
    """

    run_kwargs: dict[str, Any]

    def interpret_command(self) -> XiangqiGameCommand:
        red_interpreter = PlayerCommandInterpreter(
            player_input=self.run_kwargs["red_player_type"],
            algo_input=self.run_kwargs["red_algo"],
            strength_input=self.run_kwargs["red_strength"],
            key_size_input=self.run_kwargs["red_key_size"],
            zkeys_seed=self.run_kwargs["red_zkeys_seed"],
            number_zobrist_states=self.run_kwargs["red_number_zobrist_states"],
        )
        black_interpreter = PlayerCommandInterpreter(
            player_input=self.run_kwargs["black_player_type"],
            algo_input=self.run_kwargs["black_algo"],
            strength_input=self.run_kwargs["black_strength"],
            key_size_input=self.run_kwargs["black_key_size"],
            zkeys_seed=self.run_kwargs["black_zkeys_seed"],
            number_zobrist_states=self.run_kwargs[
                "black_number_zobrist_states"
            ],
        )

        return XiangqiGameCommand(
            red_player_input=red_interpreter.interpret_command(),
            black_player_input=black_interpreter.interpret_command(),
            save_summary=self.run_kwargs["save_summary"],
            output_dir_suffix=self.run_kwargs["output_dir_suffix"],
        )


class XiangqiGameCommandLine:
    """
    Collects info from command line args out outputs as a dictionary.

    Includes default values for any items not specified in command line args.
    """

    def __init__(self):
        self._parser = argparse.ArgumentParser(
            prog="play_xiangqi",
            description="A Xiangqi (a.k.a. Chinese Chess) game that can be "
            "played as Human vs. Human, AI vs. AI, or Human vs. AI",
            epilog="Note: If all default parameters are used, both players will be AI and "
            "use Minimax with search depth = 4, and 64 bit Zobrist keys.",
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
            choices=[32, 64, 128],
            required=False,
            help="Key size (in bits) used for red AI player Zobrist hashing",
        )

        self._parser.add_argument(
            "-rn",
            "--red_number_zobrist_states",
            type=int,
            choices=[1, 2],
            required=False,
            help="Number of Zobrist state values to maintain for red.",
        )

        self._parser.add_argument(
            "-rz",
            "--red_zkeys_seed",
            type=int,
            required=False,
            help="Seed for red player Zobrist Keys generator. "
            "32-bit unsigned int.",
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
            choices=[32, 64, 128],
            required=False,
            help="Key size (in bits) used for black AI player Zobrist hashing",
        )

        self._parser.add_argument(
            "-bn",
            "--black_number_zobrist_states",
            type=int,
            choices=[1, 2],
            required=False,
            help="Number of Zobrist state values to maintain for black.",
        )

        self._parser.add_argument(
            "-bz",
            "--black_zkeys_seed",
            type=int,
            required=False,
            help="Seed for black player Zobrist Keys generator. "
            "32-bit unsigned int.",
        )

        self._parser.add_argument(
            "-s",
            "--save_summary",
            action="store_true",
            help="Save GameSummary as .json",
        )

        self._parser.add_argument(
            "-d",
            "--output_dir_suffix",
            type=str,
            required=False,
            help="String to append to end of output directory name. Output dir "
            "relative to cwd will be "
            "./data/game_summaries/<timestamp><optional-suffix>",
        )

    def get_args(self) -> dict[str, Any]:
        self._attach_args()
        args_namespace = self._parser.parse_args()
        return vars(args_namespace)


def main() -> dict[str, Any]:
    command_retriever = XiangqiGameCommandLine()
    my_command = command_retriever.get_args()
    return my_command


if __name__ == "__main__":
    result = main()
    print(result)
