import abc
import cProfile
import json
import pkgutil
import time
import random
from typing import Set, List, Dict

import numpy as np
from xiangqigame.move import Move

from xiangqigame.board_rules import BOARD_RULES as br
from xiangqigame.enums import PieceColor
from xiangqigame.game_board import GameBoard
from xiangqigame.minimax_evaluators import BestMoves, MinimaxEvaluator, \
    PiecePoints
import xiangqigame.piece_points as pts


class MoveSelector(abc.ABC):

    @abc.abstractmethod
    def select_move(
            self,
            game_board: GameBoard,
            cur_player: int,
            cur_moves: List[Dict]
    ) -> Dict:
        pass


class RandomMoveSelector(MoveSelector):

    def select_move(
            self,
            game_board: GameBoard,
            cur_player: PieceColor,
            cur_moves: List[Dict]
    ) -> Dict:
        return random.choice(tuple(cur_moves))


class MinimaxMoveSelector(MoveSelector):

    def __init__(self, evaluator: MinimaxEvaluator, search_depth: int):
        self._evaluator = evaluator
        self._search_depth = search_depth
        self._node_counter = 0

    def _reset_node_counter(self):
        self._node_counter = 0

    def _minimax_rec(
            self,
            game_board: GameBoard,
            search_depth: int,
            alpha: int,
            beta: int,
            cur_player: PieceColor,
            initiating_player: PieceColor):
        self._node_counter += 1
        cur_moves = game_board.calc_final_moves_of(color=cur_player)
        if cur_moves == set():
            return self._evaluator.evaluate_winner(
                cur_player, initiating_player)

        if search_depth == 0:
            return self._evaluator.evaluate_leaf(
                game_board=game_board,
                cur_player=cur_player,
                cur_player_moves=cur_moves,
                initiating_player=initiating_player)

        if cur_player == initiating_player:
            max_eval = -np.inf
            best_moves = set()
            ranked_moves = self._evaluator.generate_ranked_move_list(
                game_board=game_board,
                cur_player=cur_player,
                cur_player_moves=cur_moves)
            for rated_move in ranked_moves:
                executed_move = game_board.execute_move(rated_move.move)
                cur_eval = self._minimax_rec(
                    game_board=game_board,
                    search_depth=search_depth - 1,
                    alpha=alpha,
                    beta=beta,
                    cur_player=br.opponent_of[initiating_player],
                    initiating_player=initiating_player,
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
            ranked_moves = self._evaluator.generate_ranked_move_list(
                game_board=game_board,
                cur_player=cur_player,
                cur_player_moves=cur_moves)
            for rated_move in ranked_moves:
                executed_move = game_board.execute_move(rated_move.move)
                cur_eval = self._minimax_rec(
                    game_board=game_board,
                    search_depth=search_depth - 1,
                    alpha=alpha,
                    beta=beta,
                    cur_player=initiating_player,
                    initiating_player=initiating_player,
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

    def select_move(
            self,
            game_board: GameBoard,
            cur_player: PieceColor,
            cur_moves: List[Dict]):
        self._reset_node_counter()
        minimax_result = self._minimax_rec(
            game_board=game_board,
            search_depth=self._search_depth,
            alpha=-np.inf,
            beta=np.inf,
            cur_player=cur_player,
            initiating_player=cur_player)

        return random.choice(list(minimax_result.best_moves))


if __name__ == "__main__":
    game_config = json.loads(
        pkgutil.get_data('xiangqigame.data', 'game_start.json'))
    cur_board = GameBoard(game_config['board_data'])
    base_pts = pts.BasePoints(piece_vals=pts.base_pts_icga_2004)
    position_pts = pts.PositionPts(pts_arrays_red=pts.position_points_icga_2004)
    cur_evaluator = PiecePoints(
        base_pts=base_pts, position_pts=position_pts, position_multiplier=1)

    move_selector = MinimaxMoveSelector(evaluator=cur_evaluator, search_depth=3)

    start = time.time()
    proposed_move = move_selector.select_move(
        game_board=cur_board, cur_player=PieceColor.BLACK, cur_moves=[])
    end = time.time()

    print(f"Selected ** move {proposed_move} in {end - start} seconds")
