import numpy as np
from typing import Tuple, Dict, TypedDict, List
from piece_decoder import decode_piece, encode_piece
from enums import PieceColor, PieceType
from game_rules import castles, opponent_of, forward_direction
from game_piece import GamePiece
from board_space import BoardSpace

import board_utilities as bu


class GameBoard:

    def __init__(self, board_data):
        num_files = len(board_data[0])
        num_ranks = len(board_data)
        self._map = np.array([[decode_piece(board_data[row][col]) for col
                      in range(num_files)] for row in range(num_ranks)])

    def __repr__(self):
        return repr(
            np.asarray(
                [[encode_piece(self._map[row][col]) for col in
                  range(len(self._map[0]))] for row in range(len(self._map))]
            )
        )

    def get_occupant(self, space: BoardSpace):
        return self._map[space.rank][space.file]

    def get_color(self, space: BoardSpace):
        return self._map[space.rank][space.file]['color']

    def set_occupant(self, space: BoardSpace, piece: GamePiece):
        self._map[space.rank][space.file] = piece

    def get_general_position(self, piece_color: PieceColor):
        for space in castles[piece_color]:
            if self.get_occupant(space)['type'] == PieceType.GENERAL:
                return space

    def get_vertical_path(self, from_space: BoardSpace, to_rank: int):
        separation = (to_rank - from_space.rank)
        if separation == 0:
            return list()
        elif separation > 0:
            first_step_rank = from_space.rank + 1
            low_rank = first_step_rank
            high_rank = to_rank
        else:
            first_step_rank = from_space.rank - 1
            low_rank = to_rank
            high_rank = first_step_rank
        return self._map[low_rank:(high_rank + 1), from_space.file]

    def null_piece_moves(self):
        return set()

    def soldier_moves(self, from_position: BoardSpace):
        soldier_moves = set()

        cur_color = self.get_color(from_position)

        forward_space = bu.add_board_spaces(
            from_position, BoardSpace(0, forward_direction[cur_color]))
        if bu.is_on_board(forward_space):
            soldier_moves.add((from_position, forward_space))

        if not bu.is_in_homeland_of(cur_color, from_position):
            for space in bu.get_side_adjacent_spaces(from_position):
                soldier_moves.add((from_position, space))

        return soldier_moves

    def cannon_moves(self, from_position: BoardSpace):
        return set()

    def chariot_moves(self, from_position: BoardSpace):


        chariot_moves = set()

        horizontal_spaces = self._map[from_position.rank, :]
        vertical_spaces = self._map[:, from_position.file]

        # TODO: add bu function to determine which spaces above are Ok to move to

        # TODO: add a step counter var and use that to check if still in array bounds
        for space in horizontal_spaces[from_position.file:]:
            print(space)

        for space in np.flip(horizontal_spaces[:from_position.file]):
            pass

        for space in vertical_spaces[from_position.rank:]:
            pass

        for space in np.flip(vertical_spaces[:from_position.rank]):
            pass



        return chariot_moves

    def horse_moves(self, from_position: tuple):
        return set()

    def elephant_moves(self, from_position: tuple):
        return set()

    def advisor_moves(self, from_position: tuple):
        return set()

    def flying_general_moves(self, from_position: BoardSpace):

        flying_moves = set()

        # is there a way to avoid having this line in every move function??
        cur_color = self.get_occupant(from_position)['color']

        other_gen_position =\
            self.get_general_position(opponent_of[cur_color])

        if bu.in_same_file(from_position, other_gen_position):
            intermediate_pieces =\
                self.get_vertical_path(from_position, other_gen_position.rank)
            if all(piece['type'] == PieceType.NULL_PIECE
                   for piece in intermediate_pieces):
                flying_moves.add((from_position, other_gen_position))

        return flying_moves

    def standard_general_moves(self, from_position: BoardSpace):

        cur_color = self.get_occupant(from_position)['color']

        return {(from_position, space) for space in
                bu.get_adjacent_spaces(from_position) if space in
                castles[cur_color]}

    def general_moves(self, from_position: BoardSpace):
        flying_move = self.flying_general_moves(from_position)
        standard_moves = self.standard_general_moves(from_position)
        return flying_move | standard_moves
