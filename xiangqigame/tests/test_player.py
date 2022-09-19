from unittest import mock
from xiangqigame.board_components import BoardSpace
from xiangqigame.enums import PieceColor
from xiangqigame.game_player_interface import HumanPlayer
from xiangqigame.move import Move
from xiangqigame.tests.fixtures import algebraic_move_interpreter, \
    starting_game_board


class TestHumanPlayer:

    def test_human_init(
            self,
            algebraic_move_interpreter,
            starting_game_board):
        player = HumanPlayer(
            color=PieceColor.RED,
            move_interpreter=algebraic_move_interpreter)

        with mock.patch("builtins.input", return_value="a4, a5"):
            proposed_move = player.propose_move(
                game_board=starting_game_board,
                cur_moves=
                starting_game_board.calc_final_moves_of(PieceColor.RED))
        assert proposed_move == Move(
            start=BoardSpace(rank=3, file=0),
            end=BoardSpace(rank=4, file=0))


