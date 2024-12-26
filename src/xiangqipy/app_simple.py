import xiangqi_bindings as xb
from pathlib import Path
from xiangqipy.command_input import (
    RawPlayerInput,
    XiangqiGameCommandLine,
)


arg_parse_to_player_spec = {
    "algo": "evaluator_type",
    "key_size": "zobrist_key_size_bits",
    "number_zobrist_states": "zobrist_calculator_count",
    "strength": "minimax_search_depth",
}

evaluator_type_str_to_enum = {
    "random": xb.EvaluatorType.kRandom,
    "human": xb.EvaluatorType.kHuman,
    "minimax": xb.EvaluatorType.kMinimax,
}


def run(custom_output_root: Path = None, **kwargs) -> xb.GameSummary:
    command_line_kwargs = XiangqiGameCommandLine().get_args()
    run_kwargs = {**command_line_kwargs, **kwargs}

    red_player_info = {"color": xb.PieceColor.kRed} | {
        key.removeprefix("red_"): val
        for key, val in run_kwargs.items()
        if key.startswith("red_")
    }
    black_player_info = {"color": xb.PieceColor.kBlk} | {
        key.removeprefix("black_"): val
        for key, val in run_kwargs.items()
        if key.startswith("black_")
    }

    red_player_input = RawPlayerInput(**red_player_info)
    black_player_input = RawPlayerInput(**black_player_info)

    red_player_spec = red_player_input.to_player_spec()
    black_player_spec = black_player_input.to_player_spec()

    game_runner = xb.GameRunner(
        red_player_spec=red_player_spec, black_player_spec=black_player_spec
    )

    game_summary = game_runner.run_game()

    return game_summary


if __name__ == "__main__":
    result = run(
        red_player_type="ai",
        black_player_type="ai",
        red_strength=4,
        black_strength=2,
    )
