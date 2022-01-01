import json
from enums import GameState, PieceColor
from game_board import GameBoard
import numpy as np


def main():
    with open('game_start.json') as game_info:
        game_config = json.load(game_info)
    my_game = Game(game_config)
    print(my_game._board)


class Game:

    def __init__(self, game_config):
        board_data = game_config['board_data']

        self._game_state = GameState.UNFINISHED
        self._whose_turn = PieceColor.RED
        self._board = GameBoard(board_data)


if __name__ == '__main__':
    main()

