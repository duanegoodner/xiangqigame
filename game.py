import json
from game_board import GameBoard


def main():
    with open('game_start.json') as game_info:
        game_config = json.load(game_info)

    new_game = Game(game_config['game_state'],
                    GameBoard(game_config['board_list']),
                    game_config['whose_turn'])


class Game:
    def __init__(self, game_state, board, whose_turn):
        self._game_state = game_state
        self._board = board
        self._whose_turn = whose_turn


if __name__ == '__main__':
    main()
