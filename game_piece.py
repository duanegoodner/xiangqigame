from enums import PieceColor, PieceType
from typing import TypedDict
from collections import namedtuple
from utilities import get_size


class GamePiece(TypedDict):
    piece_type: PieceType
    piece_color: PieceColor


GamePieceTwo = namedtuple("GamePieceTwo", "piece_type piece_color")

class_game_piece: GamePiece = {'piece_type': PieceType.HORSE,
                               'piece_color': PieceColor.RED}

named_tuple_game_piece = GamePieceTwo(PieceType.HORSE, PieceColor.RED)

standard_tuple_game_piece = (PieceType.HORSE, PieceColor.RED)

print(get_size(class_game_piece))
print(get_size(named_tuple_game_piece))
print(get_size(standard_tuple_game_piece))
