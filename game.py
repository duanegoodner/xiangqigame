import json
from game_board import GameBoard
import numpy as np


def main():
    with open('game_start.json') as game_info:
        game_config = json.load(game_info)

    new_game = Game(game_config)
    new_board = new_game.get_board()
    new_board_state = new_board.get_map()
    print(np.asarray(new_board_state))
    print(new_board.get_all_pieces_of('RED'))



    # test_val = new_board.is_legal_move(test_from, test_to)
    # print(test_val)


class Game:
    def __init__(self, game_config):
        self._game_state = game_config['game_state']
        self._board = GameBoard((game_config['board_list']))
        self._whose_turn = game_config['whose_turn']

    def get_game_state(self):
        return self._game_state

    def get_legal_moves(self, player):
        pass

    def get_board(self):
        return self._board

    def is_in_check(self, color):
        pass

    def make_move(self, from_square, to_square):
        pass

    def update_turn(self):
        if self._whose_turn == 'RED':
            self._whose_turn = 'BLACK'
        if self._whose_turn == 'BLACK':
            self._whose_turn = 'RED'


if __name__ == '__main__':
    main()
