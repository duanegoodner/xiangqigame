import json
from enums import GameState, PieceColor, PieceType
from game_board import GameBoard
from game_piece import GamePiece
from utilities import get_size
from board_space import BoardSpace, BoardVector
import board_utilities as bu
import numpy as np
from collections import namedtuple
from game_rules import castles


def main():
    with open('game_start.json') as game_info:
        game_config = json.load(game_info)
    my_game = Game(game_config)
    print(my_game._board)
    print(my_game._board.calc_moves_of(PieceColor.RED))

    my_game_piece: GamePiece = {'piece_type': PieceType.HORSE,
                                'piece_color': PieceColor.RED}

    print(get_size(my_game_piece))




class Game:

    def __init__(self, game_config):
        board_data = game_config['board_data']

        self._game_state = GameState.UNFINISHED
        self._whose_turn = PieceColor.RED
        self._board = GameBoard(board_data)


if __name__ == '__main__':
    main()

