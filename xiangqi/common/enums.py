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
    ILLEGAL_AUTO_MOVE = auto()


class Out(Enum):
    INPUT_PROMPT = auto()
    INVALID_INPUT = auto()
    ILLEGAL_MOVE = auto()
    ILLEGAL_AUTO_MOVE = auto()
    TURN = auto()
    WON_GAME = auto()
    IN_CHECK = auto()
    WHITESPACE = auto()



