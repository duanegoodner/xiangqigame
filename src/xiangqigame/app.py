import colorama
import xiangqigame_core as core
from pathlib import Path
from xiangqigame.command_input import (
    RunKwargsInterpreter,
    XiangqiGameCommandLine,
)
from xiangqigame.game import Game, GameSummary
from xiangqigame.game_output_generator import GameOutputGenerator
from xiangqigame.game_summary_io import export_game_summary
from xiangqigame.game_summary_plot_manager import GameSummaryPlotManager
from xiangqigame.handlers.signals import set_signal_handlers
from xiangqigame.output_path_builder import create_output_path
from xiangqigame.player_builder import RedAndBlackPlayersBuilder


def run(**kwargs) -> GameSummary:
    set_signal_handlers()
    colorama.init()

    command_line_kwargs = XiangqiGameCommandLine().get_args()
    run_kwargs = {**command_line_kwargs, **kwargs}

    run_kwargs_interpreter = RunKwargsInterpreter(run_kwargs=run_kwargs)
    xiangqi_command = run_kwargs_interpreter.interpret_command()

    game_board = core.GameBoard()
    players = RedAndBlackPlayersBuilder(
        xiangqi_command=xiangqi_command, game_board=game_board
    ).build()
    my_game = Game(players=players, game_board=game_board)

    game_summary = my_game.play()

    # Optionally saves GameSummary and plots under /data/<game-ID>
    if xiangqi_command.save_summary:
        output_generator = GameOutputGenerator(
            game_summary=game_summary,
            output_dir_suffix=xiangqi_command.output_dir_suffix,
        )
        output_generator.generate_output()


if __name__ == "__main__":
    run()
