from typing import List
from xiangqigame_core import GameBoard, PieceColor
from xiangqigame.enums import GameState
from xiangqigame.game import Game
from xiangqigame.players import ScriptedPlayer, ScriptedPlayerWithRetries

import unittest


class ScriptedGameTests(unittest.TestCase):

    red_test_moves = ["a1, a1", "b1, c3", "b1, c3"]

    black_test_moves = ["a7, a6"]

    @staticmethod
    def play_game(red_moves: List[str], black_moves: List[str]) -> Game:
        # red_moves, black_moves = move_list[::2], move_list[1::2]
        game_board = GameBoard()
        red_player = ScriptedPlayerWithRetries(
            color=PieceColor.kRed, move_list=red_moves
        )
        black_player = ScriptedPlayerWithRetries(
            color=PieceColor.kBlk, move_list=black_moves
        )
        game = Game(
            players={
                PieceColor.kRed: red_player,
                PieceColor.kBlk: black_player,
            },
            game_board=game_board,
        )
        game.play()
        return game

    def test_scripted_game(self):
        game_a = self.play_game(
            red_moves=self.red_test_moves, black_moves=self.black_test_moves
        )

        self.assertTrue(game_a._game_state == GameState.RED_WON)


if __name__ == "__main__":
    unittest.main()
