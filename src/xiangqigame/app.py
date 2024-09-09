import colorama
from xiangqigame_core import GameBoard

from xiangqigame.command_input import XiangqiGameCommandLine
from xiangqigame.game import Game
from xiangqigame.handlers.signals import set_signal_handlers
from xiangqigame.player_builder import RedAndBlackPlayersBuilder


def run(*args):
    set_signal_handlers()
    colorama.init()
    xiangqi_command = XiangqiGameCommandLine().get_args(*args)
    game_board = GameBoard()
    players = RedAndBlackPlayersBuilder(
        xiangqi_command=xiangqi_command, game_board=game_board
    ).build()
    my_game = Game(players=players, game_board=game_board)
    my_game.play()


if __name__ == "__main__":
    run()
