import numpy as np
from typing import Tuple, Dict
from piece_decoder import decode_piece, encode_piece
from enums import GameState, PieceColor, PieceType
from game_piece import GamePiece
from game_rules import castles




class GameBoard:

    def __init__(self, board_data):
        num_files = len(board_data[0])
        num_ranks = len(board_data)
        self._map = [[decode_piece(board_data[row][col]) for col
                      in range(num_files)] for row in range(num_ranks)]

    def __repr__(self):
        return repr(
            np.asarray(
                [[encode_piece(self._map[row][col]) for col in
                  range(len(self._map[0]))] for row in range(len(self._map))]
            )
        )

    def get_occupant(self, space: tuple):
        return self._map[space[0]][space[1]]

    def set_occupant(self, space: Tuple[int], piece: Dict):
        self._map[space[0]][space[1]] = piece

    def get_general_position(self, piece_color: PieceColor):
        for space in castles[piece_color]:
            if self.get_occupant(space) == PieceType.GENERAL:
                return space



    def null_piece_moves(self):
        return set()

    def soldier_moves(self, from_position: tuple):
        return set()

    def cannon_moves(self, from_position: tuple):
        return set()

    def chariot_moves(self, from_position: tuple):
        return set()

    def horse_moves(self, from_position: tuple):
        return set()

    def elephant_moves(self, from_position: tuple):
        return set()

    def advisor_moves(self, from_position: tuple):
        return set()

    def flying_general_moves(self, from_position: tuple):

        cur_color = self.get_occupant(from_position).get_piece_color()

        flying_move = set()
        other_gen_position = self.get_general_position()




    def standard_general_moves(self, from_position: tuple):
        return set()

    def general_moves(self, from_position: tuple):
        flying_move = self.flying_general_moves(from_position)
        standard_moves = self.standard_general_moves(from_position)
        return flying_move | standard_moves
