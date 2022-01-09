import numpy as np
from typing import Tuple, Dict, TypedDict, List
from piece_decoder import decode_piece, encode_piece
from enums import PieceColor, PieceType
from game_rules import castles, opponent_of, forward_direction
from game_piece import GamePiece
from board_space import BoardSpace, BoardVector

import board_utilities as bu


class GameBoard:

    def __init__(self, board_data):
        num_files = len(board_data[0])
        num_ranks = len(board_data)
        self._map = np.array([[decode_piece(board_data[row][col]) for col
                               in range(num_files)] for row in
                              range(num_ranks)])

    def __repr__(self):
        return repr(
            np.asarray(
                [[encode_piece(self._map[row][col]) for col in
                  range(len(self._map[0]))] for row in range(len(self._map))]
            )
        )

    def is_occupied(self, space: BoardSpace):
        return self._map[space.rank][space.file][
                   'type'] != PieceType.NULL_PIECE

    def get_occupant(self, space: BoardSpace):
        return self._map[space.rank][space.file]

    def get_type(self, space: BoardSpace):
        return self._map[space.rank][space.file]['type']

    def get_color(self, space: BoardSpace) -> PieceColor:
        return self._map[space.rank][space.file]['color']

    def set_occupant(self, space: BoardSpace, piece: GamePiece):
        self._map[space.rank][space.file] = piece

    def get_general_position(self, color: PieceColor):
        castle_edges = castles[color]

        castle_spaces = [
            BoardSpace(rank, file) for rank in
            range(castle_edges.min_rank, castles[color].max_rank + 1) for
            file in range(castle_edges.min_file, castle_edges.max_file)
        ]

        for space in castle_spaces:
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

    def search_spaces(self, from_space: BoardSpace, direction: BoardVector):

        empty_spaces = []
        first_occupied_space = None

        next_step = from_space.add_board_vector(direction)

        while bu.is_on_board(next_step) and \
                self.get_occupant(next_step)['type'] == PieceType.NULL_PIECE:
            empty_spaces.append(next_step)
            next_step = next_step.add_board_vector(direction)

        if bu.is_on_board(next_step):
            first_occupied_space = next_step

        return {
            'empty_spaces': empty_spaces,
            'first_occupied_space': first_occupied_space
        }

    def null_piece_moves(self, *args):
        return set()

    def soldier_moves(self, from_position: BoardSpace, color: PieceColor):
        soldier_moves = set()

        # color = self.get_color(from_position)

        forward_space = from_position.add_board_vector(
            BoardVector(forward_direction[color], 0)
        )
        if bu.is_on_board(forward_space):
            soldier_moves.add((from_position, forward_space))

        if not bu.is_in_homeland_of(color, from_position):
            for space in bu.get_adjacent_spaces(from_position, vertical=False):
                soldier_moves.add((from_position, space))

        return soldier_moves

    def cannon_moves(self, from_position: BoardSpace, color: PieceColor):

        cannon_moves = set()
        # color = self.get_color(from_position)
        search_directions = bu.board_vectors_horiz + bu.board_vectors_vert

        for direction in search_directions:
            search_results = self.search_spaces(from_position, direction)
            cannon_moves.update(
                (from_position, empty_space) for empty_space in
                search_results['empty_spaces'])

            if search_results['first_occupied_space']:
                landing_space = search_results['first_occupied_space'].\
                    add_board_vector(direction)
                if bu.is_on_board(landing_space) and (
                        self.get_color(landing_space) == opponent_of[color]
                ):
                    cannon_moves.add((from_position, landing_space))

        return cannon_moves

    def chariot_moves(self, from_position: BoardSpace, color: PieceColor):

        chariot_moves = set()
        # color = self.get_color(from_position)
        search_directions = bu.board_vectors_horiz + bu.board_vectors_vert

        for direction in search_directions:
            search_results = self.search_spaces(from_position, direction)
            chariot_moves.update(
                (from_position, empty_space) for empty_space in
                search_results['empty_spaces'])

            if (search_results['first_occupied_space']) and \
                    (self.get_occupant(search_results['first_occupied_space'])
                     ['color'] == opponent_of[color]):
                chariot_moves.add(
                    (from_position, search_results['first_occupied_space'])
                )

        return chariot_moves

    def horse_moves(self, from_position: BoardSpace, color: PieceColor):

        horse_moves = set()
        # color = self.get_color(from_position)

        for direction in bu.horse_paths.keys():
            first_step = from_position.add_board_vector(direction)
            if bu.is_on_board(first_step) and not self.is_occupied(first_step):
                second_steps = [first_step.add_board_vector(second_step) for
                                second_step in bu.horse_paths[direction]]

                horse_moves.update(
                    (from_position, second_step) for second_step in
                    second_steps if bu.is_on_board(second_step) and
                    (self.get_color(second_step) != color))

        return horse_moves

    def elephant_moves(self, from_position: BoardSpace, color: PieceColor):

        elephant_moves = set()
        # color = self.get_color(from_position)

        for direction in bu.diag_directions:
            first_step = from_position.add_board_vector(direction)
            if bu.is_on_board(first_step) and not self.is_occupied(first_step):
                second_step = first_step.add_board_vector(direction)
                if bu.is_on_board(second_step) and (self.get_color(second_step)
                                                    != color):
                    elephant_moves.add((from_position, second_step))

        return elephant_moves

    def advisor_moves(self, from_position: BoardSpace, color: PieceColor):
        advisor_moves = set()
        # color = self.get_color(from_position)

        for direction in bu.diag_directions:
            destination = from_position.add_board_vector(direction)
            if bu.is_on_board(destination) and\
                    (destination.is_in_castle_of(color))\
                    and (self.get_color(destination) != color):
                advisor_moves.add((from_position, destination))

        return advisor_moves

    def flying_general_moves(self, from_position: BoardSpace, color: PieceColor):

        flying_moves = set()

        # is there a way to avoid having this line in every move function??
        # color = self.get_occupant(from_position)['color']

        other_gen_position = \
            self.get_general_position(opponent_of[color])

        if bu.in_same_file(from_position, other_gen_position):
            intermediate_pieces = \
                self.get_vertical_path(from_position, other_gen_position.rank)
            if all(piece['type'] == PieceType.NULL_PIECE
                   for piece in intermediate_pieces):
                flying_moves.add((from_position, other_gen_position))

        return flying_moves

    def standard_general_moves(self, from_position: BoardSpace, color: PieceColor):

        # color = self.get_occupant(from_position)['color']

        return {(from_position, space) for space in
                bu.get_adjacent_spaces(from_position) if
                space.is_in_castle_of(color) and
                self.get_color(space) != color}

    def general_moves(self, from_position: BoardSpace, color: PieceColor):
        flying_move = self.flying_general_moves(from_position, color)
        standard_moves = self.standard_general_moves(from_position, color)
        return flying_move | standard_moves

    piece_moves = {
        PieceType.NULL_PIECE: null_piece_moves,
        PieceType.SOLDIER: soldier_moves,
        PieceType.CANNON: cannon_moves,
        PieceType.CHARIOT: chariot_moves,
        PieceType.HORSE: horse_moves,
        PieceType.ELEPHANT: elephant_moves,
        PieceType.ADVISOR: advisor_moves,
        PieceType.GENERAL: general_moves
    }

    def calc_moves_from(self, position: BoardSpace):

        color = self.get_color(position)

        return self.piece_moves[self.get_type(position)](self, position, color)
