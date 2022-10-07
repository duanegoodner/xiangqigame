import xiangqigame.cython_modules.cython_board_builder as cbb
import xiangqigame.starting_board_builder as bb


np_board = bb.StartingBoardBuilder().build_initial_board_array()

cbb.modify(np_board)

