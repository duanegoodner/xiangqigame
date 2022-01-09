from enums import PieceColor, PieceType
from typing import TypedDict


class GamePiece(TypedDict):
    piece_type: PieceType
    piece_color: PieceColor
