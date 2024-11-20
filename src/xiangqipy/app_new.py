from dataclasses import dataclass
from enums import EvaluatorTypeNew
from pathlib import Path
from typing import Dict, Any, Literal, NamedTuple
import colorama
import xiangqi_bindings as bindings
from xiangqipy.command_input import (
    RunKwargsInterpreter,
    XiangqiGameCommandLine,
)
from xiangqipy.game import Game
from xiangqipy.game_interfaces import Player
from xiangqipy.game_output_generator import GameOutputGenerator
from xiangqipy.game_summary import GameSummary
from xiangqipy.handlers.signals import set_signal_handlers
from xiangqipy.player_builder import RedAndBlackPlayersBuilder
import xiangqipy.binding_type_unions as btu


@dataclass
class PlayerRunArgs:
    player_type: str = None
    algo: str = None
    strength: int = None
    key_size: int = None
    number_zobrist_states: int = None
    zkeys_seed: int = None


@dataclass
class PlayerInputNew:
    evaluator_type: EvaluatorTypeNew
    zobrist_key_size: int | None = None
    num_confirmation_keys: int | None = None
    search_depth: int | None = None
    primary_calculator_seed: int | None = None
    confirmation_calculator_seeds: list[int] | None = None

    @classmethod
    def from_player_run_args(cls, player_run_args: PlayerRunArgs):
        if player_run_args.player_type == "person":
            return cls(evaluator_type=EvaluatorTypeNew.HUMAN)

        if player_run_args.algo == "random":
            return cls(evaluator_type=EvaluatorTypeNew.RANDOM)

        evaluator_type = EvaluatorTypeNew.MINIMAX
        zobrist_key_size = (
            player_run_args.key_size if player_run_args.key_size else 64
        )
        num_confirmation_keys = (
            player_run_args.number_zobrist_states - 1
            if player_run_args.number_zobrist_states
            else 0
        )
        search_depth = (
            player_run_args.strength if player_run_args.strength else 4
        )
        # TODO modify CommandInput to accept multiple seeds
        primary_calculator_seed = player_run_args.zkeys_seed
        confirmation_calculator_seeds = None

        return cls(
            evaluator_type=evaluator_type,
            zobrist_key_size=zobrist_key_size,
            num_confirmation_keys=num_confirmation_keys,
            search_depth=search_depth,
            primary_calculator_seed=primary_calculator_seed,
            confirmation_calculator_seeds=confirmation_calculator_seeds,
        )


@dataclass
class XiangqiGameCommandNew:
    red_input: PlayerInputNew
    black_input: PlayerInputNew
    save_summary: bool = False
    output_dir_suffix: str = ""


@dataclass
class RunKwargsInterpreterNew:
    run_kwargs: dict[str, Any]

    def player_kwargs(self, color: Literal["red", "black"]) -> dict[str, Any]:
        return {
            key.removeprefix(f"{color}_"): value
            for key, value in self.run_kwargs.items()
            if key.startswith(color)
        }

    def player_run_args(self, color: Literal["red", "black"]) -> PlayerRunArgs:
        return PlayerRunArgs(**self.player_kwargs(color))

    @property
    def red_player_run_args(self) -> PlayerRunArgs:
        return self.player_run_args("red")

    @property
    def black_player_run_args(self) -> PlayerRunArgs:
        return self.player_run_args("black")

    def build_xiangqi_game_command(self) -> XiangqiGameCommandNew:
        return XiangqiGameCommandNew(
            red_input=PlayerInputNew.from_player_run_args(
                self.red_player_run_args
            ),
            black_input=PlayerInputNew.from_player_run_args(
                self.black_player_run_args
            ),
            save_summary=self.run_kwargs["save_summary"],
            output_dir_suffix=self.run_kwargs["output_dir_suffix"],
        )




class MoveEvaluatorBuilder:


    def __init__(
        self,
        color: bindings.PieceColor,
        player_input: PlayerInputNew,
        game_board: bindings.GameBoard,
    ):
        self.color = color
        self.player_input = player_input
        self.game_board = game_board

    @property
    def zobrist_bit_size_label(self) -> str:



def build_move_evaluator(
    color: bindings.PieceColor,
    player_input: PlayerInputNew,
    game_board: bindings.GameBoard,
) -> btu.MinimaxMoveEvaluatorType:
    pass


def run(custom_output_root: Path = None, **kwargs: dict) -> GameSummary:
    set_signal_handlers()
    colorama.init()

    command_line_kwargs = XiangqiGameCommandLine().get_args()
    run_kwargs = {**command_line_kwargs, **kwargs}
    xiangqi_game_command = RunKwargsInterpreterNew(
        run_kwargs=run_kwargs
    ).build_xiangqi_game_command()

    game_board = bindings.GameBoard()

    print("pause")


if __name__ == "__main__":
    run()
