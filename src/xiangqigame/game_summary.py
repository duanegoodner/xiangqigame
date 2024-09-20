from dataclasses import dataclass
from typing import Dict, List

import msgspec
import xiangqigame_core as core

import xiangqigame.core_dataclass_mirrors as cdm
from xiangqigame.enums import GameState
from xiangqigame.player_summary import PlayerSummary


@dataclass
class PlayerSummaries:
    kRed: PlayerSummary
    kBlk: PlayerSummary


class GameSummary(msgspec.Struct):
    game_state: GameState
    whose_turn: core.PieceColor
    move_log: List[cdm.ExecutedMove]
    player_summaries: PlayerSummaries

    @property
    def move_counts(self) -> int:
        return len(self.move_log)

    @property
    def move_numbers(self) -> Dict[core.PieceColor, List[int]]:
        return {
            core.PieceColor.kBlk: [
                val for val in range(1, self.move_counts + 1) if (val % 2) == 0
            ],
            core.PieceColor.kRed: [
                val for val in range(self.move_counts + 1) if (val % 2) == 1
            ],
        }

    def get_player_summary(self, player: core.PieceColor) -> PlayerSummary:
        return self.player_summaries.__dict__[player.name]
