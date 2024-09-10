import pytest
from xiangqigame_core import GameBoard, MinimaxMoveEvaluator, PieceColor



@pytest.fixture
def starting_board():
    return GameBoard()

def test_red_get_search_summaries(starting_board):
    search_depth = 3

    move_evaluator = MinimaxMoveEvaluator(
        evaluating_player=PieceColor.kRed,
        starting_search_depth=search_depth,
        game_board=starting_board,
    )
    
    proposed_move = move_evaluator.select_move()
    move_summaries = move_evaluator.get_search_summaries()
    
    assert True
