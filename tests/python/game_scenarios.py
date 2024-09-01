from cpp_modules.src.pybind_modules.GameBoardPy import PieceColor
from xiangqigame.enums import GameState
# from xiangqigame.piece_definitions import PColor
from xiangqigame.game import Game
from xiangqigame.players import ScriptedPlayer
from xiangqigame.data import alg_games as ag
from xiangqigame.data import game_a as g_a

import unittest


class GameResultTests(unittest.TestCase):

    def test_a(self):
        # moves_a = convert_alg_move_list(ag.game_a)
        moves_a = g_a.game_a
        red_moves = moves_a[::2]
        black_moves = moves_a[1::2]
        red_player = ScriptedPlayer(color=PieceColor.kRed, move_list=red_moves)
        black_player = ScriptedPlayer(
            color=PieceColor.kBlk,
            move_list=black_moves
        )
        game_a = Game(red_player=red_player, black_player=black_player)
        game_a.play()
        self.assertTrue(game_a._game_state == GameState.RED_WON)

    # def test_b(self):
    #     moves_b = convert_alg_move_list(ag.game_b)
    #     game_b = Game(self.game_config, scripted_moves=moves_b)
    #     game_b.play_scripted_moves()
    #     self.assertTrue(game_b._game_state == GameState.ILLEGAL_AUTO_MOVE)
    #
    # def test_c(self):
    #     moves_c = convert_alg_move_list(ag.game_c)
    #     game_c = Game(self.game_config, scripted_moves=moves_c)
    #     game_c.play_scripted_moves()
    #     self.assertTrue(game_c._game_state == GameState.UNFINISHED)
    #
    # def test_d(self):
    #     moves_d = convert_alg_move_list(ag.game_d)
    #     game_d = Game(self.game_config, scripted_moves=moves_d)
    #     game_d.play_scripted_moves()
    #     self.assertTrue(game_d._game_state == GameState.RED_WON)


if __name__ == '__main__':
    unittest.main()












