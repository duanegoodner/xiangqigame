import piece_encoding as pe


class GamePiece(object):

    def __init__(self, piece_type='NULL PIECE', color='NULL COLOR'):
        self._piece_type = piece_type
        self._color = color

    @classmethod
    def from_piece_code(cls, piece_code):
        return cls(piece_type=pe.decode_piece_type(piece_code),
                   color=pe.decode_color(piece_code))

    def __repr__(self):
        return repr(pe.encode_piece(self._piece_type, self._color))

    def __eq__(self, other):
        return self._piece_type, self._color == \
               other.get_piece_type(), other.get_color()

    def is_null_piece(self):
        return self._piece_type == 'NULL PIECE'

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
