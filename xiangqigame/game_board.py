from typing import Set

import numpy as np
from xiangqigame.board_rules import BOARD_RULES as br
from xiangqigame.enums import PieceColor, PieceType
from xiangqigame.utilities.notation_converter import file_index_of
from xiangqigame.utilities.piece_decoder import decode_piece, encode_piece
from xiangqigame.game_piece import GamePiece, null_piece
from xiangqigame.board_components import BoardSpace, BoardVector, castles
from xiangqigame.move import Move, ExecutedMove
from xiangqigame.utilities import board_utilities as bu


class GameBoard:
    opponent_of = {
        PieceColor.RED: PieceColor.BLACK,
        PieceColor.BLACK: PieceColor.RED
    }

    def __init__(self, board_data):
        assert len(board_data[0]) == br.board_dim.num_files
        assert len(board_data) == br.board_dim.num_ranks
        self._map = np.array(
            [[decode_piece(board_data[row][col]) for col in
              range(br.board_dim.num_files)]
             for row in range(br.board_dim.num_ranks)])

    def __str__(self):
        file_labels = [' ' + char for char in list(file_index_of.keys())]
        file_labels.insert(0, '  ')

        board_list = [[encode_piece(self._map[row][col]) for col in
                       range(len(self._map[0]))]
                      for row in range(len(self._map))]

        for row_index in range(len(board_list)):
            board_list[row_index].insert(0, ' ' + str(row_index + 1) + ' ' * (
                        2 - len(str(row_index + 1))))

        board_list.insert(0, file_labels)

        board_list = ["  ".join(row) for row in board_list]

        return str(
            '\n\n'.join([str(rank) for rank in board_list])
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

    def execute_move(self, move: Move):
        moving_piece = self.get_occupant(move.start)
        destination_piece = self.get_occupant(move.end)
        self.set_occupant(space=move.end, piece=moving_piece)
        self.set_occupant(space=move.start, piece=null_piece)
        return ExecutedMove(spaces=move, moving_piece=moving_piece,
                            destination_piece=destination_piece)

    def undo_move(self, move: ExecutedMove):
        self.set_occupant(move.spaces.start, move.moving_piece)
        self.set_occupant(move.spaces.end, move.destination_piece)

    def get_all_spaces_occupied_by(self, color):
        return {BoardSpace(rank, file) for rank in
                range(br.board_dim.num_ranks) for file in
                range(br.board_dim.num_files) if
                self.get_color(BoardSpace(rank, file)) == color}

    def get_general_position(self, color: PieceColor):
        castle_edges = castles[color]
        castle_spaces = [
            BoardSpace(rank, file) for rank in
            range(castle_edges.min_rank, castles[color].max_rank + 1) for
            file in range(castle_edges.min_file, castle_edges.max_file + 1)
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
            return self._map[low_rank:high_rank, from_space.file]

        else:
            first_step_rank = from_space.rank - 1
            low_rank = to_rank
            high_rank = first_step_rank
        return self._map[(low_rank + 1):(high_rank + 1), from_space.file]

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
        forward_space = from_position.add_board_vector(
            BoardVector(bu.forward_direction[color], 0))

        if bu.is_on_board(forward_space):
            soldier_moves.add(Move(from_position, forward_space))
        if not bu.is_in_homeland_of(color, from_position):
            for space in bu.get_adjacent_spaces(from_position, vertical=False):
                soldier_moves.add(Move(from_position, space))
        return soldier_moves

    def cannon_moves(self, from_position: BoardSpace, color: PieceColor):
        cannon_moves = set()
        search_directions = bu.board_vectors_horiz + bu.board_vectors_vert

        for direction in search_directions:
            search_results = self.search_spaces(from_position, direction)
            cannon_moves.update(
                Move(from_position, empty_space) for empty_space in
                search_results['empty_spaces'])

            if search_results['first_occupied_space']:
                second_search = self.search_spaces(
                    search_results['first_occupied_space'], direction)
                if second_search['first_occupied_space'] and \
                        (self.get_color(second_search['first_occupied_space'])
                         == self.opponent_of[color]):
                    cannon_moves.add(
                        Move(from_position,
                             second_search['first_occupied_space']))

        return cannon_moves

    def chariot_moves(self, from_position: BoardSpace, color: PieceColor):
        chariot_moves = set()
        search_directions = bu.board_vectors_horiz + bu.board_vectors_vert

        for direction in search_directions:
            search_results = self.search_spaces(from_position, direction)
            chariot_moves.update(
                Move(from_position, empty_space) for empty_space in
                search_results['empty_spaces'])

            if (search_results['first_occupied_space']) and \
                    (self.get_occupant(search_results['first_occupied_space'])
                     ['color'] == self.opponent_of[color]):
                chariot_moves.add(
                    Move(from_position, search_results['first_occupied_space'])
                )

        return chariot_moves

    def horse_moves(self, from_position: BoardSpace, color: PieceColor):
        horse_moves = set()

        for direction in bu.horse_paths.keys():
            first_step = from_position.add_board_vector(direction)
            if bu.is_on_board(first_step) and not self.is_occupied(first_step):
                second_steps = [
                    first_step.add_board_vector(second_step) for
                    second_step in bu.horse_paths[direction]
                ]
                horse_moves.update(
                    Move(from_position, second_step) for second_step in
                    second_steps if bu.is_on_board(second_step) and
                    (self.get_color(second_step) != color))

        return horse_moves

    def elephant_moves(self, from_position: BoardSpace, color: PieceColor):
        elephant_moves = set()

        for direction in bu.diag_directions:
            first_step = from_position.add_board_vector(direction)
            if (
                    bu.is_on_board(first_step) and
                    not self.is_occupied(first_step)
                    and bu.is_in_homeland_of(color, first_step)
            ):
                second_step = first_step.add_board_vector(direction)
                if (
                        bu.is_on_board(second_step) and (
                        self.get_color(second_step) != color) and (
                        bu.is_in_homeland_of(color, second_step))
                ):
                    elephant_moves.add(Move(from_position, second_step))

        return elephant_moves

    def advisor_moves(self, from_position: BoardSpace, color: PieceColor):
        advisor_moves = set()

        for direction in bu.diag_directions:
            destination = from_position.add_board_vector(direction)
            if bu.is_on_board(destination) and \
                    (destination.is_in_castle_of(color)) \
                    and (self.get_color(destination) != color):
                advisor_moves.add(Move(from_position, destination))

        return advisor_moves

    def flying_general_moves(self, from_position: BoardSpace,
                             color: PieceColor):
        flying_moves = set()
        other_gen_position = \
            self.get_general_position(self.opponent_of[color])

        if bu.in_same_file(from_position, other_gen_position):
            intermediate_pieces = \
                self.get_vertical_path(from_position, other_gen_position.rank)
            if all(piece['type'] == PieceType.NULL_PIECE
                   for piece in intermediate_pieces):
                flying_moves.add(Move(from_position, other_gen_position))

        return flying_moves

    def standard_general_moves(self, from_position: BoardSpace,
                               color: PieceColor):

        return {Move(from_position, space) for space in
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

    def calc_temp_moves_from(self, position: BoardSpace):
        color = self.get_color(position)
        return self.piece_moves[self.get_type(position)](self, position, color)

    def calc_temp_moves_of(self, color: PieceColor):
        team_moves = set()

        for space in self.get_all_spaces_occupied_by(color):
            team_moves = team_moves | self.calc_temp_moves_from(space)

        return team_moves

    def calc_final_moves_from(self, position: BoardSpace):
        potential_moves = self.calc_temp_moves_from(position)
        moves_resulting_in_check = set()

        for move in potential_moves:
            test_move = self.execute_move(move)
            resulting_opp_moves = self.calc_temp_moves_of(
                self.opponent_of[test_move.moving_piece['color']])
            resulting_opp_destinations = {
                move.end for move in resulting_opp_moves
            }
            if self.get_general_position(test_move.moving_piece['color']) \
                    in resulting_opp_destinations:
                moves_resulting_in_check.add(move)
            self.undo_move(test_move)

        legal_moves = potential_moves - moves_resulting_in_check

        return legal_moves

    def calc_final_moves_of(self, color) -> Set[Move]:
        final_moves = set()

        for space in self.get_all_spaces_occupied_by(color):
            final_moves = final_moves | self.calc_final_moves_from(space)

        return final_moves
