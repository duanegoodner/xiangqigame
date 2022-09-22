import random
import time
import xiangqigame.move_translator as mt
from typing import List, Set, Tuple, Callable
from xiangqigame.game_board import GameBoard
from xiangqigame.game_interfaces import Player
import xiangqigame.terminal_output as msg
from xiangqigame.enums import PieceColor
from xiangqigame.move import Move


class HumanPlayer(Player):

    def __init__(self, color: PieceColor):
        super().__init__(color)
        self._input_req = msg.InputRetrievalMessages()

    def propose_move(self, game_board: GameBoard, cur_moves: Set[Move]) -> Move:
        valid_input = None

        while not valid_input:
            user_input = input(self._input_req.input_prompt)
            parsed_input = mt.parse_input(user_input)
            if mt.is_valid_algebraic_pair(parsed_input):
                valid_input = parsed_input
            else:
                self._input_req.notify_invalid_input()

        proposed_move = mt.convert_parsed_input_to_move(
            parsed_input=valid_input)
        return proposed_move

    def illegal_move_notice_response(
            self,
            illegal_move: Move,
            game_board: GameBoard,
            cur_moves: Set[Move]):
        self._input_req.notify_illegal_move()
        self.propose_move(game_board, cur_moves)


class ScriptedPlayer(Player):

    def __init__(self,
                 color: PieceColor,
                 move_list: List[str]):
        super().__init__(color)
        self._move_list = move_list
        self._move_index = 0

    def propose_move(self, game_board: GameBoard, cur_moves: Set[Move]) -> Move:
        # time.sleep(0.2)
        parsed_input = mt.parse_input(
            self._move_list[self._move_index])
        if not mt.is_valid_algebraic_pair(parsed_input):
            raise InvalidEntryInMoveList(self._move_list[self._move_index])
        self._move_index += 1
        move = mt.convert_parsed_input_to_move(parsed_input=parsed_input)
        return move

    def illegal_move_notice_response(
            self,
            illegal_move: Move,
            game_board: GameBoard,
            cur_moves: Set[Move]):
        raise IllegalMoveInMoveList(illegal_move)


class AIPlayer(Player):

    def __init__(
            self,
            color: PieceColor,
            move_selector: Callable[..., Move]):
        super().__init__(color)
        self._move_selector = move_selector

    def propose_move(self, game_board: GameBoard) -> Move:
        proposed_move = self._move_selector(
            game_board=game_board, color=self._color)
        return proposed_move

    def illegal_move_notice_response(
            self,
            illegal_move: Move,
            game_board: GameBoard,
            cur_moves: Set[Move]):
        raise IllegalAIMove(illegal_move)


class InvalidEntryInMoveList(Exception):
    def __init__(
            self,
            algebraic_move_input: str,
            message="Invalid value for proposed move "):
        self._algebraic_move_input = algebraic_move_input
        self._msg = message

    def __str__(self):
        return f"{self._algebraic_move_input} -> {self._msg}"


class IllegalMoveInMoveList(Exception):
    def __init__(
            self,
            move: Tuple[str],
            message="Illegal move in list provided by scripted player"):
        self._move = move
        self._msg = message

    def __str__(self):
        return f"{self._move} -> {self._msg}"


class IllegalAIMove(Exception):
    def __init__(
            self,
            move: Move,
            message="AI player proposed an illegal move"):
        self._move = move
        self._msg = message

    def __str__(self):
        return f"{self._move} -> {self._msg}"
