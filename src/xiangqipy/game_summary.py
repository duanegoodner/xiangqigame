from dataclasses import dataclass
from typing import Dict, List

import msgspec
import xiangqi_bindings as bindings

import xiangqipy.core_dataclass_mirrors as cdm
from xiangqipy.enums import GameState
from xiangqipy.player_summary import PlayerSummary


@dataclass
class PlayerSummaries:
    kRed: PlayerSummary
    kBlk: PlayerSummary


class GameSummary(msgspec.Struct):
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
