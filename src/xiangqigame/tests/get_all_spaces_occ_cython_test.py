from xiangqigame.game_board_new import GameBoard
from xiangqigame.cython_modules.get_all_spaces_occupied_by import\
    run_get_all_spaces_occupied_by

from xiangqigame.piece_definitions import PColor


board = GameBoard()
result = run_get_all_spaces_occupied_by(board._map, PColor.RED)
print(result)
