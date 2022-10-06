import abc
from typing import List, Dict
from xiangqigame.enums import PieceColor, GameState
from xiangqigame.game_board import GameBoard


class Player(abc.ABC):

    def __init__(
            self,
            color: int):
        self._color = color

    @abc.abstractmethod
    def propose_move(self, game_board: GameBoard, cur_moves: List[Dict]) -> Dict:
        pass

    @abc.abstractmethod
    def illegal_move_notice_response(
            self, illegal_move: Dict,
            game_board: GameBoard,
            cur_moves: List[Dict]):
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
            prev_move: Dict = None):
        pass
