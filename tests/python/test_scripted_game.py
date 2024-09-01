from typing import List
from xiangqigame_core import GameBoard, PieceColor
from xiangqigame.enums import GameState
from xiangqigame.game import Game
from xiangqigame.players import ScriptedPlayer

import unittest


class ScriptedGameTests(unittest.TestCase):

    scripted_moves = [
        "a7, a6",
        "a4, a5",
        "a6, a5",
        "b3, b5",
        "a5, b5",
        "c4, c5",
        "b5, c5",
        "c1, a3",
        "e7, e6",
        "e4, e5",
        "e6, e5",
        "h1, g3",
        "g10, e8",
        "g3, e4",
        "e5, e4",
        "g1, i3",
        "a10, a3",
        "e1, e2",
        "a3, e3",
        "e2, f2",
        "i10, i9",
        "f1, e2",
        "e3, e2",
        "f2, f3",
        "i9, f9",
    ]

    @staticmethod
    def play_game(move_list: List[str]) -> Game:
        red_moves, black_moves = move_list[::2], move_list[1::2]
        game_board = GameBoard()
        red_player = ScriptedPlayer(color=PieceColor.kRed, move_list=red_moves)
        black_player = ScriptedPlayer(color=PieceColor.kBlk, move_list=black_moves)
        game = Game(
            players={PieceColor.kRed: red_player, PieceColor.kBlk: black_player},
            game_board=game_board,
        )
        game.play()
        return game

    def test_scripted_game(self):
        game_a = self.play_game(move_list=self.scripted_moves)
        self.assertTrue(game_a._game_state == GameState.RED_WON)


if __name__ == "__main__":
    unittest.main()
