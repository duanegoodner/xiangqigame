"""Dev script for profiling. No longer works after migration from MoveSelector
to MoveEvaluator, but keep here to use as reference for other profiling work."""

import cProfile
import pstats
import time
from pstats import SortKey

import xiangqigame.piece_points as pts
from xiangqigame_core import (
    GameBoard,
    MinimaxMoveEvaluator64,
    MinimaxMoveEvaluator128,
    PieceColor,
)


def select_red_starting_move(
    search_depth: int,
    move_evaluator: MinimaxMoveEvaluator64 | MinimaxMoveEvaluator128,
):
    game_board = GameBoard()
    move_evaluator = move_evaluator(
        evaluating_player=PieceColor.kRed,
        starting_search_depth=search_depth,
        game_board=game_board,
    )
    start = time.time()
    proposed_move = move_evaluator.select_move()
    end = time.time()
    print(f"Selected ** move {proposed_move} in {end - start} seconds")


if __name__ == "__main__":
    depth = 4
    cProfile.run(
        statement=f"select_red_starting_move({depth}, MinimaxMoveEvaluator64)",
        filename=f"red_starting_move_64bit_depth_{depth}",
    )
    p = pstats.Stats(f"red_starting_move_64bit_depth_{depth}")
    p.sort_stats(SortKey.CUMULATIVE).print_stats()
    
    cProfile.run(
        statement=f"select_red_starting_move({depth}, MinimaxMoveEvaluator128)",
        filename=f"red_starting_move_128bit_depth_{depth}",
    )
    p = pstats.Stats(f"red_starting_move_128bit_depth_{depth}")
    p.sort_stats(SortKey.CUMULATIVE).print_stats()
    
    
    
    
