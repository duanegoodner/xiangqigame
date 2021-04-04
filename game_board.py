from game_piece import GamePiece
import numpy as np


class GameBoard:

    # _move_rules = {
    #     'CHARIOT': move_rules.chariot,
    #     'HORSE': move_rules.horse,
    #     'ELEPHANT': move_rules.elephant,
    #     'ADVISOR': move_rules.advisor,
    #     'general': move_rules.general,
    #     'CANNON': move_rules.cannon,
    #     'SOLDIER': move_rules.soldier,
    # }

    def __init__(self, board_list):
        self._num_ranks = len(board_list)
        self._num_files = len(board_list[0])
        self._map = [[GamePiece(board_list[row][col]) for col
                      in range(self._num_files)] for row in
                     range(self._num_ranks)]
        self._castle_red = [(row, col + 3) for row in range(3) for col in
                            range(3)]
        self._castle_black = [(row + 7, col + 3) for row in range(3) for col in
                              range(3)]
        self._re_red = 4
        self._re_black = 5

    def get_map(self):
        return self._map

    def is_on_board(self, space):
        return all(index > 0 for index in space) and \
               space[0] < self._num_ranks and space[1] < self._num_files

    def get_occupant(self, board_space):
        return self._map[board_space[0]][board_space[1]]

    def get_piece_info(self, board_space):
        return board_space, self.get_occupant(board_space)

    def get_all_pieces_of(self, color):
        num_ranks = len(self._map)
        num_files = len(self._map[0])

        return {(row, col): self.get_occupant((row, col)) for col in
                range(num_files) for row in range(num_ranks)
                if self.get_occupant((row, col)).get_color() == color}

    def get_castle(self, color):
        if color == 'RED':
            return self._castle_red
        if color == 'BLACK':
            return self._castle_black

    def is_in_homeland_of(self, board_space, color):
        if color == 'RED':
            return board_space[1] <= self._re_red
        if color == 'BLACK':
            return board_space[1] >= self._re_black

    def get_general_position(self, color):
        for space in self.get_castle(color):
            if self.get_occupant(space).get_piece_type() == 'general':
                return space

    def get_vertical_path(self, from_space, to_rank):
        file = from_space[1]
        low_rank = min(from_space[0], to_rank)
        high_rank = max(from_space[0], to_rank)
        return np.asarray(self._map)[low_rank:high_rank, file].tolist()

    def get_adjacent_spaces(self, space):
        unit_deltas = (0, 1), (0, -1), (1, 0), (-1, 0)
        return {tuple(np.add(space, delta)) for delta in unit_deltas
                if self.is_on_board(tuple(np.add(space, delta)))}






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
