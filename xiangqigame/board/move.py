from typing import NamedTuple
from xiangqi_game.board.board_components import BoardSpace
from xiangqi_game.board.game_piece import GamePiece


class Move(NamedTuple):
    start: BoardSpace
    end: BoardSpace


class ExecutedMove(NamedTuple):
    spaces: Move
    moving_piece: GamePiece
    destination_piece: GamePiece







