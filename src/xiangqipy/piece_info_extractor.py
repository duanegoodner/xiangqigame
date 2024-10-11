"""
No longer used, but keep for reference: PieceReader for converting int to
PieceType.
"""

import math


class PieceReader:
    """
    Converts primitive integer to piece type and color.
    """

    @staticmethod
    def get_type(piece: int):
        return abs(piece)

    @staticmethod
    def get_color(piece: int):
        return 0 if piece == 0 else int(math.copysign(1, piece))

    def get_piece_info(self, piece: int):
        return {
            "piece_type": self.get_type(piece),
            "color": self.get_color(piece),
        }


PIECE_READER = PieceReader()
