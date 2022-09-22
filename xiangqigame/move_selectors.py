import abc
import cProfile
import json
import pkgutil
import time
import random
from typing import Set

import numpy as np
from xiangqigame.move import Move

from xiangqigame.board_rules import BOARD_RULES as br
from xiangqigame.enums import PieceColor
from xiangqigame.game_board import GameBoard
from xiangqigame.minimax_evaluators import BestMoves, MinimaxEvaluator, \
    PiecePoints
import xiangqigame.piece_points as pts


class MoveSelector(abc.ABC):

    @abc.abstractmethod
    def select_move(
            self,
            game_board: GameBoard,
            cur_player: PieceColor,
            cur_moves: Set[Move]
    ) -> Move:
        pass


