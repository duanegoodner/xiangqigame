import numpy as np


board_space = np.dtype([('rank', np.int8), ('file', np.int8)])

move = np.dtype([('start', board_space), ('end', board_space)])

game_piece = np.dtype([('piece', np.int8)])


game_board = np.zeros(shape=(10, 9), dtype=game_piece)

my_spaces = np.empty(2, dtype=board_space)


