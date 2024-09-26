import numpy as np
import xiangqigame.move_translator as mt
from typing import List, Tuple

from xiangqigame.enums import PlayerType, EvaluatorType

# from cpp_modules.src.pybind_modules.GameBoardPy import GameBoard, PieceColor, Move, MoveCollection
from xiangqigame.game_interfaces import Player
import xiangqigame.terminal_output as msg

from xiangqigame_core import (
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
        self.propose_move(game_board, cur_moves)


class ScriptedPlayer(Player):

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
        self.propose_move(game_board, cur_moves)


class AIPlayer(Player):

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
            return self._move_evaluator.get_search_summaries()

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
    def __init__(
        self, move: Move, message="AI player proposed an illegal move"
    ):
        self._move = move
        self._msg = message

    def __str__(self):
        return f"From: {self._move.start.rank}, {self._move.end.file}; To: {self._move.end.rank}, {self._move.end.file}"
