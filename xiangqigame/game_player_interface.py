import random
import re
from typing import List, Set, Tuple
from xiangqigame.board_components import BoardSpace
from xiangqigame.game_board import GameBoard
from xiangqigame.game_interfaces import Player
import xiangqigame.user_io.messages as msg
from xiangqigame.enums import PieceColor, Out
from xiangqigame.move import Move


class AlgebraicMoveInterpreter:

    @staticmethod
    def parse_input(player_input: str) -> List[str]:
        first_step = player_input.strip().split(',')
        return [item.strip() for item in first_step]

    @staticmethod
    def is_valid_algebraic_pair(parsed_input: List[str]) -> bool:
        return len(parsed_input) == 2 and all([
            re.fullmatch('^[a-i](?:10|[1-9])$', item) for item in parsed_input
        ])

    @staticmethod
    def algebraic_space_to_boardspace(algebraic_space: str):
        alg_column = algebraic_space[0]
        file = ord(alg_column) - ord('a')

        alg_row = algebraic_space[1:]
        rank = int(alg_row) - 1

        return BoardSpace(rank=rank, file=file)

    def convert_parsed_input_to_move(self, parsed_input: List[str]):

        board_space_from_to = [
            self.algebraic_space_to_boardspace(entry) for entry in parsed_input]
        return Move(start=board_space_from_to[0], end=board_space_from_to[1])


class HumanPlayer(Player):

    def __init__(
            self,
            color: PieceColor,
            move_interpreter: AlgebraicMoveInterpreter,
            move_log: List[Move] = None):
        super().__init__(color, move_log)
        self._move_interpreter = move_interpreter

    def propose_move(self, game_board: GameBoard, cur_moves: Set[Move]) -> Move:
        valid_input = None

        while not valid_input:
            user_input = input(msg.messages[Out.INPUT_PROMPT])
            parsed_input = self._move_interpreter.parse_input(user_input)
            if self._move_interpreter.is_valid_algebraic_pair(parsed_input):
                valid_input = parsed_input
            else:
                msg.output(Out.INVALID_INPUT)

        proposed_move = self._move_interpreter.convert_parsed_input_to_move(
            parsed_input=valid_input)
        self._move_log.append(proposed_move)
        return proposed_move

    def illegal_move_notice_response(self, game_board: GameBoard, cur_moves: Set[Move]):
        self._move_log.pop()
        self.propose_move(game_board, cur_moves)


class ScriptedPlayer(Player):

    def __init__(self,
                 color: PieceColor,
                 move_interpreter: AlgebraicMoveInterpreter,
                 move_list: List[str],
                 move_log: List[Tuple[str]] = None):
        super().__init__(color, move_log)
        self._move_list = move_list
        self._move_interpreter = move_interpreter
        self._move_index = 0

    def propose_move(self, game_board: GameBoard, moves: Set[Move]) -> Move:
        parsed_input = self._move_interpreter.parse_input(
            self._move_list[self._move_index])
        if not self._move_interpreter.is_valid_algebraic_pair(parsed_input):
            raise InvalidEntryInMoveList(self._move_list[self._move_index])
        self._move_index += 1
        self._move_log.append(parsed_input)
        return self._move_interpreter.convert_valid_alg_spaces_to_move(
            valid_parsed_input=parsed_input)

    def illegal_move_notice_response(self, game_board: GameBoard, moves: Set[Move]):
        invalid_move = self._move_log.pop()
        raise IllegalMoveInMoveList(invalid_move)


class AIPlayer(Player):

    def __init__(self, color: PieceColor, move_log: List[Move] = None):
        super().__init__(color, move_log)

    def propose_move(self, game_board: GameBoard, cur_moves: Set[Move]) -> Move:

        proposed_move = random.choice(tuple(cur_moves))
        self._move_log.append(proposed_move)
        return random.choice(proposed_move)

    def illegal_move_notice_response(self, game_board: GameBoard, cur_moves: Set[Move]):
        invalid_move = self._move_log.pop()
        raise IllegalAIMove(invalid_move)


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





