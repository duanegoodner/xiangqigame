"""
@file enums.py

Enums that are only used on the Python side of the app.
"""

from enum import Enum, auto


class GameState(Enum):
    """
    Enum indicating state of game: is either unfinished, or a particular player has won.
    """
    UNFINISHED = auto()
    RED_WON = auto()
    BLACK_WON = auto()
    DRAW = auto()


class PlayerType(Enum):
    """
    Enum indicating type of player: Human, AI, or Scripted.
    Scripted players are primarily used for testing specific move sequences.
    """
    HUMAN = auto()
    AI = auto()
    SCRIPTED = auto()


class EvaluatorType(Enum):
    """
    Enum indicating type of core MoveEvaluator used for a Player.
    """
    NULL = auto()
    MINIMAX = auto()
    RANDOM = auto()
