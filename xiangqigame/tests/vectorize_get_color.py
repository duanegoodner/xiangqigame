import numpy as np
import xiangqigame.game_board_no_enums as gb_ne

board = gb_ne.GameBoard()
td_array = board._map_td

_index_array = np.array(
    [[(row, col) for col in range(9)] for row in
     range(10)],
    dtype=[("rank", int), ("file", int)])


def get_color(row: int, col: int):
    return td_array[row, col]["color"]


color_array = np.array([[get_color(row, col) for col in range(9)] for row in range(10)])

get_color_v = np.vectorize(get_color)

result = get_color_v([[0, 0, 0], [1, 1, 1]], [[0, 1, 2], [0, 1, 2]])
print(result)
