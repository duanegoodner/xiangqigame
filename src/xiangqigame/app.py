import colorama
from xiangqigame.command_input import XiangqiGameCommandLine
from xiangqigame.player_builder import RedAndBlackPlayersBuilder
from xiangqigame.game import Game
from xiangqigame.handlers.signals import set_signal_handlers


def run(*args):
    set_signal_handlers()
    colorama.init()
    xiangqi_command = XiangqiGameCommandLine().get_args(*args)
    players = RedAndBlackPlayersBuilder(xiangqi_command=xiangqi_command).build()
    my_game = Game(players)
    my_game.play()


if __name__ == '__main__':
    run()
