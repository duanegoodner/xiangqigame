import abc
from dataclasses import dataclass
from typing import List, Set, NamedTuple

from xiangqigame.enums import PieceColor
from xiangqigame.game_board import GameBoard
from xiangqigame.move import Move


class Player(abc.ABC):

    def __init__(
            self,
            color: PieceColor,
            move_log: List[Move] = None):
        self._color = color
        if move_log is None:
            move_log = []
        self._move_log = move_log

    @property
    def move_log(self):
        return self._move_log

    @abc.abstractmethod
    def propose_move(self, game_board: GameBoard, cur_moves: Set[Move]) -> Move:
        pass

    @abc.abstractmethod
    def illegal_move_notice_response(self, game_board: GameBoard,
                                     cur_moves: Set[Move]):
        pass

