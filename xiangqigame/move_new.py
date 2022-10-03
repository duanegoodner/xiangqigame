from typing import NamedTuple
from xiangqigame.board_components_new import BoardSpace


class Move(NamedTuple):
    start: BoardSpace
    end: BoardSpace


class ExecutedMove(NamedTuple):
    spaces: Move
    moving_piece: int
    destination_piece: int