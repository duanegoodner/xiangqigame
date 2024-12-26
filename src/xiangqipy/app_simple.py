from dataclasses import dataclass
from pathlib import Path
from typing import Any, Literal

import xiangqi_bindings as xb

from xiangqipy.command_input import XiangqiGameCommandLine


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


def run(custom_output_root: Path = None, **kwargs) -> xb.GameSummary:
    command_line_kwargs = XiangqiGameCommandLine().get_args()
    run_kwargs = {**command_line_kwargs, **kwargs}

    # red_player_spec = build_player_spec(
    #     run_kwargs=run_kwargs,
    #     color_enum=xb.PieceColor.kRed,
    #     color_prefix="red_",
    # )
    # black_player_spec = build_player_spec(
    #     run_kwargs=run_kwargs,
    #     color_enum=xb.PieceColor.kBlk,
    #     color_prefix="black_",
    # )
    #
    # game_runner = xb.GameRunner(
    #     red_player_spec=red_player_spec, black_player_spec=black_player_spec
    # )

    game_runner = build_game_runner(run_kwargs)

    game_summary = game_runner.run_game()

    return game_summary


if __name__ == "__main__":
    result = run(
        red_algo="minimax",
        red_strength=3,
        black_algo="minimax",
        black_strength=2,
    )
