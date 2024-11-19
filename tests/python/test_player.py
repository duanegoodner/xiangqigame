from unittest import mock

import pytest
from xiangqipy.players import AIPlayer, HumanPlayer
from xiangqi_bindings import GameBoard, PieceColor, RandomMoveEvaluator

# from xiangqigame_fixtures import starting_game_board


@pytest.fixture
def starting_game_board() -> GameBoard:
    return GameBoard()


class TestHumanPlayer:

    def test_human_init(self, starting_game_board):
        player = HumanPlayer(color=PieceColor.kRed)

        with mock.patch("builtins.input", return_value="a7, a6"):
            proposed_move = player.propose_move(
                game_board=starting_game_board,
                cur_moves=starting_game_board.CalcFinalMovesOf(PieceColor.kRed),
            )
        assert proposed_move.start.rank == 6
        assert proposed_move.start.file == 0
        assert proposed_move.end.rank == 5
        assert proposed_move.end.file == 0


class TestAIPlayer:

    def test_ai_game(self, starting_game_board):
        move_evaluator = RandomMoveEvaluator(
            evaluating_player=PieceColor.kRed, game_board=starting_game_board
        )
        player = AIPlayer(color=PieceColor.kRed, move_evaluator=move_evaluator)
        move = player.propose_move(
            game_board=starting_game_board,
            cur_moves=starting_game_board.CalcFinalMovesOf(PieceColor.kRed),
        )

        print(move)
