from board_space import BoardSpace
from game_piece import GamePiece


_forward_direction = {
    'RED': 1,
    'BLACK': -1
}


class GameBoard:
    def __init__(self, board_list):
        num_ranks = len(board_list)
        num_files = len(board_list[0])

        self._map = [[BoardSpace((row, col))
                      for col in range(num_files)] for row in range(num_ranks)]
        self._castle_red = [[row, col + 3] for row in range(3) for col in
                            range(3)]
        self._castle_black = [[row + 7, col + 3] for row in range(3) for col in
                              range(3)]
        self._re_red = 4
        self._re_black = 5
