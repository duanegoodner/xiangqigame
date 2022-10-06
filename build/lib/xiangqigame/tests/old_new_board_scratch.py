import cProfile
import json
import pkgutil
import pstats
import time
import xiangqigame.game_board as gb
import xiangqigame.game_board_new as gbn
import xiangqigame.game_board_no_enums as gb_ne

# game_config = json.loads(pkgutil.get_data("xiangqigame.data", "game_start.json"))
# old_board = gb.GameBoard(game_config["board_data"])
new_board = gbn.GameBoard()

# old_moves = old_board.calc_final_moves_of(gb.PieceColor.BLACK)
new_moves = new_board.calc_final_moves_of(gbn.PColor.BLK)
#
# old_board_blk_elephant = old_board.elephant_moves(
#     from_position=gb.BoardSpace(rank=0, file=2), color=gb.PieceColor.BLACK)

new_board_blk_elephant = new_board.elephant_moves(from_position=(0, 2), color=gbn.PColor.BLK)

print(new_board_blk_elephant)

