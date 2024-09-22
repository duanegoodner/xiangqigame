from cProfile import label

import numpy as np
import pandas as pd
from matplotlib import pyplot as plt
import xiangqigame_core as core
from typing import Dict, cast, Tuple
from xiangqigame.core_dataclass_mirrors import PointsT
from xiangqigame.game_summary import GameSummary


class SearchStatsPlotter:
    def __init__(self, game_summary: GameSummary):
        plt.style.use("bmh")
        self.game_summary = game_summary
        self.fig, self.axes = plt.subplots(
            nrows=len(self.data_columns),
            ncols=1,
            figsize=(8, 11),
            sharex=False,
            sharey=False,
        )

    _player_colors = {
        core.PieceColor.kRed: "red",
        core.PieceColor.kBlk: "black",
    }

    _data_labels = {
        core.PieceColor.kRed: "Red player",
        core.PieceColor.kBlk: "Black player",
    }

    _ylabels = {
        "num_nodes": "# nodes visited",
        "search_time_s": "Search time (s)",
        "mean_time_per_node_ns": "Mean time per node (ns)",
        "eval_score": "Minimax score (points)",
    }

    @property
    def raw_dfs(self) -> Dict[core.PieceColor, pd.DataFrame]:
        return {
            core.PieceColor.kRed: self.game_summary.get_player_summary(
                player=core.PieceColor.kRed
            ).first_search_stats,
            core.PieceColor.kBlk: self.game_summary.get_player_summary(
                player=core.PieceColor.kBlk
            ).first_search_stats,
        }

    @property
    def eval_winsorize_cutoff(self) -> int:
        largest_score_magnitudes = []
        extreme_values = [np.iinfo(PointsT).max, np.iinfo(PointsT).min]
        for raw_df in self.raw_dfs.values():
            assert raw_df["eval_score"].dtype == PointsT

            largest_score_magnitudes.append(
                np.abs(
                    raw_df[~raw_df["eval_score"].isin(extreme_values)][
                        "eval_score"
                    ].min()
                )
            )
            largest_score_magnitudes.append(
                np.abs(
                    raw_df[~raw_df["eval_score"].isin(extreme_values)][
                        "eval_score"
                    ].max()
                )
            )
        return 1.5 * max(largest_score_magnitudes)

    @property
    def winsorize_specs(self) -> Dict[str, Dict[str, int | float]]:
        return {
            "eval_score": {
                "min": -1 * self.eval_winsorize_cutoff,
                "max": self.eval_winsorize_cutoff,
            },
        }

    @property
    def data_columns(self) -> pd.Index:
        return [
            item.columns for item in self.raw_dfs.values() if item is not None
        ][0]

    def has_data(self, player: core.PieceColor) -> bool:
        return self.raw_dfs[player] is not None

    @property
    def num_players_with_data(self) -> int:
        return sum(
            [
                int(self.has_data(player=core.PieceColor.kRed)),
                int(self.has_data(player=core.PieceColor.kBlk)),
            ]
        )

    def winsorize(self, data: pd.Series):
        return np.clip(
            data,
            a_min=self.winsorize_specs[str(data.name)]["min"],
            a_max=self.winsorize_specs[str(data.name)]["max"],
        )

    def _plot_data(
        self,
        player: core.PieceColor,
        data_col: str,
        ax: plt.Axes,
    ):
        player_df = self.raw_dfs[player]
        data = self.raw_dfs[player][data_col]
        if data_col in self.winsorize_specs.keys():
            data = self.winsorize(data=data)
        ax.plot(
            player_df.index,
            data,
            color=self._player_colors[player],
            label=self._data_labels[player],
        )

    def _set_fig_layout(self):
        plt.subplots_adjust(
            left=0.12,
            right=0.73,
            top=0.95,
            bottom=0.1,
            hspace=0.05,
            wspace=0.1,
        )

        self.fig.text(x=0.15, y=0.93, s="Minimax Search Stats", fontsize=18)

        for idx, data_col in enumerate(self.data_columns):
            self.axes[idx].set_ylabel(self._ylabels[data_col])
            if idx != len(self.data_columns) - 1:
                self.axes[idx].set_xticklabels([])

        self.axes[len(self.data_columns) - 1].set_xlabel("Game Move Number")



    def plot(self):
        self._set_fig_layout()
        for player in self.raw_dfs.keys():
            for idx, data_col in enumerate(self.data_columns):
                self._plot_data(
                    player=player, data_col=data_col, ax=self.axes[idx]
                )

        self.axes[0].legend()
        plt.show()
