import math

import numpy as np
import random
from typing import Set, NamedTuple

from xiangqigame.board_rules import BOARD_RULES as br
from xiangqigame.enums import PieceColor
from xiangqigame.game_board import GameBoard
from xiangqigame.move import Move


def random_move(cur_moves: Set[Move], *args, **kwargs) -> Move:
    return random.choice(tuple(cur_moves))


