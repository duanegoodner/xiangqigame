import math
from typing import NamedTuple


# class PieceType(NamedTuple):
#     NUL: int = 0
#     GEN: int = 1
#     ADV: int = 2
#     ELE: int = 3
#     HOR: int = 4
#     CHA: int = 5
#     CAN: int = 6
#     SOL: int = 7
#
#
# PType = PieceType()
#
#
# class PieceColor(NamedTuple):
#     RED: int = -1
#     NUL: int = 0
#     BLK: int = 1
#
#
# PColor = PieceColor()


class PieceDecoder:

    @staticmethod
    def get_type(piece: int):
        return abs(piece)

    @staticmethod
    def get_color(piece: int):
        return 0 if piece == 0 else int(math.copysign(1, piece))

    def get_piece_info(self, piece: int):
        return {
            "piece_type": self.get_type(piece),
            "color": self.get_color(piece)
        }


PIECE_DECODER = PieceDecoder()
