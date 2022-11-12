from unittest import mock
from xiangqigame.board_components import BoardSpace
from xiangqigame.enums import PieceColor
from xiangqigame.players import AIPlayer, HumanPlayer
from xiangqigame.move import Move
from xiangqigame.tests.fixtures import starting_game_board


class TestHumanPlayer:

    def test_human_init(
            self,
            starting_game_board):
        player = HumanPlayer(color=PieceColor.RED)

        with mock.patch("builtins.input", return_value="a7, a6"):
            proposed_move = player.propose_move(
                game_board=starting_game_board,
                cur_moves=
                starting_game_board.calc_final_moves_of(PieceColor.RED))
        assert proposed_move == Move(
            start=BoardSpace(rank=6, file=0),
            end=BoardSpace(rank=5, file=0))


class TestAIPlayer:

    def test_ai_game(
            self,
            starting_game_board):
        player = AIPlayer(color=PieceColor.RED)
        move = player.propose_move(
            game_board=starting_game_board,
            cur_moves=
            starting_game_board.calc_final_moves_of(
                PieceColor.RED))

        print(move)
