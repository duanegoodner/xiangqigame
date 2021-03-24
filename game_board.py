from board_space import BoardSpace
from game_piece import GamePiece


class GameBoard:
    def __init__(self, board_list):
        num_ranks = len(board_list)
        num_files = len(board_list[0])
        self._map = [[BoardSpace((row, col), GamePiece(board_list[row][col]))
                      for col in range(num_files)] for row in range(num_ranks)]
