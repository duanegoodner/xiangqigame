"""
Classes for terminal UI output including board representation and, messages
requesting info, and status messages.
"""

import os
import colorama as cr
import subprocess
from dataclasses import dataclass
from typing import Dict

import xiangqipy.move_translator as mt
from xiangqi_bindings import (
    GameBoard,
    GamePiece,
    Move,
    opponent_of,
    PieceColor,
    PieceType,
)
from xiangqipy.game_interfaces import GameStatusReporter
from xiangqipy.enums import GameState, EvaluatorType
from xiangqipy.player_summary import PlayerSummary


@dataclass
class InputRetrievalMessages:
    """
    Provides messages requesting input from a Player.
    """
    input_prompt: str = "Enter a move in the form 'from_space, to_space': "
    invalid_input_msg: str = "Invalid input"
    illegal_move_msg: str = "Illegal move. Please enter a different move."

    def notify_invalid_input(self):
        print(self.invalid_input_msg)

    def notify_illegal_move(self):
        print(self.illegal_move_msg)


class TerminalStatusReporter(GameStatusReporter):
    """
    Outputs a text-base game board with GamePiece locations in algebraic
    notation.
    """

    _disp_format = {
        PieceColor.kRed: cr.Style.BRIGHT + cr.Fore.WHITE + cr.Back.RED,
        PieceColor.kBlk: cr.Style.BRIGHT + cr.Fore.BLACK + cr.Back.WHITE,
        PieceColor.kNul: cr.Fore.RESET + cr.Back.RESET,
    }

    _display_team_name = {PieceColor.kRed: "Red", PieceColor.kBlk: "Black"}

    _color_to_code = {
        PieceColor.kRed: "r",
        PieceColor.kBlk: "b",
        PieceColor.kNul: "-",
    }

    _type_to_code = {
        PieceType.kCha: "R",
        PieceType.kHor: "H",
        PieceType.kEle: "E",
        PieceType.kAdv: "A",
        PieceType.kGen: "G",
        PieceType.kCan: "C",
        PieceType.kSol: "S",
        PieceType.kNnn: "-",
    }

    @staticmethod
    def clear_screen():
        _ = subprocess.call("clear" if os.name == "posix" else "cls")

    def encode_piece(self, piece: GamePiece):
        return (
            f"{self._disp_format[piece.piece_color]}"
            f"{self._type_to_code[piece.piece_type]}"
            f"{self._color_to_code[piece.piece_color]}"
            f"{cr.Style.RESET_ALL}"
        )

    def format_board_output(self, board: GameBoard):
        file_labels = [
            f" {chr(char_num)}  " for char_num in range(ord("a"), ord("j"))
        ]
        file_labels.insert(0, "\t")
        file_labels.insert(len(file_labels), "\n")

        board_list = [
            [
                f" {self.encode_piece(board.map()[row][col])} "
                for col in range(len(board.map()[0]))
            ]
            for row in range(len(board.map()))
        ]
        for row_index in range(len(board_list)):
            board_list[row_index].insert(0, f" {str(10 - row_index)}\t")
        board_list.insert(0, file_labels)
        board_list = ["".join(row) for row in board_list]

        return str("\n\n".join([str(rank) for rank in board_list]))

    @staticmethod
    def move_evaluator_translator(
        player_summary: PlayerSummary,
    ) -> Dict[str, str]:
        display_dispatch = {
            "Minimax": f"Minimax, max search depth = "
            f"{player_summary.max_search_depth}, zobrist hash key size = "
            f"{player_summary.zobrist_key_size} bits",
            "Random": "Random",
        }

        return display_dispatch

    def display_player_info(self, player_summary: PlayerSummary):

        player_type_string = f"Player Type = {player_summary.player_type.name}"
        move_evaluator_string = (
            f", Move Evaluator = {player_summary.move_evaluator_type.name}"
            if player_summary.move_evaluator_type != EvaluatorType.NULL
            else ""
        )
        search_depth_string = (
            f", Max Search Depth = {player_summary.max_search_depth}"
            if player_summary.max_search_depth
            else ""
        )

        zobrist_key_size_string = (
            f", Zobrist Key Size = {player_summary.zobrist_key_size}"
            if player_summary.zobrist_key_size
            else ""
        )

        print(
            f"\n{self._display_team_name[player_summary.color]} Player:\n"
            f"{player_type_string}{move_evaluator_string}{search_depth_string}{zobrist_key_size_string}"
        )

    def display_prev_move(self, color: PieceColor, prev_move: Move = None):
        if prev_move:
            print(
                f"Most recent move:\n"
                f"{mt.convert_move_to_input_str(prev_move)} "
                f"({self._display_team_name[color]})\n"
            )
        else:
            print("Most recent move:\n" "NA... No moves executed yet.\n")

    def display_whose_turn(self, color: PieceColor):
        print(f"Whose turn:\n{self._display_team_name[color]}\n")

    def display_if_is_in_check(self, color: PieceColor, is_in_check: bool):
        if is_in_check:
            print(f"{self._display_team_name[color]} is in check.")

    def display_final_move(self, color: PieceColor, final_move: Move):
        print(
            f"Final move:\n"
            f"{mt.convert_move_to_input_str(final_move)} "
            f"({self._display_team_name[color]})\n"
        )

    @staticmethod
    def display_winner(game_state: GameState):
        if game_state == GameState.RED_WON:
            print("Red won the game.")
        if game_state == GameState.BLACK_WON:
            print("Black won the game.")

    def report_game_info(
        self,
        red_player_summary: PlayerSummary,
        black_player_summary: PlayerSummary,
        game_state: GameState,
        game_board: GameBoard,
        whose_turn: PieceColor,
        is_in_check: bool,
        move_count: int,
        prev_move: Move = None,
    ):
        self.clear_screen()
        print(f"{self.format_board_output(game_board)}\n")
        self.display_player_info(player_summary=red_player_summary)
        self.display_player_info(player_summary=black_player_summary)
        print(f"\nMove count: {move_count}\n")

        if game_state == GameState.UNFINISHED:
            self.display_prev_move(
                color=opponent_of(whose_turn), prev_move=prev_move
            )
            self.display_whose_turn(whose_turn)
            self.display_if_is_in_check(
                color=whose_turn, is_in_check=is_in_check
            )

        else:
            self.display_final_move(
                color=opponent_of(whose_turn), final_move=prev_move
            )
            self.display_winner(game_state)
