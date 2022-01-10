from enum import Enum, auto


class PieceColor(Enum):
    RED = auto()
    BLACK = auto()
    NULL_COLOR = auto()


class PieceType(Enum):
    CHARIOT = auto()
    HORSE = auto()
    ELEPHANT = auto()
    ADVISOR = auto()
    GENERAL = auto()
    CANNON = auto()
    SOLDIER = auto()
    NULL_PIECE = auto()


class GameState(Enum):
    UNFINISHED = auto()
    RED_WON = auto()
    BLACK_WON = auto()



