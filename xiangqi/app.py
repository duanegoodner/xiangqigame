import json
import pkgutil
from xiangqi.game import Game
from xiangqi.handlers.signals import set_signal_handlers


def run():

    set_signal_handlers()

    game_config = json.loads(pkgutil.get_data('xiangqi', 'data/game_start.json'))
    my_game = Game(game_config)
    my_game.play_interactive()


if __name__ == '__main__':
    run()
