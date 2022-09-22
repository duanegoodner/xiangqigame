from xiangqigame.enums import PieceColor
from xiangqigame.move_selectors import minimax_red
from .fixtures import starting_game_board


class TestMinimaxRed:

    def test_start_of_game(self, starting_game_board):
        result = minimax_red(
            game_board=starting_game_board,
            search_depth=1,
            cur_player=PieceColor.RED)

        print(f"\n****** {result }********")

