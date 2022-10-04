import math
from typing import Set, List
import numpy as np
from xiangqigame.board_components_new import BoardVector, BoardSpace, \
    SpaceSearchResult
from xiangqigame.piece_definitions import PType, PColor
from xiangqigame.board_utilities_new import BOARD_UTILITIES as bu
from xiangqigame.board_layout import BoardLayout as blo
from xiangqigame.move_new import Move, ExecutedMove
from xiangqigame.starting_board_builder import StartingBoardBuilder


class GameBoard:
    _river_edge_black = 4
    _river_edge_red = 5
    _num_ranks = 10
    _num_files = 9
    _horizontal_vects = np.array([[0, 1], [0, -1]])

    def __init__(self, board_map: np.array = None):
        if board_map is None:
            board_map = StartingBoardBuilder().build_initial_board()
        self._map = board_map

    @property
    def castles(self):
        return {
            PColor.BLK: self._map[bu.castle_slices[PColor.BLK]],
            PColor.RED: self._map[bu.castle_slices[PColor.RED]]
        }

    def is_occupied(self, space: BoardSpace):
        return self._map[space] != 0

    def get_type(self, space: BoardSpace) -> int:
        return abs(self._map[space.rank, space.file])

    def get_color(self, space: BoardSpace):
        piece_val = self._map[space.rank, space.file]
        return 0 if piece_val == 0 else int(math.copysign(1, piece_val))

    def get_occupant(self, space: BoardSpace):
        return self._map[space.rank, space.file]

    def set_occupant(self, space: BoardSpace, piece: int):
        self._map[space.rank, space.file] = piece

    def exists_and_passes_color_test(
            self, dest: BoardSpace, moving_piece_color: int):
        return bu.is_on_board(dest) and (
                self.get_color(dest) != moving_piece_color)

    def execute_move(self, move: Move):
        moving_piece = self.get_occupant(move.start)
        destination_piece = self.get_occupant(move.end)
        self.set_occupant(space=move.end, piece=moving_piece)
        self.set_occupant(space=move.start, piece=PType.NUL)
        return ExecutedMove(spaces=move, moving_piece=moving_piece,
                            destination_piece=destination_piece)

    def undo_move(self, move: ExecutedMove):
        self.set_occupant(move.spaces.start, move.moving_piece)
        self.set_occupant(move.spaces.end, move.destination_piece)

    def get_all_spaces_occupied_by(
            self, color: int) -> Set[BoardSpace]:
        spaces_array = np.argwhere(bu.is_color[color](self._map))
        return set(map(BoardSpace.from_plain_tuple, spaces_array))

    def get_general_position(self, color: int) -> BoardSpace:
        for space in blo.castle_coords[color]:
            if abs(self._map[space]) == PType.GEN:
                return BoardSpace.from_plain_tuple(space)

    def get_vertical_path(self, from_space: BoardSpace, to_rank: int):

        if from_space.rank == to_rank:
            return np.array([])
        elif to_rank > from_space.rank:
            return self._map[(from_space.rank + 1):to_rank, from_space.file]
        else:
            return self._map[(to_rank + 1):from_space.rank, from_space.file]

    def search_spaces(self, from_space: BoardSpace, direction: BoardVector):
        empty_spaces = []
        first_occupied_space = None
        next_step = from_space.add_board_vector(direction)
        # next_step = from_space + direction

        while bu.is_on_board(next_step) and (
                self.get_occupant(next_step) == PType.NUL):
            empty_spaces.append(next_step)
            next_step = next_step.add_board_vector(direction)
        if bu.is_on_board(next_step):
            first_occupied_space = next_step
        return SpaceSearchResult(
            empty_spaces=empty_spaces,
            first_occupied_space=first_occupied_space)

    # def null_piece_moves(self, *args):
    #     return []

    def soldier_moves(self, from_position: BoardSpace, color: int):
        soldier_moves = set()
        fwd_space = from_position.add_board_vector(bu.fwd_unit_vect(color))

        if self.exists_and_passes_color_test(fwd_space, color):
            soldier_moves.add(Move(from_position, fwd_space))
        if not bu.is_in_homeland_of(color, from_position):
            for vector in bu.sideways_unit_vect:
                dest = from_position + vector
                if self.exists_and_passes_color_test(dest, color):
                    soldier_moves.add(Move(from_position, dest))
        return soldier_moves

    def cannon_moves(self, from_position: BoardSpace, color: int):
        cannon_moves = set()
        search_directions = bu.all_orthogonal_unit_vects

        for direction in search_directions:
            search_results = self.search_spaces(from_position, direction)
            cannon_moves.update(
                Move(from_position, empty_space) for empty_space in
                search_results.empty_spaces
            )

            #
            # cannon_moves.append(
            #     Move(from_position, empty_space) for empty_space in
            #     search_results.empty_spaces)

            if search_results.first_occupied_space:
                second_search = self.search_spaces(
                    search_results.first_occupied_space, direction)
                if second_search.first_occupied_space and (
                        self.get_color(second_search.first_occupied_space)
                        == bu.opponent_of[color]):
                    cannon_moves.add(Move(
                        from_position, second_search.first_occupied_space))

        return cannon_moves

    def chariot_moves(self, from_position: BoardSpace, color: int):
        chariot_moves = set()
        search_directions = bu.all_orthogonal_unit_vects
        for direction in search_directions:
            search_results = self.search_spaces(from_position, direction)

            chariot_moves.update(
                Move(from_position, empty_space) for empty_space in
                search_results.empty_spaces)

            if search_results.first_occupied_space and (
                    self.get_color(search_results.first_occupied_space)
                    == bu.opponent_of[color]):
                chariot_moves.add(
                    Move(from_position, search_results.first_occupied_space))

        return chariot_moves

    def horse_moves(self, from_position: BoardSpace, color: int):
        horse_moves = set()

        for direction in bu.horse_paths.keys():
            first_step = from_position.add_board_vector(direction)
            if bu.is_on_board(first_step) and not self.is_occupied(first_step):
                second_steps = [
                    first_step.add_board_vector(second_step) for second_step in
                    bu.horse_paths[direction]
                ]

                horse_moves.update(
                    Move(from_position, second_step) for second_step in
                    second_steps if bu.is_on_board(second_step) and
                    (self.get_color(second_step) != color))

        return horse_moves

    def elephant_moves(self, from_position: BoardSpace, color: int):
        elephant_moves = set()

        for direction in bu.diag_directions:
            first_step = from_position.add_board_vector(direction)
            if (
                    bu.is_on_board(first_step) and
                    not self.is_occupied(first_step) and
                    bu.is_in_homeland_of(color, first_step)
            ):
                second_step = first_step.add_board_vector(direction)
                if (
                        self.exists_and_passes_color_test(second_step, color)
                        and bu.is_in_homeland_of(color, second_step)
                ):
                    elephant_moves.add(Move(from_position, second_step))

        return elephant_moves

    def advisor_moves(self, from_position: BoardSpace, color: int):
        advisor_moves = set()

        for direction in bu.diag_directions:
            destination = from_position.add_board_vector(direction)
            if bu.is_in_castle_of(color, destination) and (
                    self.get_color(destination) != color):
                advisor_moves.add(Move(from_position, destination))

        return advisor_moves

    def flying_general_moves(self, from_position: BoardSpace, color: int):
        flying_moves = set()
        other_gen_position = self.get_general_position(
            bu.opponent_of[color])

        if from_position.file == other_gen_position.file:

            slice_start = min(from_position.rank, other_gen_position.rank)
            slice_end = max(from_position.rank, other_gen_position.rank) + 1
            if (self._map[slice_start:slice_end, from_position.file] ==
                PType.NUL).all():
                flying_moves.add(Move(from_position, other_gen_position))

            return flying_moves

    def standard_general_moves(self, from_position: BoardSpace, color: int):

        unit_vects = (
                              bu.fwd_unit_vect(color), bu.rev_unit_vect(color)
                          ) + bu.sideways_unit_vect
        adjacent_spaces = (from_position.add_board_vector(vect) for vect in
                           unit_vects)

        return {Move(from_position, space) for space in
                adjacent_spaces if
                bu.is_in_castle_of(color, space) and
                self.get_color(space) != color}

    def general_moves(self, from_position: BoardSpace, color: int):
        return (self.flying_general_moves(from_position, color) |
                self.standard_general_moves(from_position, color))

    piece_moves = {
        PType.SOL: soldier_moves,
        PType.CAN: cannon_moves,
        PType.CHA: chariot_moves,
        PType.HOR: horse_moves,
        PType.ELE: elephant_moves,
        PType.ADV: advisor_moves,
        PType.GEN: general_moves
    }

    def calc_temp_moves_from(self, position: BoardSpace) -> Set[Move]:
        color = self.get_color(position)
        return self.piece_moves[self.get_type(position)](self, position, color)

    def calc_temp_moves_of(self, color: int):
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
                bu.opponent_of[bu.get_piece_color(test_move.moving_piece)])
            resulting_opp_destinations = {
                move.end for move in resulting_opp_moves}
            if self.get_general_position(
                    bu.get_piece_color(test_move.moving_piece)
            ) in resulting_opp_destinations:
                moves_resulting_in_check.add(move)
            self.undo_move(test_move)

        legal_moves = potential_moves - moves_resulting_in_check

        return legal_moves

    def calc_final_moves_of(self, color: int) -> List[Move]:
        final_moves = set()
        for space in self.get_all_spaces_occupied_by(color):
            final_moves = final_moves | self.calc_final_moves_from(space)
        return final_moves
