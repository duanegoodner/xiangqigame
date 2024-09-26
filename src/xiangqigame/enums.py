from enum import Enum, auto


class GameState(Enum):
    UNFINISHED = auto()
    RED_WON = auto()
    BLACK_WON = auto()


class PlayerType(Enum):
    HUMAN = auto()
    AI = auto()
    SCRIPTED = auto()


class EvaluatorType(Enum):
    NULL = auto()
    MINIMAX = auto()
    RANDOM = auto()
