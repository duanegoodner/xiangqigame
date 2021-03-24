_color_codes = {
    'R': 'RED',
    'B': 'BLACK',
    '-': 'NO COLOR'
}

_type_codes = {
    'r': 'CHARIOT',
    'h': 'HORSE',
    'e': 'ELEPHANT',
    'a': 'ADVISOR',
    'g': 'GENERAL',
    'c': 'CANNON',
    's': 'SOLDIER',
    '-': 'EMPTY'
}


class GamePiece:

    def __init__(self, piece_code):
        self._piece_code = piece_code
        self._piece_type = _type_codes[piece_code[0]]
        self._color = _color_codes[piece_code[1]]

    def __repr__(self):
        return repr(self._piece_code)

    def get_piece_type(self):
        return self._piece_type

    def get_color(self):
        return self._color
