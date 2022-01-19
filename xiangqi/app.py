import json
import pkg_resources
from .game import Game


def run():
    game_start_data = pkg_resources.resource_filename('xiangqi',
                                                      'data/game_start.json')

    with open(game_start_data) as game_info:
        game_config = json.load(game_info)
    my_game = Game(game_config)
    my_game.play_interactive()


if __name__ == '__main__':
    run()
