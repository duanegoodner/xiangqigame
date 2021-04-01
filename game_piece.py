import piece_encoding as pe
import move_rules as move_rules


class GamePiece(object):

    def __init__(self, piece_code):
        self._piece_code = piece_code
        self._piece_type = pe.decode_piece_type(piece_code)
        self._color = pe.decode_color(piece_code)

    def __repr__(self):
        return repr(self._piece_code)

    def __eq__(self, other):
        return self._piece_type, self._color == \
               other.get_piece_type(), other.get_color()

    def is_not_null_piece(self):
        return self._piece_type != 'NULL PIECE'

    def get_piece_type(self):
        return self._piece_type

    def get_color(self):
        return self._color

    def get_opp_color(self):
        if self._color == 'RED':
            return 'BLACK'
        if self._color == 'BLACK':
            return 'RED'

    def get_forward_direction(self):
        if self.is_not_null_piece():
            return move_rules.get_forward_direction(self._color)

    def is_piece(self, piece_type, color):
        return self._color == color and self._piece_type == piece_type
