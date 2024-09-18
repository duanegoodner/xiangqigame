from dataclasses import dataclass
from typing import Any, Dict, List

import matplotlib.pyplot as plt
import numpy as np
import xiangqigame_core as core

from xiangqigame.core_dataclass_mirrors import PointsT
from xiangqigame.game import GameSummary
from xiangqigame.game_interfaces import PlayerSummary


class MinimaxPlayerInfoPlotter:

    _points_type_signed_size_dispatch = {
        (True, 32): np.int32,
        (True, 64): np.int64,
    }

    def __init__(self, player_summary: PlayerSummary):
        assert player_summary.move_evaluator_type in [
            "MinimaxMoveEvaluator64",
            "MinimaxMoveEvaluator128",
        ]
        self.player_summary = player_summary

    @property
    def num_moves(self) -> int:
        return len(self.player_summary.search_summaries.first_searches)

    @property
    def move_numbers(self) -> List[int]:
        if self.player_summary.piece_color == core.PieceColor.kRed:
            return [2 * val + 1 for val in range(self.num_moves)]
        if self.player_summary.piece_color == core.PieceColor.kBlk:
            return [2 * val + 2 for val in range(self.num_moves)]

    def plot_vs_move_numbers(self, data: List[Any], ax: plt.Axes):
        ax.plot(self.move_numbers, data)

    def get_winsorized_eval_scores(self, cutoff: PointsT) -> np.array:
        assert cutoff >= 0
        result = (
            self.player_summary.search_summaries.first_searches_eval_scores.copy()
        )
        for idx in range(len(result)):
            if result[idx] > cutoff:
                result[idx] = cutoff
            if result[idx] < -1 * cutoff:
                result[idx] = -1 * cutoff
        return result

    def plot_eval_scores(
        self,
        ax: plt.Axes,
        winsorize_magnitude: PointsT,
    ):
        data = self.get_winsorized_eval_scores(winsorize_magnitude)
        self.plot_vs_move_numbers(data=data, ax=ax)


@dataclass
class EvalScoreDataCleaner:
    red_data: np.ndarray = None
    black_data: np.ndarray = None
    cutoff_multiplier: float = 2.0

    def get_winsorize_points_cutoff(self, score_data: np.array) -> int:
        if score_data is not None:
            assert score_data.dtype == PointsT
            assert np.issubdtype(score_data.dtype, np.signedinteger)
            non_extreme_vals = score_data[
                (score_data != np.iinfo(score_data.dtype).min)
                & (score_data != np.iinfo(score_data.dtype).max)
            ]
            largest_magnitude = np.max(np.abs(non_extreme_vals))
            return int(self.cutoff_multiplier * largest_magnitude)

    @staticmethod
    def winsorize_score_data(
        score_data: np.ndarray, cutoff: int
    ) -> np.ndarray | None:
        assert cutoff >= 0
        if score_data is not None:
            return np.clip(score_data, a_min=-1 * cutoff, a_max=cutoff)

    def get_all_winsorized_score_data(
        self,
    ) -> Dict[core.PieceColor, List[np.array]]:
        winsorize_cutoff = max(
            [
                self.get_winsorize_points_cutoff(data)
                for data in [self.red_data, self.black_data]
                if data is not None
            ]
        )

        winsorized_red_data = self.winsorize_score_data(
            score_data=self.red_data, cutoff=winsorize_cutoff
        )
        winsorized_black_data = self.winsorize_score_data(
            score_data=self.black_data, cutoff=winsorize_cutoff
        )

        return {
            core.PieceColor.kRed: [winsorized_red_data],
            core.PieceColor.kBlk: [winsorized_black_data],
        }


