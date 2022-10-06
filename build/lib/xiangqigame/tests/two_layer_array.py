import numpy as np
import time

import xiangqigame.game_board_new as gbn


def time_func_10k(func):
    def wrapper(*args, **kwargs):
        start = time.time()
        for i in range(10000):
            func(*args, **kwargs)
        end = time.time()

        return end - start

    return wrapper


board = gbn.GameBoard()

color_map = np.sign(board._map)
piece_map = abs(board._map)
piece_color_layer = np.zeros(shape=(2, 10, 9), dtype=int)
piece_color_layer[0, :, :] = color_map
piece_color_layer[1, :, :] = piece_map


@time_func_10k
def one_layer_move():
    moving_piece = board._map[0, 0]
    dest_piece = board._map[1, 0]
    board._map[1, 0] = board._map[0, 0]
    board._map[0, 0] = 0

    board._map[1, 0] = dest_piece
    board._map[0, 0] = moving_piece





@time_func_10k
def two_layer_move():
    moving_piece = piece_color_layer[:, 0, 0]
    dest_piece = piece_color_layer[:, 1, 0]

    piece_color_layer[:, 1, 0] = piece_color_layer[:, 0, 0]
    piece_color_layer[:, 0, 0] = 0

    piece_color_layer[:, 1, 0] = dest_piece
    piece_color_layer[:, 0, 0] = moving_piece



print(f"two_layer = {two_layer_move()}, "
      f"one_layer = {one_layer_move()},")



