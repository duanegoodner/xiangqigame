from enums import PieceColor, PieceType
from typing import TypedDict


class GamePiece(TypedDict):
    type: PieceType
    color: PieceColor


null_piece: GamePiece = {'type': PieceType.NULL_PIECE,
                         'color': PieceColor.NULL_COLOR}
