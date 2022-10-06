import os
import colorama as cr
import subprocess
from dataclasses import dataclass
from typing import Dict

import xiangqigame.move_translator as mt
from xiangqigame.board_utilities_new import BOARD_UTILITIES as bu
from xiangqigame.game_interfaces import GameStatusReporter
from xiangqigame.enums import GameState
from xiangqigame.game_board_new import GameBoard
from xiangqigame.piece_definitions import PColor, PType
from xiangqigame.piece_definitions import PIECE_DECODER as pd


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

    _disp_format = {
        PColor.RED: cr.Fore.RED + cr.Back.WHITE,
        PColor.BLK: cr.Fore.BLACK + cr.Back.WHITE,
        PColor.NUL: cr.Fore.RESET + cr.Back.RESET
    }

    _display_team_name = {
        PColor.RED: "Red",
        PColor.BLK: "Black"
    }

    _color_to_code = {
        PColor.RED: "r",
        PColor.BLK: "b",
        PColor.NUL: "-"
    }

    _type_to_code = {
        PType.CHA: "R",
        PType.HOR: "H",
        PType.ELE: "E",
        PType.ADV: "A",
        PType.GEN: "G",
        PType.CAN: "C",
        PType.SOL: "S",
        PType.NUL: "-"
    }

    @staticmethod
    def clear_screen():
        _ = subprocess.call('clear' if os.name == 'posix' else 'cls')

    def encode_piece(self, piece_info: Dict):
        return (
            f"{self._disp_format[piece_info['color']]}"
            f"{self._type_to_code[piece_info['piece_type']]}"
            f"{self._color_to_code[piece_info['color']]}"
            f"{cr.Style.RESET_ALL}"
        )

    def format_board_output(self, board: GameBoard):
        file_labels = [
            f" {chr(char_num)}  " for char_num in range(ord("a"), ord("j"))
        ]
        file_labels.insert(0, '\t')
        file_labels.insert(len(file_labels), '\n')

        board_list = [
            [f" {self.encode_piece(pd.get_piece_info(board.map[row, col]))} "
             for col in range(len(board.map[0]))]
            for row in range(len(board.map))
        ]
        for row_index in range(len(board_list)):
            board_list[row_index].insert(0, f" {str(row_index + 1)}\t")
        board_list.insert(0, file_labels)
        board_list = ["".join(row) for row in board_list]

        return str('\n\n'.join([str(rank) for rank in board_list]))

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
        print(f"{self.format_board_output(game_board)}\n")
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
