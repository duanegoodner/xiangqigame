from typing import List
from xiangqigame_core import GameBoard, PieceColor
from xiangqigame.enums import GameState
from xiangqigame.game import Game
from xiangqigame.players import ScriptedPlayer

import unittest


class ScriptedGameTests(unittest.TestCase):

    scripted_moves = [
        "a4, a5",
        "a7, a6",
        "a5, a6",
        "b8, b6",
        "a6, b6",
        "c7, c6",
        "b6, c6",
        "c10, a8",
        "e4, e5",
        "e7, e6",
        "e5, e6",
        "h10, g8",
        "g1, e3",
        "g8, e7",
        "e6, e7",
        "g10, i8",
        "a1, a8",
        "e10, e9",
        "a8, e8",
        "e9, f9",
        "i1, i2",
        "f10, e9",
        "e8, e9",
        "f9, f8",
        "i2, f2",
    ]

    @staticmethod
    def play_game(move_list: List[str]) -> Game:
        red_moves, black_moves = move_list[::2], move_list[1::2]
        game_board = GameBoard()
        red_player = ScriptedPlayer(color=PieceColor.kRed, move_list=red_moves)
        black_player = ScriptedPlayer(
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
        game_a = self.play_game(move_list=self.scripted_moves)
        self.assertTrue(game_a._game_state == GameState.RED_WON)


if __name__ == "__main__":
    unittest.main()
