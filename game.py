import json
from enums import GameState, PieceColor
from game_board import GameBoard
from board_space import BoardSpace, BoardVector, board_space_plus_vect
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

    print(my_game._board.get_vertical_path(BoardSpace(9, 1), 5))
    print(my_game._board.get_general_position(PieceColor.RED))
    print(my_game._board.general_moves(BoardSpace(0, 4)))
    print(my_game._board.soldier_moves(BoardSpace(3, 4)))
    print(my_game._board.chariot_moves(BoardSpace(0, 0)))
    print(bu.get_adjacent_spaces(BoardSpace(3, 3)))
    print(my_game._board.horse_moves(BoardSpace(0, 1)))




class Game:

    def __init__(self, game_config):
        board_data = game_config['board_data']

        self._game_state = GameState.UNFINISHED
        self._whose_turn = PieceColor.RED
        self._board = GameBoard(board_data)


if __name__ == '__main__':
    main()

