"""
GameSummary class and its component classes.
"""

from dataclasses import dataclass
from typing import Dict, List

import msgspec
import pandas as pd
import xiangqi_bindings as bindings

import xiangqipy.core_dataclass_mirrors as cdm
from xiangqipy.enums import GameState
from xiangqipy.player_summary import PlayerSummary


@dataclass
class PlayerSummaries:
    """
    A data container for holding one PlayerSummary for each player in a Game.
    """

    kRed: PlayerSummary
    kBlk: PlayerSummary


class GameSummary(msgspec.Struct):
    """
    Holds summary info of a xiangqipy.game.Game; implements @ref msgspec.Struct for json IO.

    Attributes are all either primitives or from core_data_class_mirrors to
    minimize need for enc_hook's in msgspec IO.

    @param game_id str: unique ID of the Game, typically based on timestamp when Game is instantiated.
    @param game_state xiangqipy.enums.GameState: state of the Game.
    @param whose_turn xianqi_bindings.PieceColor: color of Player to execute next move.
    @param move_log (List[xiangqipy.core_dataclass_mirrors.ExecutedMove]): log of all executed moves.
    @param player_summaries (PlayerSummaries): contains a xiangqipy.player_summary.PlayerSummary for
    each xiangqipy.players.AIPlayer in the Game.
    """

    game_id: str
    game_state: GameState
    whose_turn: bindings.PieceColor
    move_log: List[cdm.ExecutedMove]
    player_summaries: PlayerSummaries

    @property
    def move_counts(self) -> int:
        """
        Number of moves executed in the Game.
        """
        return len(self.move_log)

    @property
    def move_numbers(self) -> Dict[bindings.PieceColor, List[int]]:
        """
        Dictionary with list of move ID numbers executed by each player;
        normally Red player's list has odd integers, and Black's has even integers.
        """
        return {
            bindings.PieceColor.kBlk: [
                val for val in range(1, self.move_counts + 1) if (val % 2) == 0
            ],
            bindings.PieceColor.kRed: [
                val for val in range(self.move_counts + 1) if (val % 2) == 1
            ],
        }

    def get_player_summary(self, player: bindings.PieceColor) -> PlayerSummary:
        """
        Gets the xiangqipy.player_summary.PlayerSummary for a particular xiangqi_bindings.PieceColor.

        @param player xiangqi_bindings.PieceColor of player that PlayerSummary is being retrieved for.
        """
        return self.player_summaries.__dict__[player.name]

    @property
    def basic_stats(self) -> pd.Series:
        basic_game_stats = pd.Series(
            [self.move_counts, self.game_state.name],
            index=["move_counts", "game_state"],
        )
        for player in [bindings.PieceColor.kRed, bindings.PieceColor.kBlk]:
            player_summary = self.get_player_summary(player=player)
            if player_summary.has_search_summaries:
                player_stats = player_summary.selection_stats
                player_stats.index = [
                    f"{player.name}_{idx}"
                    for idx in player_stats.index
                ]

                basic_game_stats = pd.concat([basic_game_stats, player_stats])

        return basic_game_stats
