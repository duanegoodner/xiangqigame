import json
from game.game import Game


def run():
    with open('data/game_start.json') as game_info:
        game_config = json.load(game_info)
    my_game = Game(game_config)
    my_game.play_interactive()


if __name__ == '__main__':
    run()
