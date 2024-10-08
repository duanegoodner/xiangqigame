from typing import List, Callable
from xiangqigame_core import GameBoard, PieceColor
from xiangqigame.enums import GameState
from xiangqigame.game import Game
from xiangqigame.players import (
    Player,
    ScriptedPlayer,
    ScriptedPlayerWithRetries,
)

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

    orig_red_moves, orig_black_moves = (
        scripted_moves[::2],
        scripted_moves[1::2],
    )

    @staticmethod
    def play_game(
        red_moves: List[str],
        black_moves: List[str],
        red_player_constructor: Callable[..., Player],
        black_player_constructor: Callable[..., Player],
    ) -> Game:
        game_board = GameBoard()
        red_player = red_player_constructor(
            color=PieceColor.kRed, move_list=red_moves
        )
        black_player = black_player_constructor(
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
            red_moves=self.orig_red_moves,
            black_moves=self.orig_black_moves,
            red_player_constructor=ScriptedPlayer,
            black_player_constructor=ScriptedPlayer,
        )
        self.assertTrue(game_a._game_state == GameState.RED_WON)

    def test_scripted_game_with_retry_allowed(self):
        modified_red_moves = ["a1, a1"] + self.orig_red_moves
        game_b = self.play_game(
            red_moves=modified_red_moves,
            black_moves=self.orig_black_moves,
            red_player_constructor=ScriptedPlayerWithRetries,
            black_player_constructor=ScriptedPlayer,
        )
        self.assertTrue(game_b._game_state == GameState.RED_WON)


if __name__ == "__main__":
    unittest.main()
