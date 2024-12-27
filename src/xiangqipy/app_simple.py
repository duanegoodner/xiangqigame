from pathlib import Path

import xiangqi_bindings as xb

from xiangqipy.command_input import (
    XiangqiGameCommandLine,
    build_game_runner,
    get_output_file_info,
)
from xiangqipy.game_output_generator import GameOutputGenerator
from xiangqipy.game_summary_converter import CoreToPyGameSummaryConverter


def run(custom_output_root: Path = None, **kwargs):
    command_line_kwargs = XiangqiGameCommandLine().get_args()
    run_kwargs = {**command_line_kwargs, **kwargs}

    output_file_info = get_output_file_info(run_kwargs)
    game_runner = build_game_runner(run_kwargs)

    core_game_summary = game_runner.run_game()
    py_game_summary = CoreToPyGameSummaryConverter(
        core_game_summary=core_game_summary
    ).convert()

    if output_file_info.save_summary:
        output_generator = GameOutputGenerator(
            game_summary=py_game_summary,
            output_dir_suffix=output_file_info.output_dir_suffix,
            custom_output_root=custom_output_root,
        )
        output_generator.generate_output()


if __name__ == "__main__":
    run(red_algo="random", black_algo="random")
