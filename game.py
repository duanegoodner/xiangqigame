import json
from enums import GameState, PieceColor
from game_board import GameBoard
from board_space import BoardSpace, BoardVector
import board_utilities as bu
import numpy as np
from collections import namedtuple
from game_rules import castles


def main():
    with open('game_start.json') as game_info:
        game_config = json.load(game_info)
    my_game = Game(game_config)
    print(my_game._board.get_color(BoardSpace(0, 0)))
    print(my_game._board)
    print(my_game._board.calc_moves_from(BoardSpace(6, 8)))


class Game:

    def __init__(self, game_config):
        board_data = game_config['board_data']

        self._game_state = GameState.UNFINISHED
        self._whose_turn = PieceColor.RED
        self._board = GameBoard(board_data)


if __name__ == '__main__':
    main()

