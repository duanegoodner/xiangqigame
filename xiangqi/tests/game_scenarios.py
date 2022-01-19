import json
from xiangqi.common import GameState
from xiangqi.game import Game
from xiangqi.data import alg_games as ag
from xiangqi.user_io import convert_alg_move_list

import unittest

with open('../data/game_start.json') as game_info:
    game_config = json.load(game_info)


class GameResultTests(unittest.TestCase):
    def test_a(self):
        moves_a = convert_alg_move_list(ag.game_a)
        game_a = Game(game_config, auto_moves=moves_a)
        game_a.play_auto_moves()
        self.assertTrue(game_a._game_state == GameState.RED_WON)

    def test_b(self):
        moves_b = convert_alg_move_list(ag.game_b)
        game_b = Game(game_config, auto_moves=moves_b)
        game_b.play_auto_moves()
        self.assertTrue(game_b._game_state == GameState.ILLEGAL_AUTO_MOVE)

    def test_c(self):
        moves_c = convert_alg_move_list(ag.game_c)
        game_c = Game(game_config, auto_moves=moves_c)
        game_c.play_auto_moves()
        self.assertTrue(game_c._game_state == GameState.UNFINISHED)

    def test_d(self):
        moves_d = convert_alg_move_list(ag.game_d)
        game_d = Game(game_config, auto_moves=moves_d)
        game_d.play_auto_moves()
        self.assertTrue(game_d._game_state == GameState.RED_WON)


if __name__ == '__main__':
    unittest.main()










