import cProfile
import json
import pkgutil
import time

import numpy as np
from typing import Set, NamedTuple
from xiangqigame.board_rules import BOARD_RULES as br
from xiangqigame.enums import PieceColor
from xiangqigame.game_board import GameBoard
from xiangqigame.minimax_evaluators import BestMoves, MinimaxEvaluator, \
    MoveCounts, PiecePoints
from xiangqigame.move import Move

# class BestMoves(NamedTuple):
#     best_eval: int
#     best_moves: Set[Move]


node_counter = 0


def minimax(
        game_board: GameBoard,
        search_depth: int,
        alpha: int,
        beta: int,
        cur_player: PieceColor,
        initiating_player: PieceColor,
        evaluator: MinimaxEvaluator
):
    global node_counter
    node_counter += 1
    cur_moves = game_board.calc_final_moves_of(color=cur_player)
    if cur_moves == set():
        return evaluator.evaluate_winner(cur_player, initiating_player)
        # if cur_player == initiating_player:
        #     return BestMoves(best_eval=-np.inf, best_moves=set())
        # else:
        #     return BestMoves(best_eval=np.inf, best_moves=set())

    if search_depth == 0:
        return evaluator.evaluate_leaf(
            game_board=game_board,
            cur_player=cur_player,
            cur_player_moves=cur_moves,
            initiating_player=initiating_player)

        # opponent_moves = game_board.calc_final_moves_of(
        #     color=br.opponent_of[cur_player])
        # if cur_player == initiating_player:
        #     return BestMoves(best_eval=len(cur_moves) - len(opponent_moves),
        #                      best_moves=set())
        # else:
        #     return BestMoves(best_eval=len(opponent_moves) - len(cur_moves),
        #                      best_moves=set())

    if cur_player == initiating_player:
        max_eval = -np.inf
        best_moves = set()
        ranked_moves = evaluator.generate_ranked_move_list(
            game_board=game_board,
            cur_player=cur_player,
            cur_player_moves=cur_moves)
        for rated_move in ranked_moves:
            executed_move = game_board.execute_move(rated_move.move)
            cur_eval = minimax(
                game_board=game_board,
                search_depth=search_depth - 1,
                alpha=alpha,
                beta=beta,
                cur_player=br.opponent_of[initiating_player],
                initiating_player=initiating_player,
                evaluator=evaluator
            ).best_eval
            if cur_eval == max_eval:
                best_moves.add(rated_move.move)
            elif cur_eval > max_eval:
                max_eval = cur_eval
                best_moves.clear()
                best_moves.add(rated_move.move)
            game_board.undo_move(executed_move)
            alpha = max(alpha, cur_eval)
            if beta <= alpha:
                break
        return BestMoves(best_eval=max_eval, best_moves=best_moves)

    else:
        min_eval = np.inf
        best_moves = set()
        ranked_moves = evaluator.generate_ranked_move_list(
            game_board=game_board,
            cur_player=cur_player,
            cur_player_moves=cur_moves)
        for rated_move in ranked_moves:
            executed_move = game_board.execute_move(rated_move.move)
            cur_eval = minimax(
                game_board=game_board,
                search_depth=search_depth - 1,
                alpha=alpha,
                beta=beta,
                cur_player=initiating_player,
                initiating_player=initiating_player,
                evaluator=evaluator
            ).best_eval
            if cur_eval == min_eval:
                best_moves.add(rated_move)
            elif cur_eval < min_eval:
                min_eval = cur_eval
                best_moves.clear()
                best_moves.add(rated_move)
            game_board.undo_move(executed_move)
            beta = min(beta, cur_eval)
            if beta <= alpha:
                break
        return BestMoves(best_eval=min_eval, best_moves=best_moves)


game_config = json.loads(
    pkgutil.get_data('xiangqigame.data', 'game_start.json'))
cur_board = GameBoard(game_config['board_data'])
start = time.time()
cur_evaluator = PiecePoints(position_multiplier=1)
result = minimax(game_board=cur_board, search_depth=4, alpha=-np.inf,
                 beta=np.inf, cur_player=PieceColor.RED,
                 initiating_player=PieceColor.RED,
                 evaluator=cur_evaluator)
end = time.time()

print(f"Visited {node_counter} nodes in {end - start} seconds")
print(result)
