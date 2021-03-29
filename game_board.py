from game_piece import GamePiece
import move_rules as move_rules
import numpy as np


_forward_direction = {
    'RED': 1,
    'BLACK': -1
}


class GameBoard:

    _move_rules = {
        'CHARIOT': move_rules.chariot,
        'HORSE': move_rules.horse,
        'ELEPHANT': move_rules.elephant,
        'ADVISOR': move_rules.advisor,
        'GENERAL': move_rules.general,
        'CANNON': move_rules.cannon,
        'SOLDIER': move_rules.soldier,
    }

    def __init__(self, board_list):
        num_ranks = len(board_list)
        num_files = len(board_list[0])

        self._map = np.array(
            [[GamePiece(board_list[row][col]) for col in
              range(num_files)] for row in range(num_ranks)]
        )
        self._castle_red = [[row, col + 3] for row in range(3) for col in
                            range(3)]
        self._castle_black = [[row + 7, col + 3] for row in range(3) for col in
                              range(3)]
        self._re_red = 4
        self._re_black = 5

    def get_map(self):
        return self._map

    def get_occupant(self, coordinates):
        return self._map[coordinates]

    # def get_general_position(self, color):
    #     for rank in range(len(self._map)):
    #         for file in range(len(self._map[0])):
    #             space = self._map[rank][file]
    #             piece = space.get_occupant()
    #             if piece.is_piece('GENERAL', color):
    #                 return rank, file
    #
    # def is_legal_move(self, from_space: BoardSpace, to_space: BoardSpace):
    #     piece = from_space.get_occupant()
    #     if piece.get_piece_type() == 'EMPTY':
    #         return False
    #     if piece.get_color() == to_space.get_occupant().get_color():
    #         return False
    #     if not self._move_rules[piece.get_piece_type()](self):
    #         return False
    #
    #     return True
