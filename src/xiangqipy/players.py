"""
@file players.py

Implementations of Player, and Exceptions for notifying specific Player types
of illegal proposed Move.

Human Player does not have an associated Exception - just prompt for new entry.
"""

import numpy as np
import xiangqipy.move_translator as mt
from typing import List, Tuple

from xiangqipy.enums import PlayerType, EvaluatorType

from xiangqipy.game_interfaces import Player
import xiangqipy.terminal_output as msg

from xiangqi_bindings import (
    GameBoard,
    PieceColor,
    Move,
    MoveCollection,
    MinimaxMoveEvaluator64,
    MinimaxMoveEvaluator128,
    RandomMoveEvaluator,
    SearchSummaries,
)


class HumanPlayer(Player):
    """
    Uses terminal UI to propose moves.
    """

    def __init__(self, color: PieceColor, player_type: PlayerType) -> None:
        super().__init__(color=color, player_type=player_type)
        self._input_req = msg.InputRetrievalMessages()

    def propose_move(
        self, game_board: GameBoard, cur_moves: List[Move]
    ) -> Move:
        valid_input = None

        while not valid_input:
            user_input = input(self._input_req.input_prompt)
            parsed_input = mt.parse_input(user_input)
            if mt.is_valid_algebraic_pair(parsed_input):
                valid_input = parsed_input
            else:
                self._input_req.notify_invalid_input()

        proposed_move = mt.convert_parsed_input_to_move(
            parsed_input=valid_input
        )
        return proposed_move

    def illegal_move_notice_response(
        self, illegal_move: Move, game_board: GameBoard, cur_moves: List[Move]
    ):
        self._input_req.notify_illegal_move()
        # self.propose_move(game_board, cur_moves)


class ScriptedPlayer(Player):
    """
    Proposes moves sequentially from a list of algebraic notation moves.
    """

    def __init__(self, color: PieceColor, move_list: List[str]):
        super().__init__(color=color, player_type=PlayerType.SCRIPTED)
        self._move_list = move_list
        self._move_index = 0

    def propose_move(
        self, game_board: GameBoard, cur_moves: List[Move]
    ) -> Move:
        # time.sleep(0.2)
        parsed_input = mt.parse_input(self._move_list[self._move_index])
        if not mt.is_valid_algebraic_pair(parsed_input):
            raise InvalidEntryInMoveList(self._move_list[self._move_index])
        self._move_index += 1
        move = mt.convert_parsed_input_to_move(parsed_input=parsed_input)
        return move

    def illegal_move_notice_response(
        self,
        illegal_move: Tuple[str],
        game_board: GameBoard,
        cur_moves: List[Move],
    ):
        raise IllegalMoveInMoveList(illegal_move, game_board.map)


class ScriptedPlayerWithRetries(Player):
    """
    Has intentionally illegal move in list, followed by legal move (for tests).
    """
    def __init__(self, color: PieceColor, move_list: List[str]):
        super().__init__(color=color, player_type=PlayerType.SCRIPTED)
        self._move_list = move_list
        self._move_index = 0
        self._input_req = msg.InputRetrievalMessages()

    def propose_move(
        self, game_board: GameBoard, cur_moves: List[Move]
    ) -> Move:
        # time.sleep(0.2)
        parsed_input = mt.parse_input(self._move_list[self._move_index])
        if not mt.is_valid_algebraic_pair(parsed_input):
            raise InvalidEntryInMoveList(self._move_list[self._move_index])
        self._move_index += 1
        move = mt.convert_parsed_input_to_move(parsed_input=parsed_input)
        return move

    def illegal_move_notice_response(
        self, illegal_move: Move, game_board: GameBoard, cur_moves: List[Move]
    ):
        self._input_req.notify_illegal_move()
        # self.propose_move(game_board, cur_moves)


class AIPlayer(Player):
    """
    Proposed moves selected using an implementation of core MoveEvaluator.
    """

    def __init__(
        self,
        color: PieceColor,
        player_type: PlayerType,
        evaluator_type: EvaluatorType,
        move_evaluator: (
            MinimaxMoveEvaluator64
            | MinimaxMoveEvaluator128
            | RandomMoveEvaluator
        ),
    ):
        super().__init__(
            color=color, player_type=player_type, evaluator_type=evaluator_type
        )
        self._move_evaluator = move_evaluator

    @property
    def move_selection_summary(self) -> SearchSummaries | None:
        if type(self._move_evaluator).__name__ in [
            "MinimaxMoveEvaluator64",
            "MinimaxMoveEvaluator128",
        ]:
            return self._move_evaluator.search_summaries

    def propose_move(
        self, game_board: GameBoard, cur_moves: MoveCollection
    ) -> Move:
        proposed_move = self._move_evaluator.select_move(
            # game_board=game_board, piece_color=self._color
        )
        return proposed_move

    def illegal_move_notice_response(
        self, illegal_move: Move, game_board: GameBoard, cur_moves: List[Move]
    ):
        raise IllegalAIMove(illegal_move)


class InvalidEntryInMoveList(Exception):
    """
    Raised when scripted player proposed move has invalid syntax.
    """
    def __init__(
        self,
        algebraic_move_input: str,
        message="Invalid value for proposed move ",
    ):
        self._algebraic_move_input = algebraic_move_input
        self._msg = message

    def __str__(self):
        return f"{self._algebraic_move_input} -> {self._msg}"


class IllegalMoveInMoveList(Exception):
    """
    Raised when scripted player proposes an illegal move.
    """
    def __init__(
        self,
        board_map: np.array,
        move: Tuple[str],
        message="Illegal move in list provided by scripted player",
    ):
        self._move = move
        self._msg = message
        self._board_map = board_map

    def __str__(self):
        return f"{self._move} -> {self._msg}\n{self._board_map}"


class IllegalAIMove(Exception):
    """
    Raised when AI player proposed illegal move.
    """
    def __init__(
        self, move: Move, message="AI player proposed an illegal move"
    ):
        self._move = move
        self._msg = message

    def __str__(self):
        return f"From: {self._move.start.rank}, {self._move.end.file}; To: {self._move.end.rank}, {self._move.end.file}"
