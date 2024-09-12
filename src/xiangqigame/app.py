import colorama
import xiangqigame_core as core
from pathlib import Path
from xiangqigame.command_input import (
    RunKwargsInterpreter,
    XiangqiGameCommandLine,
)
from xiangqigame.game import Game
from xiangqigame.game_summary_io import export_game_summary
from xiangqigame.handlers.signals import set_signal_handlers
from xiangqigame.player_builder import RedAndBlackPlayersBuilder


def run(**kwargs) -> Path | None:
    set_signal_handlers()
    colorama.init()
    
    command_line_kwargs = XiangqiGameCommandLine().get_args()
    run_kwargs = {**command_line_kwargs, **kwargs}
        
    run_kwargs_interpreter = RunKwargsInterpreter(
        run_kwargs=run_kwargs
    )
    xiangqi_command = run_kwargs_interpreter.interpret_command()
    
    game_board = core.GameBoard()
    players = RedAndBlackPlayersBuilder(
        xiangqi_command=xiangqi_command, game_board=game_board
    ).build()
    my_game = Game(players=players, game_board=game_board)
    game_summary = my_game.play()
    
    if xiangqi_command.save_summary:
        export_game_summary(game_summary=game_summary)
    
    return game_summary
    
    
if __name__ == "__main__":
    run()
