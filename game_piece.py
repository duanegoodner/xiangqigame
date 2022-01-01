from enums import PieceColor, PieceType
from piece_decoder import decode_piece, encode_piece, decode_piece_type,\
    decode_color


class GamePiece(object):

    def __init__(self, piece_color: PieceColor, piece_type: PieceType):
        self.piece_type = piece_type
        self.piece_color = piece_color

    @classmethod
    def from_piece_code(cls, piece_code):
        return cls(piece_color=decode_color(piece_code),
                   piece_type=decode_piece_type(piece_code))

    def __repr__(self):
        return repr(encode_piece(self.piece_type, self.piece_color))

    def get_piece_type(self):
        return self.piece_type

    def get_piece_color(self):
        return self.piece_color

    def is_null_piece(self):
        return self.piece_type == PieceType.NULL_PIECE
