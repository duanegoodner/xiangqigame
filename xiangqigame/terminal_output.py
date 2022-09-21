import os
import subprocess

import xiangqigame.move_translator as mt
from xiangqigame.board_rules import BOARD_RULES as br
from xiangqigame.move import Move
from xiangqigame.enums import PieceColor, Out, GameState
from xiangqigame.game_board import GameBoard

messages = {
    Out.INPUT_PROMPT: "Enter a move in the form 'from_space, to_space': ",
    Out.INVALID_INPUT: 'Invalid input.',
    Out.ILLEGAL_MOVE: 'Illegal move. Please enter a different move.',
    Out.WHITESPACE: '\n',
}


class TerminalStatusReporter:

    _display_team_name = {
        PieceColor.RED: "Red",
        PieceColor.BLACK: "Black"
    }

    @staticmethod
    def clear_screen():
        _ = subprocess.call('clear' if os.name == 'posix' else 'cls')

    def display_prev_move(self, color: PieceColor, prev_move: Move = None):
        if prev_move:
            print(f"Most recent move:\n"
                  f"{mt.convert_move_to_input_str(prev_move)} "
                  f"({self._display_team_name[color]})\n")
        else:
            print("Most recent move:\n"
                  "NA... No moves executed yet.\n")

    def display_whose_turn(self, color: PieceColor):
        print(f"Whose turn:\n{self._display_team_name[color]}\n")

    def display_if_is_in_check(self, color: PieceColor, is_in_check: bool):
        if is_in_check:
            print(f"{self._display_team_name[color]} is in check.")

    def display_final_move(self, color: PieceColor, final_move: Move):
        print(f"Final move:\n"
              f"{mt.convert_move_to_input_str(final_move)} "
              f"({self._display_team_name[color]})\n")

    @staticmethod
    def display_winner(game_state: GameState):
        if game_state == GameState.RED_WON:
            print("Red won the game.")
        if game_state == GameState.BLACK_WON:
            print("Black won the game.")

    def display_game_info(
            self, game_state: GameState,
            game_board: GameBoard,
            whose_turn: PieceColor,
            is_in_check: bool,
            prev_move: Move = None):
        self.clear_screen()
        print(f"{game_board}\n")

        if game_state == GameState.UNFINISHED:
            self.display_prev_move(
                color=br.opponent_of[whose_turn],
                prev_move=prev_move)
            self.display_whose_turn(whose_turn)
            self.display_if_is_in_check(
                color=whose_turn, is_in_check=is_in_check)

        else:
            self.display_final_move(color=br.opponent_of[whose_turn],
                                    final_move=prev_move)
            self.display_winner(game_state)


# display_team_name = {
#     PieceColor.RED: "Red",
#     PieceColor.BLACK: "Black"
# }
#
#
# def clear_screen():
#     _ = subprocess.call('clear' if os.name == 'posix' else 'cls')
#
#
# def output(*args):
#     msg_parts = [messages[arg] for arg in args]
#     message = ''.join(msg_parts)
#     print(message)
#
#
# def display_board(game_board: GameBoard):
#     print(f"{game_board}\n")
#
#
# def display_prev_move(color: PieceColor, prev_move: Move = None):
#     if prev_move:
#         print(f"Most recent move:\n"
#               f"{mt.convert_move_to_input_str(prev_move)} "
#               f"({display_team_name[color]})\n")
#     else:
#         print("Most recent move:\n"
#               "NA... No moves executed yet.\n")
#
#
# def display_whose_turn(color: PieceColor):
#     print(f"Whose turn:\n{display_team_name[color]}\n")
#
#
# def display_pre_move_info(
#         board: GameBoard,
#         whose_turn: PieceColor,
#         is_in_check: bool,
#         prev_move: Move = None):
#     clear_screen()
#     display_board(board)
#     display_prev_move(color=br.opponent_of[whose_turn], prev_move=prev_move)
#     display_whose_turn(whose_turn)
#     if is_in_check:
#         print(f"{display_team_name[whose_turn]} is in check.")
#
#
# def display_winner(game_state: GameState):
#     if game_state == GameState.RED_WON:
#         print("Red won the game.")
#     if game_state == GameState.BLACK_WON:
#         print("Black won the game.")
#
#
# def display_post_game_info(
#         game_state: GameState,
#         game_board: GameBoard,
#         final_move: Move):
#     clear_screen()
#     display_board(game_board)
#     print(f"Final move:\n"
#           f"{mt.convert_move_to_input_str(final_move)}\n")
#     display_winner(game_state)
