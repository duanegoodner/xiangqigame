from typing import Dict
from common.enums import PieceColor, PieceType
from board.game_piece import GamePiece


_code_to_color = {
    'R': PieceColor.RED,
    'B': PieceColor.BLACK,
    '-': PieceColor.NULL_COLOR
}

_color_to_code = dict((color, code) for code, color in
                      _code_to_color.items())

_code_to_type = {
    'r': PieceType.CHARIOT,
    'h': PieceType.HORSE,
    'e': PieceType.ELEPHANT,
    'a': PieceType.ADVISOR,
    'g': PieceType.GENERAL,
    'c': PieceType.CANNON,
    's': PieceType.SOLDIER,
    '-': PieceType.NULL_PIECE
}

_type_to_code = dict(
    (color, code) for code, color in _code_to_type.items())


def decode_color(piece_code):
    return _code_to_color[piece_code[1]]


def decode_piece_type(piece_code):
    return _code_to_type[piece_code[0]]


def decode_piece(piece_code):
    decoded_piece: GamePiece = {
        'type': _code_to_type[piece_code[0]],
        'color': _code_to_color[piece_code[1]]
    }
    return decoded_piece


def encode_piece(decoded_piece: Dict):
    return _type_to_code[decoded_piece['type']] +\
           _color_to_code[decoded_piece['color']]







