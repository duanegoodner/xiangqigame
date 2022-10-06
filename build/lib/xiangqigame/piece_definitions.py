from typing import NamedTuple


class PieceType(NamedTuple):
    NUL: int = 0
    GEN: int = 1
    ADV: int = 2
    ELE: int = 3
    HOR: int = 4
    CHA: int = 5
    CAN: int = 6
    SOL: int = 7


PType = PieceType()


class PieceColor(NamedTuple):
    RED: int = -1
    NUL: int = 0
    BLK: int = 1


PColor = PieceColor()
