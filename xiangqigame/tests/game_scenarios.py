import json
import pkgutil
from xiangqigame.enums import GameState
from xiangqigame.game import Game
from xiangqigame.data import alg_games as ag
from xiangqigame.user_io.single_move import convert_alg_move_list

import unittest


class GameResultTests(unittest.TestCase):
    game_config = json.loads(
        pkgutil.get_data('xiangqigame.data', 'game_start.json'))

    def test_a(self):
        moves_a = convert_alg_move_list(ag.game_a)
        game_a = Game(self.game_config, scripted_moves=moves_a)
        game_a.play_scripted_moves()
        self.assertTrue(game_a._game_state == GameState.RED_WON)

    def test_b(self):
        moves_b = convert_alg_move_list(ag.game_b)
        game_b = Game(self.game_config, scripted_moves=moves_b)
        game_b.play_scripted_moves()
        self.assertTrue(game_b._game_state == GameState.ILLEGAL_AUTO_MOVE)

    def test_c(self):
        moves_c = convert_alg_move_list(ag.game_c)
        game_c = Game(self.game_config, scripted_moves=moves_c)
        game_c.play_scripted_moves()
        self.assertTrue(game_c._game_state == GameState.UNFINISHED)

    def test_d(self):
        moves_d = convert_alg_move_list(ag.game_d)
        game_d = Game(self.game_config, scripted_moves=moves_d)
        game_d.play_scripted_moves()
        self.assertTrue(game_d._game_state == GameState.RED_WON)


if __name__ == '__main__':
    unittest.main()












