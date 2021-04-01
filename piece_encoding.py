

_code_to_color = {
    'R': 'RED',
    'B': 'BLACK',
    '-': 'NULL COLOR'
}

_color_to_code = dict((color, code) for code, color in _code_to_color.items())

_code_to_type = {
    'r': 'CHARIOT',
    'h': 'HORSE',
    'e': 'ELEPHANT',
    'a': 'ADVISOR',
    'g': 'GENERAL',
    'c': 'CANNON',
    's': 'SOLDIER',
    '-': 'NULL PIECE'
}

_type_to_code = dict((color, code) for code, color in _code_to_type.items())


def decode_color(piece_code):
    return _code_to_color[piece_code[1]]


def decode_piece_type(piece_code):
    return _code_to_type[piece_code[0]]


def decode_piece(piece_code):
    return _code_to_type[piece_code[0]], _code_to_color[piece_code[1]]


def encode_piece(piece_type, color):
    return _type_to_code[piece_type] + _color_to_code[color]
