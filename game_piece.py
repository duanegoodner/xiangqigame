

_piece_code_to_color = {
    'R': 'RED',
    'B': 'BLACK',
    '-': 'NO COLOR'
}


_piece_color_to_code = dict((color, code) for code, color
                            in _piece_code_to_color.items())


_piece_code_to_type = {
    'r': 'CHARIOT',
    'h': 'HORSE',
    'e': 'ELEPHANT',
    'a': 'ADVISOR',
    'g': 'GENERAL',
    'c': 'CANNON',
    's': 'SOLDIER',
    '-': 'EMPTY'
}


_piece_type_to_code = dict((color, code) for code, color
                           in _piece_code_to_type.items())


class GamePiece:

    def __init__(self, piece_code):
        self._piece_code = piece_code
        self._piece_type = _piece_code_to_type[piece_code[0]]
        self._color = _piece_code_to_color[piece_code[1]]

    def __repr__(self):
        return repr(self._piece_code)

    def __eq__(self, other):
        return self._piece_type, self._color == \
               other.get_piece_type(), other.get_color()

    def get_piece_code(self):
        return self._piece_code

    def get_piece_type(self):
        return self._piece_type

    def get_color(self):
        return self._color

    def get_opp_color(self):
        if self._color == 'RED':
            return 'BLACK'
        if self._color == 'BLACK':
            return 'RED'

    def is_piece(self, piece_type, color):
        return self._color == color and self._piece_type == piece_type
