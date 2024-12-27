from pathlib import Path

import xiangqi_bindings as xb

from xiangqipy.command_input import XiangqiGameCommandLine, build_game_runner
from xiangqipy.game_summary_converter import CoreToPyGameSummaryConverter


def run(custom_output_root: Path = None, **kwargs) -> xb.GameSummary:
    command_line_kwargs = XiangqiGameCommandLine().get_args()
    run_kwargs = {**command_line_kwargs, **kwargs}
    game_runner = build_game_runner(run_kwargs)

    core_game_summary = game_runner.run_game()
    py_game_summary = CoreToPyGameSummaryConverter(
        core_game_summary=core_game_summary
    ).convert()

    return py_game_summary


if __name__ == "__main__":
    result = run(
        red_algo="minimax",
        red_strength=3,
        black_algo="minimax",
        black_strength=2,
    )
