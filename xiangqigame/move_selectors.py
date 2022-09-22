import math

import numpy as np
import random
from typing import Set, NamedTuple

from xiangqigame.board_rules import BOARD_RULES as br
from xiangqigame.enums import PieceColor
from xiangqigame.game_board import GameBoard
from xiangqigame.move import Move


def random_move(cur_moves: Set[Move], *args, **kwargs) -> Move:
    return random.choice(tuple(cur_moves))


class BestMoves(NamedTuple):
    best_eval: float
    best_moves: Set[Move]


def minimax_red(game_board: GameBoard, search_depth: int,
                cur_player: PieceColor):
    cur_moves = game_board.calc_final_moves_of(color=cur_player)
    if cur_moves == set():
        if cur_player == PieceColor.RED:
            return BestMoves(best_eval=-np.inf, best_moves=set())
        else:
            return BestMoves(best_eval=np.inf, best_moves=set())

    if search_depth == 0:
        opponent_moves = game_board.calc_final_moves_of(color=br.opponent_of[cur_player])
        if cur_player == PieceColor.RED:
            return BestMoves(best_eval=len(cur_moves) - len(opponent_moves),
                             best_moves=set())
        else:
            return BestMoves(best_eval=len(opponent_moves) - len(cur_moves),
                             best_moves=set())

    if cur_player == PieceColor.RED:
        max_eval = -np.inf
        best_moves = set()
        for move in cur_moves:
            executed_move = game_board.execute_move(move)
            cur_eval = minimax_red(
                game_board=game_board,
                search_depth=search_depth - 1,
                cur_player=PieceColor.BLACK).best_eval
            if math.isclose(cur_eval, max_eval):
                best_moves.add(move)
            elif cur_eval > max_eval:
                max_eval = cur_eval
                best_moves.clear()
                best_moves.add(move)
            game_board.undo_move(executed_move)
        return BestMoves(best_eval=max_eval, best_moves=best_moves)

    else:
        min_eval = np.inf
        best_moves = set()
        for move in cur_moves:
            executed_move = game_board.execute_move(move)
            cur_eval = minimax_red(
                game_board=game_board,
                search_depth=search_depth - 1,
                cur_player=PieceColor.RED).best_eval
            if math.isclose(cur_eval, min_eval):
                best_moves.add(move)
            elif cur_eval < min_eval:
                min_eval = cur_eval
                best_moves.clear()
                best_moves.add(move)
            game_board.undo_move(executed_move)
        return BestMoves(best_eval=min_eval, best_moves=best_moves)


