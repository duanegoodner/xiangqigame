from enums import PieceColor, PieceType


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
    return _code_to_type[piece_code[0]],\
           _code_to_color[piece_code[1]]


def encode_piece(piece_type, piece_color):
    return _type_to_code[piece_type] + _color_to_code[piece_color]







