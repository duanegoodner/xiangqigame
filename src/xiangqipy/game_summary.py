"""
@file game_summary.py

GameSummary class and its component classes.
"""

from dataclasses import dataclass
from typing import Dict, List

import msgspec
import xiangqi_bindings as bindings

import xiangqipy.core_dataclass_mirrors as cdm
from xiangqipy.enums import GameState
from xiangqipy.player_summary import PlayerSummary


@dataclass
class PlayerSummaries:
    """
    A data container for holding onle PlayerSummary for each player in a Game.
    """
    kRed: PlayerSummary
    kBlk: PlayerSummary


class GameSummary(msgspec.Struct):
    """
    Holds summary info of a Game; implement msgspec.Struct for json IO.

    Attributes are all either primitives or from core_data_class_mirrors to
    minimize need for enc_hook's in msgspec IO.
    """
    game_id: str
    game_state: GameState
    whose_turn: bindings.PieceColor
    move_log: List[cdm.ExecutedMove]
    player_summaries: PlayerSummaries

    @property
    def move_counts(self) -> int:
        return len(self.move_log)

    @property
    def move_numbers(self) -> Dict[bindings.PieceColor, List[int]]:
        return {
            bindings.PieceColor.kBlk: [
                val for val in range(1, self.move_counts + 1) if (val % 2) == 0
            ],
            bindings.PieceColor.kRed: [
                val for val in range(self.move_counts + 1) if (val % 2) == 1
            ],
        }

    def get_player_summary(self, player: bindings.PieceColor) -> PlayerSummary:
        return self.player_summaries.__dict__[player.name]
