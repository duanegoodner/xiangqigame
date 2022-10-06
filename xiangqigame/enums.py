from enum import Enum, auto


class GameState(Enum):
    UNFINISHED = auto()
    RED_WON = auto()
    BLACK_WON = auto()
