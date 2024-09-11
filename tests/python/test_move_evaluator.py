import datetime
import pytest
from xiangqigame_core import GameBoard, MinimaxMoveEvaluator64, PieceColor


@pytest.fixture
def starting_board():
    return GameBoard()


def test_red_get_search_summaries(starting_board):
    search_depth = 3

    move_evaluator = MinimaxMoveEvaluator64(
        evaluating_player=PieceColor.kRed,
        starting_search_depth=search_depth,
        game_board=starting_board,
    )

    # run move_evaluator.select_move() to generate and store data in 
    # move_evaluator.search_summaries_
    proposed_move = move_evaluator.select_move()
    search_summaries = move_evaluator.get_search_summaries()
    
    num_move_selections = len(search_summaries.first_searches)
    assert num_move_selections == 1
    
    num_extra_searches = len(search_summaries.extra_searches)
    assert 0 <= num_extra_searches <= 1
    
    search_time = search_summaries.first_searches[0].time
    assert search_time > datetime.timedelta(0.)
    
    zobrist_key_size = move_evaluator.zobrist_key_size_bits()
    assert zobrist_key_size == 64
