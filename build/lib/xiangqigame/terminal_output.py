import os
import subprocess
from dataclasses import dataclass
from typing import Dict

import xiangqigame.move_translator as mt
from xiangqigame.board_utilities_new import BOARD_UTILITIES as bu
from xiangqigame.game_interfaces import GameStatusReporter
from xiangqigame.enums import GameState
from xiangqigame.game_board import GameBoard
from xiangqigame.piece_definitions import PColor


@dataclass
class InputRetrievalMessages:
    input_prompt: str = "Enter a move in the form 'from_space, to_space': "
    invalid_input_msg: str = "Invalid input"
    illegal_move_msg: str = "Illegal move. Please enter a different move."

    def notify_invalid_input(self):
        print(self.invalid_input_msg)

    def notify_illegal_move(self):
        print(self.illegal_move_msg)


class TerminalStatusReporter(GameStatusReporter):

    _display_team_name = {
        PColor.RED: "Red",
        PColor.BLK: "Black"
    }

    @staticmethod
    def clear_screen():
        _ = subprocess.call('clear' if os.name == 'posix' else 'cls')

    def display_prev_move(self, color: int, prev_move: Dict = None):
        if prev_move:
            print(f"Most recent move:\n"
                  f"{mt.convert_move_to_input_str(prev_move)} "
                  f"({self._display_team_name[color]})\n")
        else:
            print("Most recent move:\n"
                  "NA... No moves executed yet.\n")

    def display_whose_turn(self, color: int):
        print(f"Whose turn:\n{self._display_team_name[color]}\n")

    def display_if_is_in_check(self, color: int, is_in_check: bool):
        if is_in_check:
            print(f"{self._display_team_name[color]} is in check.")

    def display_final_move(self, color: int, final_move: Dict):
        print(f"Final move:\n"
              f"{mt.convert_move_to_input_str(final_move)} "
              f"({self._display_team_name[color]})\n")

    @staticmethod
    def display_winner(game_state: GameState):
        if game_state == GameState.RED_WON:
            print("Red won the game.")
        if game_state == GameState.BLACK_WON:
            print("Black won the game.")

    def report_game_info(
            self, game_state: GameState,
            game_board: GameBoard,
            whose_turn: int,
            is_in_check: bool,
            move_count: int,
            prev_move: Dict = None):
        self.clear_screen()
        print(f"{game_board}\n")
        print(f"Move count: {move_count}\n")

        if game_state == GameState.UNFINISHED:
            self.display_prev_move(
                color=bu.opponent_of[whose_turn],
                prev_move=prev_move)
            self.display_whose_turn(whose_turn)
            self.display_if_is_in_check(
                color=whose_turn, is_in_check=is_in_check)

        else:
            self.display_final_move(color=bu.opponent_of[whose_turn],
                                    final_move=prev_move)
            self.display_winner(game_state)
