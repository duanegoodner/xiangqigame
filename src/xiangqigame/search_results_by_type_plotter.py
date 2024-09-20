from dataclasses import dataclass
from typing import Dict, Tuple, cast

import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import xiangqigame_core as core

from xiangqigame.game import GameSummary


@dataclass
class SearchResultByTypePlotterSettings:
    cmap: str = "tab10"
    plot_style: str = "bmh"
    fig_size: Tuple[float, float] = (12, 8)
    fig_text_x: float = 0.15
    fig_text_y: float = 0.93
    subplot_adj_left: float = 0.08
    subplot_adj_right: float = 0.73
    subplot_adj_top: float = 0.85
    subplot_adj_bottom: float = 0.1
    subplot_adj_hspace: float = 0.05
    legend_bbox_to_anchor: Tuple[float, float] = (1.05, 0.40)


class SearchResultByTypePlotter:

    settings_for_num_players_with_data = {
        1: {
            "fig_size": (8, 8),
            "subplot_adj_left": 0.12,
            "subplot_adj_right": 0.60,
        },
        2: {
            "fig_size": (12, 8),
            "subplot_adj_left": 0.08,
            "subplot_adj_right": 0.73,
        },
    }

    def __init__(self, game_summary: GameSummary):
        self.game_summary = game_summary
        # self.cmap = plt.get_cmap(cmap)
        self.validate_dfs()
        self.settings = SearchResultByTypePlotterSettings(
            **self.settings_for_num_players_with_data[
                self.num_players_with_data
            ]
        )
        self.cmap = plt.get_cmap(self.settings.cmap)

    @property
    def dfs(self) -> Dict[core.PieceColor, pd.DataFrame]:
        return {
            core.PieceColor.kRed: self.game_summary.get_player_summary(
                player=core.PieceColor.kRed
            ).first_searches_by_type_df,
            core.PieceColor.kBlk: self.game_summary.get_player_summary(
                player=core.PieceColor.kBlk
            ).first_searches_by_type_df,
        }

    def validate_dfs(self):
        """
        If each player has a dataframe with selection data, confirm that
        that column names for the two dataframes match.
        """
        if self.num_players_with_data == 2:
            assert (
                self.dfs[core.PieceColor.kRed].columns
                == self.dfs[core.PieceColor.kBlk].columns
            ).all()

    @property
    def data_columns(self) -> pd.Index:
        """
        Since we've confirmed df col names match, we can grab col names from
        one df and use when plotting either/both players' data.
        """
        return [
            item.columns for item in self.dfs.values() if item is not None
        ][0]

    @property
    def max_row_sum(self) -> int:
        row_sums = {}
        for player, df in self.dfs.items():
            if df is not None:
                row_sums[player] = df.sum(axis=1)
        max_row_sums = {
            player: max(row_sum_list)
            for player, row_sum_list in row_sums.items()
        }
        return max(max_row_sums.values())

    @property
    def column_colors(self) -> Dict[str, Tuple[float, float, float, float]]:
        return {
            cast(str, col_name): self.cmap(idx)
            for idx, col_name in enumerate(self.data_columns)
        }

    def has_data(self, player: core.PieceColor) -> bool:
        return self.dfs[player] is not None

    @property
    def num_players_with_data(self) -> int:
        return sum(
            [
                int(self.has_data(player=core.PieceColor.kRed)),
                int(self.has_data(player=core.PieceColor.kBlk)),
            ]
        )

    @property
    def player_plot_col(self) -> Dict[core.PieceColor, int]:
        if self.num_players_with_data == 2:
            return {core.PieceColor.kRed: 0, core.PieceColor.kBlk: 1}
        elif self.has_data(player=core.PieceColor.kRed):
            return {core.PieceColor.kRed: 0, core.PieceColor.kBlk: None}
        elif self.has_data(player=core.PieceColor.kBlk):
            return {core.PieceColor.kRed: None, core.PieceColor.kBlk: 1}

    def _set_figure_layout(self):
        fig, axes = plt.subplots(
            nrows=2,
            ncols=self.num_players_with_data,
            figsize=self.settings.fig_size,
            sharex=False,
            sharey=False,
        )

        fig.text(
            x=self.settings.fig_text_x,
            y=self.settings.fig_text_y,
            s="Minimax Node Counts by Search Result Type",
            fontsize=18,
        )

        plt.subplots_adjust(
            left=self.settings.subplot_adj_left,
            right=self.settings.subplot_adj_right,
            top=self.settings.subplot_adj_top,
            bottom=self.settings.subplot_adj_bottom,
            hspace=self.settings.subplot_adj_hspace,
            # wspace=0.1,
        )

        # We need axes to be a 2-D array, even if there is only one column
        axes = np.reshape(axes, (2, self.num_players_with_data))

        return fig, axes

    def plot_df(
        self,
        df: pd.DataFrame,
        ax: plt.Axes,
    ):

        df_sorted = df[sorted(df.columns, key=lambda col: df[col].sum())]
        sorted_colors = [self.column_colors[col] for col in df_sorted.columns]
        ax.stackplot(
            df.index,
            df_sorted.T,
            labels=df_sorted.columns,
            colors=sorted_colors,
        )

    def _plot_player_data(self, player: core.PieceColor, axes: np.ndarray):
        df = self.game_summary.get_player_summary(
            player=player
        ).first_searches_by_type_df
        for plot_row in range(2):
            # inform linter of the actual type of axes element
            plot_col = self.player_plot_col[player]
            ax = cast(plt.Axes, axes[plot_row, plot_col])
            self.plot_df(
                df=df,
                ax=ax,
            )
            if plot_row == 1:
                ax.set_yscale("log")
                ax.set_xlabel("Game Move Number")

            if plot_row == 0:
                ax.set_xticklabels([])

            if plot_col == 0 and plot_row == 0:
                ax.set_ylabel("Node Counts")

            if plot_col == 0 and plot_row == 1:
                ax.set_ylabel("Node Counts (Log Scale)")

            if plot_row == 0 and player == core.PieceColor.kBlk:
                ax.set_title("Black Player")

            if plot_row == 0 and player == core.PieceColor.kRed:
                ax.set_title("Red Player")

            if (plot_row == 0) and (
                plot_col == self.num_players_with_data - 1
            ):
                handles = [
                    plt.Line2D([0], [0], color=self.column_colors[col], lw=4)
                    for col in df.columns
                ]
                ax.legend(
                    handles,
                    df.columns,
                    bbox_to_anchor=self.settings.legend_bbox_to_anchor,
                    fontsize=12,
                )  # Legend in the same order as original DataFrame

    def plot(self):
        plt.style.use(self.settings.plot_style)
        fig, axes = self._set_figure_layout()
        for player in [core.PieceColor.kRed, core.PieceColor.kBlk]:
            if self.has_data(player=player):
                self._plot_player_data(player=player, axes=axes)
        plt.show()