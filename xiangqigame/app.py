import json
import pkgutil
from xiangqi_game.game import Game
from xiangqi_game.handlers.signals import set_signal_handlers
from subprocess import Popen, call

import venv


def run():
    set_signal_handlers()

    game_config = json.loads(pkgutil.get_data('xiangqi_game', 'data/game_start.json'))
    my_game = Game(game_config)
    my_game.play_interactive()


if __name__ == '__main__':
    run()
