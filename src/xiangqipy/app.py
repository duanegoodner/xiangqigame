"""
Contains a single function, run(), that does everything needed to play a Game.
"""
from pathlib import Path
from typing import Dict
import colorama
import xiangqi_bindings as bindings
from xiangqipy.command_input import (
    RunKwargsInterpreter,
    XiangqiGameCommandLine,
)
from xiangqipy.game import Game
from xiangqipy.game_output_generator import GameOutputGenerator
from xiangqipy.game_summary import GameSummary
from xiangqipy.handlers.signals import set_signal_handlers
from xiangqipy.player_builder import RedAndBlackPlayersBuilder


def run(custom_output_root: Path = None, **kwargs: Dict) -> GameSummary:
    """
    Collects command line args, instantiates everything needed for a Game, runs the Game,
    and optionally saves GameSummary info.

    @param custom_output_root: optional Path of output parent dir; if not specified,
    defaults to ./data/game_summaries/<timestamp-based-dirname>
    @param **kwargs: optional key value pairs that can supplement or override
    command line args.
    """
    set_signal_handlers()
    colorama.init()

    command_line_kwargs = XiangqiGameCommandLine().get_args()
    run_kwargs = {**command_line_kwargs, **kwargs}

    run_kwargs_interpreter = RunKwargsInterpreter(run_kwargs=run_kwargs)
    xiangqi_command = run_kwargs_interpreter.interpret_command()

    game_board = bindings.GameBoard()
    # game_board_new = bindings.GameBoardFactory().create()
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
            custom_output_root=custom_output_root,
            # game_collection_id=game_collection_id,
        )
        output_generator.generate_output()

    return game_summary


if __name__ == "__main__":
    run()