class GameSummaryPlotter:
    def __init__(
        self, game_summary: GameSummary, winsorize_multiplier: float = 2.0
    ):
        self.game_summary = game_summary
        self.winsorize_multiplier = winsorize_multiplier

    @property
    def red_summary(self) -> PlayerSummary:
        return self.game_summary.player_summaries.red

    @property
    def black_summary(self) -> PlayerSummary:
        return self.game_summary.player_summaries.black

    def get_player_summary(self, player: core.PieceColor) -> PlayerSummary:
        if player == core.PieceColor.kRed:
            return self.red_summary
        if player == core.PieceColor.kBlk:
            return self.black_summary

    @property
    def move_numbers(self) -> Dict[core.PieceColor, List[int]]:
        return {
            core.PieceColor.kBlk: [
                val
                for val in range(1, self.game_summary.move_counts + 1)
                if (val % 2) == 0
            ],
            core.PieceColor.kRed: [
                val
                for val in range(self.game_summary.move_counts + 1)
                if (val % 2) == 1
            ],
        }

    def plot_data(
        self,
        player_data: Dict[core.PieceColor, List[np.ndarray | List]],
        ax: plt.Axes,
    ):
        if player_data.get(core.PieceColor.kRed, None) is not None:
            for series in player_data.get(core.PieceColor.kRed, []):
                ax.plot(self.move_numbers[core.PieceColor.kRed], series)
        if player_data.get(core.PieceColor.kBlk, None) is not None:
            for series in player_data.get(core.PieceColor.kBlk, []):
                ax.plot(self.move_numbers[core.PieceColor.kBlk], series)

    def plot_end_game_leaves(self, player: core.PieceColor, ax: plt.Axes):
        search_type_info = self.get_player_summary(
            player
        ).first_searches_by_type
        search_type_keys = [
            core.MinimaxResultType.EvaluatorLoses.name,
            core.MinimaxResultType.EvaluatorWins.name,
            core.MinimaxResultType.TrTableHitEvaluatorLoses.name,
            core.MinimaxResultType.TrTableHitEvaluatorWins.name,
        ]

        data = {player: [search_type_info[key] for key in search_type_keys]}
        self.plot_data(player_data=data, ax=ax)

    def plot_eval_scores(self, ax: plt.Axes):
        data_to_plot = EvalScoreDataCleaner(
            self.red_summary.first_searches_eval_scores,
            black_data=self.black_summary.first_searches_eval_scores,
        ).get_all_winsorized_score_data()

        self.plot_data(player_data=data_to_plot, ax=ax)

    def plot_endgame_leaf_counts(self, ax: plt.Axes):
        endgame_keys = [
            core.MinimaxResultType.EvaluatorLoses.name,
            core.MinimaxResultType.EvaluatorWins.name,
        ]

        red_series = [
            self.red_summary.first_searches_by_type[key]
            for key in endgame_keys
        ]

        black_series = [
            self.black_summary.first_searches_by_type[key]
            for key in endgame_keys
        ]

        self.plot_data(
            player_data={
                core.PieceColor.kRed: red_series,
                core.PieceColor.kBlk: black_series,
            },
            ax=ax,
        )

    def plot_tr_table_hit_counts(self, ax: plt.Axes):
        data_to_plot = {
            core.PieceColor.kRed: [
                self.red_summary.first_searches_by_type[
                    core.MinimaxResultType.TrTableHitStandard
                ]
            ],
            core.PieceColor.kBlk: [
                self.black_summary.first_searches_by_type[
                    core.MinimaxResultType.TrTableHitStandard
                ]
            ],
        }
        self.plot_data(player_data=data_to_plot, ax=ax)

    def plot_pruning_counts(self, ax: plt.Axes):
        keys = [core.MinimaxResultType.AlphaPrune.name, core.MinimaxResultType.BetaPrune.name]

        data_to_plot = {
            core.PieceColor.kRed: [
                self.red_summary.first_searches_by_type[key] for key in keys
            ],
            core.PieceColor.kBlk: [
                self.black_summary.first_searches_by_type[key] for key in keys
            ],
        }

        self.plot_data(player_data=data_to_plot, ax=ax)


if __name__ == "__main__":

    my_fig, my_axes = plt.subplots(nrows=2, ncols=2, sharex=True, sharey=True)
