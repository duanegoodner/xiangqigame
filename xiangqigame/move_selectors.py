import numpy as np
import random
from typing import Set

from xiangqigame.board_rules import BOARD_RULES as br
from xiangqigame.enums import PieceColor
from xiangqigame.game_board import GameBoard
from xiangqigame.move import Move


def random_move(cur_moves: Set[Move], *args, **kwargs) -> Move:
    return random.choice(tuple(cur_moves))


# def minimax(game_board: GameBoard, cur_moves: Set[Move], search_depth: int,
#             alpha: float, beta: float, maximizing_player: PieceColor):
#
#     minimizing_player = br.opponent_of[maximizing_player]
#
#      = {
#         maximizing_player:
#             game_board.calc_final_moves_of(color=maximizing_player),
#         minimizing_player:
#             game_board.calc_final_moves_of(color=minimizing_player)
#     }
#
#     if (search_depth == 0) or (
#             game_board.calc_final_moves_of(color=PieceColor.RED) == set()
#     ) or (game_board.calc_final_moves_of(color=PieceColor.BLACK) == set()):

