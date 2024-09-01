"""Dev script for profiling. No longer works after migration from MoveSelector
to MoveEvaluator, but keep here to use as reference for other profiling work."""

import cProfile
import pstats
import time
from pstats import SortKey

import xiangqigame.piece_points as pts
from xiangqigame_core import GameBoard, MinimaxMoveEvaluator, PieceColor


def select_red_starting_move(search_depth: int):
    game_board = GameBoard()
    move_evaluator = MinimaxMoveEvaluator(
        evaluating_player=PieceColor.kRed,
        starting_search_depth=search_depth,
        game_board=game_board,
    )
    start = time.time()
    proposed_move = move_evaluator.select_move()
    end = time.time()
    print(f"Selected ** move {proposed_move} in {end - start} seconds")


if __name__ == "__main__":
    depth = 2
    # select_red_starting_move(depth)
    cProfile.run(
        statement=f"select_red_starting_move({depth})",
        filename=f"red_starting_move_depth_{depth}",
    )
    p = pstats.Stats(f"red_starting_move_depth_{depth}")
    p.sort_stats(SortKey.CUMULATIVE).print_stats()
