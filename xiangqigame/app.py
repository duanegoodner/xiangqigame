import colorama
import json
import pkgutil
from xiangqigame.game import Game
from xiangqigame.handlers.signals import set_signal_handlers
from subprocess import Popen, call
import xiangqigame.data

import venv


def run():
    set_signal_handlers()
    colorama.init()

    game_config = json.loads(pkgutil.get_data('xiangqigame.data', 'game_start.json'))
    my_game = Game(game_config)
    my_game.play_interactive()


if __name__ == '__main__':
    run()
