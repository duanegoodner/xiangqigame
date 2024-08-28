import abc
from xiangqigame_cpp.xiangqigame_core import GameBoard, Move, PieceColor
from typing import List
from xiangqigame.enums import GameState


class Player(abc.ABC):

    def __init__(
            self,
            color: PieceColor):
        self._color = color

    @abc.abstractmethod
    def propose_move(
            self, game_board: GameBoard, cur_moves: List[Move]) -> Move:
        pass

    @abc.abstractmethod
    def illegal_move_notice_response(
            self, illegal_move: Move,
            game_board: GameBoard,
            cur_moves: List[Move]):
        pass


class GameStatusReporter(abc.ABC):

    @abc.abstractmethod
    def report_game_info(
            self,
            game_state: GameState,
            game_board: GameBoard,
            whose_turn: PieceColor,
            is_in_check: bool,
            move_count: int,
            prev_move: Move = None):
        pass
