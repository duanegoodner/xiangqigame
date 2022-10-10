from typing import List, Tuple, Union, Dict, Any
import numpy as np
import xiangqigame.cython_modules.cython_board_utilities as cbu
import xiangqigame.cython_modules.general_moves as gm
from xiangqigame.piece_definitions import PType, PColor
from xiangqigame.board_utilities_new import BOARD_UTILITIES as bu
from xiangqigame.starting_board_builder import StartingBoardBuilder


class GameBoard:

    def __init__(self, board_map: np.array = None):
        if board_map is None:
            board_map = StartingBoardBuilder().build_initial_board_array()
        self._map = board_map

    @property
    def map(self):
        return self._map

    @property
    def castles(self):
        return {
            PColor.BLK: self._map[bu.castle_slices[PColor.BLK]],
            PColor.RED: self._map[bu.castle_slices[PColor.RED]]
        }

    def is_occupied(self, space: Union[List[int], Tuple[int, int]]):
        return self._map[space[0], space[1]] != 0

    def get_type(self, space: Union[List[int], Tuple[int, int]]) -> int:
        return abs(self._map[space[0], space[1]])

    def get_color(self, space: Union[List[int], Tuple[int, int]]):
        return cbu.run_get_color(
            rank=space[0],
            file=space[1],
            board_map=self._map)

        # piece_val = self._map[space[0], space[1]]
        # return 0 if piece_val == 0 else int(math.copysign(1, piece_val))

    def get_piece_info(
            self, space: Union[List[int], Tuple[int, int]]) -> Dict:
        return {
            "piece_type": self.get_type(space),
            "color": self.get_color(space)
        }

    def get_occupant(self, space: Union[List[int], Tuple[int, int]]):
        return self._map[space[0], space[1]]

    def set_occupant(self, space: Union[List[int], Tuple[int, int]],
                     piece: int):
        self._map[space[0], space[1]] = piece

    def exists_and_passes_color_test(
            self,
            dest: Union[List[int], Tuple[int, int]],
            moving_piece_color: int):
        return bu.is_on_board(dest) and (
                self.get_color(dest) != moving_piece_color)

    def execute_move(self, move: Dict[str, Tuple[int, int]]):
        moving_piece = self.get_occupant(move["start"])
        destination_piece = self.get_occupant(move["end"])
        self.set_occupant(space=move["end"], piece=moving_piece)
        self.set_occupant(space=move["start"], piece=PType.NUL)
        pieces = {
            "moving_piece": moving_piece,
            "destination_piece": destination_piece
        }
        return {**move, **pieces}

    def undo_move(
            self,
            prev_move: Dict[str, Any]):
        self.set_occupant(
            space=prev_move["start"],
            piece=prev_move["moving_piece"])
        self.set_occupant(
            space=prev_move["end"],
            piece=prev_move["destination_piece"])

    def get_all_spaces_occupied_by(
            self, color: int) -> List[Tuple]:

        return cbu.run_get_all_spaces_occupied_by(self._map, color)
        # spaces_array = np.argwhere(bu.is_color[color](self._map))
        # return list(map(tuple, spaces_array))

    def get_general_position(self, color: int) -> Tuple[int, int]:

        general_space = cbu.run_get_general_position(
            color=color,
            board_map=self._map)

        # general_space = None
        # for space in blo.castle_coords[color]:
        #     if abs(self._map[space]) == PType.GEN:
        #         general_space = space
        if general_space is None:
            raise GeneralNotFound(self._map)
        return general_space

    # def get_vertical_path(self, from_space: Tuple[int, int], to_rank: int):
    #
    #     if from_space[0] == to_rank:
    #         return np.array([])
    #     elif to_rank > from_space[0]:
    #         return self._map[(from_space[0] + 1):to_rank, from_space[1]]
    #     else:
    #         return self._map[(to_rank + 1):from_space[0], from_space[1]]

    def search_spaces(
            self,
            from_space: Tuple[int, int],
            direction: Tuple[int, int]):
        return cbu.run_search_space(
            board_map=self._map,
            start_rank=from_space[0],
            start_file=from_space[1],
            dir_rank=direction[0],
            dir_file=direction[1]
        )

        # empty_spaces = []
        # first_occupied_space = None
        # next_step = bu.space_plus_vect(from_space, direction)
        #
        # while bu.is_on_board(next_step) and (
        #         self.get_occupant(next_step) == PType.NUL):
        #     empty_spaces.append(next_step)
        #     next_step = bu.space_plus_vect(next_step, direction)
        # if bu.is_on_board(next_step):
        #     first_occupied_space = next_step
        # return {
        #     "empty_spaces": empty_spaces,
        #     "first_occupied_space": first_occupied_space}

    def soldier_moves(
            self, from_position: Tuple[int, int], color: int) -> List[Dict]:
        soldier_moves = []
        fwd_space = bu.space_plus_vect(from_position, bu.fwd_unit_vect(color))

        if self.exists_and_passes_color_test(fwd_space, color):
            soldier_moves.append({"start": from_position, "end": fwd_space})
        if not bu.is_in_homeland_of(color, from_position):
            for vector in bu.sideways_unit_vect:
                dest = bu.space_plus_vect(from_position, vector)
                if self.exists_and_passes_color_test(dest, color):
                    soldier_moves.append({"start": from_position, "end": dest})
        return soldier_moves

    def cannon_moves(
            self, from_position: Tuple[int, int], color: int) -> List[Dict]:
        cannon_moves = []
        search_directions = bu.all_orthogonal_unit_vects

        for direction in search_directions:
            search_results = self.search_spaces(from_position, direction)
            for empty_space in search_results["empty_spaces"]:
                cannon_moves.append(
                    {"start": from_position, "end": empty_space})

            if search_results["first_occupied_space"]:
                second_search = self.search_spaces(
                    search_results["first_occupied_space"], direction)
                if second_search["first_occupied_space"] and (
                        self.get_color(second_search["first_occupied_space"])
                        == bu.opponent_of[color]):
                    cannon_moves.append({
                        "start": from_position,
                        "end": second_search["first_occupied_space"]})

        return cannon_moves

    def chariot_moves(
            self, from_position: Tuple[int, int], color: int) -> List[Dict]:
        chariot_moves = []
        search_directions = bu.all_orthogonal_unit_vects
        for direction in search_directions:
            search_results = self.search_spaces(from_position, direction)

            for empty_space in search_results["empty_spaces"]:
                chariot_moves.append({
                    "start": from_position, "end": empty_space})

            if search_results["first_occupied_space"] and (
                    self.get_color(search_results["first_occupied_space"])
                    == bu.opponent_of[color]):
                chariot_moves.append(
                    {"start": from_position,
                     "end": search_results["first_occupied_space"]})

        return chariot_moves

    def horse_moves(
            self, from_position: Tuple[int, int], color: int) -> List[Dict]:
        horse_moves = []

        for direction in bu.horse_paths.keys():
            first_step = bu.space_plus_vect(from_position, direction)
            if bu.is_on_board(first_step) and not self.is_occupied(first_step):
                second_steps = [
                    bu.space_plus_vect(first_step, second_step) for second_step
                    in bu.horse_paths[direction]
                ]

                for second_step in second_steps:
                    if self.exists_and_passes_color_test(second_step, color):
                        horse_moves.append(
                            {"start": from_position, "end": second_step})

        return horse_moves

    def elephant_moves(
            self, from_position: Tuple[int, int], color: int) -> List[Dict]:
        elephant_moves = []

        for direction in bu.diag_directions:
            first_step = bu.space_plus_vect(from_position, direction)
            if (bu.is_on_board(first_step) and not self.is_occupied(first_step)
                    and bu.is_in_homeland_of(color, first_step)):
                second_step = bu.space_plus_vect(first_step, direction)
                if (self.exists_and_passes_color_test(second_step, color)
                        and bu.is_in_homeland_of(color, second_step)):
                    elephant_moves.append(
                        {"start": from_position, "end": second_step})

        return elephant_moves

    def advisor_moves(
            self, from_position: Tuple[int, int], color: int) -> List[Dict]:
        advisor_moves = []

        for direction in bu.diag_directions:
            destination = bu.space_plus_vect(from_position, direction)
            if bu.is_in_castle_of(color, destination) and (
                    self.get_color(destination) != color):
                advisor_moves.append(
                    {"start": from_position, "end": destination})

        return advisor_moves

    def general_moves(
            self, from_position: Tuple[int, int], color: int) -> List[Dict]:

        return gm.run_general_moves(
            from_rank=from_position[0],
            from_file=from_position[1],
            board_map=self._map,
            color=color)

    piece_moves = {
        PType.SOL: soldier_moves,
        PType.CAN: cannon_moves,
        PType.CHA: chariot_moves,
        PType.HOR: horse_moves,
        PType.ELE: elephant_moves,
        PType.ADV: advisor_moves,
        PType.GEN: general_moves
    }

    def calc_temp_moves_from(self, position: Tuple[int, int]) -> List[Dict]:
        color = self.get_color(position)

        return self.piece_moves[self.get_type(position)](self, position, color)

    def calc_final_moves_from(self, position: Tuple[int, int]) -> List[Dict]:

        final_moves = []
        temp_moves = self.calc_temp_moves_from(position)

        for move in temp_moves:
            test_move = self.execute_move(move)
            resulting_opp_moves = self.calc_temp_moves_of(
                bu.opponent_of[bu.get_piece_color(test_move["moving_piece"])])
            resulting_opp_destinations = {
                move["end"] for move in resulting_opp_moves}
            if self.get_general_position(
                    bu.get_piece_color(test_move["moving_piece"])
            ) not in resulting_opp_destinations:
                final_moves.append(move)
            self.undo_move(test_move)

        return final_moves

    def calc_temp_moves_of(self, color: int) -> List[Dict]:
        team_moves = []
        for space in self.get_all_spaces_occupied_by(color):
            team_moves = team_moves + self.calc_temp_moves_from(space)
        return team_moves

    # def calc_final_moves_from(self, position: BoardSpace):
    #     potential_moves = self.calc_temp_moves_from(position)
    #     moves_resulting_in_check = set()
    #
    #     for move in potential_moves:
    #         test_move = self.execute_move(move)
    #         resulting_opp_moves = self.calc_temp_moves_of(
    #             bu.opponent_of[bu.get_piece_color(test_move.moving_piece)])
    #         resulting_opp_destinations = {
    #             move.end for move in resulting_opp_moves}
    #         if self.get_general_position(
    #                 bu.get_piece_color(test_move.moving_piece)
    #         ) in resulting_opp_destinations:
    #             moves_resulting_in_check.add(move)
    #         self.undo_move(test_move)
    #
    #     legal_moves = potential_moves - moves_resulting_in_check
    #
    #     return legal_moves

    def calc_final_moves_of(self, color: int) -> List[Dict]:
        final_moves = []
        for space in self.get_all_spaces_occupied_by(color):
            final_moves = final_moves + self.calc_final_moves_from(space)
        return final_moves


class GeneralNotFound(Exception):
    def __init__(
            self,
            board_map: np.array,
            msg="General not found"):
        self._msg = msg
        self._board_map = board_map

    def __str__(self):
        return f"{self._msg}\n\n{self._board_map}"
