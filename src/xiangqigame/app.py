import json

import colorama
from xiangqigame.command_input import (CommandLineInterpreter,
                                       XiangqiGameCommandLine)
from xiangqigame.game import Game
from xiangqigame.handlers.signals import set_signal_handlers
from xiangqigame.player_builder import RedAndBlackPlayersBuilder
from xiangqigame_core import GameBoard


def run(*args):
    set_signal_handlers()
    colorama.init()
    command_line_args_new = XiangqiGameCommandLine().get_args()
    command_interpreter = CommandLineInterpreter(
        command_line_args=command_line_args_new
    )
    xiangqi_command = command_interpreter.interpret_command()
    game_board = GameBoard()
    players = RedAndBlackPlayersBuilder(
        xiangqi_command=xiangqi_command, game_board=game_board
    ).build()
    my_game = Game(players=players, game_board=game_board)
    game_summary = my_game.play()
    # game_summary_string = json.dumps(game_summary)
    print("pause")


if __name__ == "__main__":
    run()
