from functools import cached_property
from pathlib import Path
from typing import Dict, Tuple, cast

import numpy as np
import pandas as pd
import xiangqigame_core as core
from matplotlib import pyplot as plt
from matplotlib.colors import Colormap

import xiangqigame.search_stats_plotter as ssp
from xiangqigame.core_dataclass_mirrors import PointsT
from xiangqigame.game_summary import GameSummary
from xiangqigame.game_summary_io import import_game_summary


class NewGameSummaryPlotter:
    search_stats_time_cols = ["search_time_s", "mean_time_per_node_ns"]

    evaluating_player_line_colors = {
        core.PieceColor.kRed: "red",
        core.PieceColor.kBlk: "black",
    }

    player_text_labels = {
        core.PieceColor.kRed: "Red Player",
        core.PieceColor.kBlk: "Black Player",
    }

    non_evaluating_player_line_colors = {
        core.PieceColor.kRed: "lightcoral",
        core.PieceColor.kBlk: "darkgray",
    }

    def __init__(self, game_summary: GameSummary):
        self.game_summary = game_summary

    def has_minimax_data(self, player: core.PieceColor) -> bool:
        return self.game_summary.get_player_summary(
            player=player
        ).has_search_summaries

    @property
    def num_players_with_minimax_data(self) -> int:
        return sum(
            [
                int(self.has_minimax_data(player=core.PieceColor.kRed)),
                int(self.has_minimax_data(player=core.PieceColor.kBlk)),
            ]
        )

    @cached_property
    def search_stats_dfs(self) -> Dict[str, pd.DataFrame]:
        return {
            core.PieceColor.kRed.name: self.game_summary.get_player_summary(
                player=core.PieceColor.kRed
            ).first_search_stats,
            core.PieceColor.kBlk.name: self.game_summary.get_player_summary(
                player=core.PieceColor.kBlk
            ).first_search_stats,
        }

    @cached_property
    def search_results_by_type_dfs(self) -> Dict[str, pd.DataFrame]:
        return {
            core.PieceColor.kRed.name: self.game_summary.get_player_summary(
                player=core.PieceColor.kRed
            ).first_searches_by_type,
            core.PieceColor.kBlk.name: self.game_summary.get_player_summary(
                player=core.PieceColor.kBlk
            ).first_searches_by_type,
        }

    def validate_search_result_by_type_dfs(self):
        if self.num_players_with_minimax_data == 2:
            assert (
                self.search_results_by_type_dfs[
                    core.PieceColor.kRed.name
                ].columns
                == self.search_results_by_type_dfs[
                    core.PieceColor.kBlk.name
                ].columns
            ).all()

    @property
    def search_results_by_type_cmap(self) -> Colormap:
        return plt.get_cmap("Accent")

    @property
    def search_results_by_type_data_columns(self) -> pd.Index:
        return [
            item.columns
            for item in self.search_results_by_type_dfs.values()
            if item is not None
        ][0]

    @property
    def search_results_by_type_column_colors(
        self,
    ) -> Dict[str, Tuple[float, float, float, float]]:
        return {
            cast(str, col_name): self.search_results_by_type_cmap(idx)
            for idx, col_name in enumerate(
                self.search_results_by_type_data_columns
            )
        }

    @property
    def player_plot_col(self) -> Dict[core.PieceColor, int]:
        if self.num_players_with_minimax_data == 2:
            return {core.PieceColor.kRed: 0, core.PieceColor.kBlk: 1}
        elif self.has_minimax_data(player=core.PieceColor.kRed):
            return {core.PieceColor.kRed: 0, core.PieceColor.kBlk: None}
        elif self.has_minimax_data(player=core.PieceColor.kBlk):
            return {core.PieceColor.kRed: None, core.PieceColor.kBlk: 1}

    def _set_search_results_by_type_layout(
        self,
    ) -> Tuple[plt.Figure, np.ndarray]:
        fig, axes = plt.subplots(
            nrows=5,
            ncols=self.num_players_with_minimax_data,
            figsize=(12, 16),
            sharex=False,
            sharey=False,
        )

        fig.text(
            x=0.15,
            y=0.93,
            s="Minimax Node Counts by Search Result Type",
            fontsize=18,
        )

        plt.subplots_adjust(
            left=0.08,
            right=0.73,
            top=0.85,
            bottom=0.1,
            hspace=0.05,
            wspace=0.1,
        )

        axes = np.reshape(axes, (5, self.num_players_with_minimax_data))

        return fig, axes

    def plot_search_results_by_type_stacked(
        self, player: core.PieceColor, ax: plt.Axes
    ):
        df = self.search_results_by_type_dfs[player.name]
        df_sorted = df[sorted(df.columns, key=lambda col: df[col].mean())]
        sorted_colors = [
            self.search_results_by_type_column_colors[col]
            for col in df_sorted.columns
        ]
        ax.stackplot(
            df.index,
            df_sorted.T,
            labels=df_sorted.columns,
            colors=sorted_colors,
        )

    def plot_player_search_results_by_type(
        self, player: core.PieceColor, axes: np.ndarray
    ):
        for plot_row in range(2):
            plot_col = self.player_plot_col[player]
            ax = cast(plt.Axes, axes[plot_row, plot_col])
            self.plot_search_results_by_type_stacked(player=player, ax=ax)

    def plot_player_search_times(
        self, player: core.PieceColor, axes: np.ndarray
    ):
        df = self.search_stats_dfs[player.name]
        plot_col = self.player_plot_col[player]
        for data_col_idx, data_col in enumerate(self.search_stats_time_cols):
            ax = cast(plt.Axes, axes[data_col_idx, plot_col])
            ax.plot(
                df.index,
                df[data_col],
                color=self.evaluating_player_line_colors[player],
            )

    def set_search_results_by_type_legend(self, axes: np.ndarray):
        upper_right_plot = cast(plt.Axes, axes[0, -1])
        handles = [
            plt.Line2D(
                xdata=[0],
                ydata=[0],
                color=self.search_results_by_type_column_colors[col],
                lw=4,
            )
            for col in self.search_results_by_type_data_columns
        ]
        upper_right_plot.legend(
            handles,
            self.search_results_by_type_data_columns,
            bbox_to_anchor=(1.05, 0.40),
            fontsize=12,
        )

    def apply_all_search_results_by_type_axes_settings(self, axes: np.ndarray):

        ylabels = [
            "Node Counts Linear Scale",
            "Node Counts Log Scale",
            "Search Time (s)",
            "Search Time per Node (ns)",
            "Eval Score",
        ]

        logscale_rows = (1, 2)
        for grid_row_idx, grid_row in enumerate(axes[logscale_rows, :]):
            for ax in grid_row:
                ax = cast(plt.Axes, ax)
                ax.set_yscale("log")

        for idx, ax in enumerate(axes[:, 0]):
            ax = cast(plt.Axes, ax)
            ax.set_ylabel(ylabels[idx])

        for grid_row_idx, grid_row in enumerate(axes[0:-1, :]):
            for ax in grid_row:
                ax = cast(plt.Axes, ax)
                ax.set_xticklabels([])

        for idx, ax in enumerate(axes[0, :]):
            ax = cast(plt.Axes, ax)
            ax.set_xticklabels([])
            if idx == self.player_plot_col[core.PieceColor.kRed]:
                ax.set_title("Red Player")
            if idx == self.player_plot_col[core.PieceColor.kBlk]:
                ax.set_title("Black Player")

        for idx, ax in enumerate(axes[4, :]):
            ax = cast(plt.Axes, ax)
            ax.set_xlabel("Game Move Number")

        if axes.shape[1] == 2:
            for idx, ax in enumerate(axes[:, 1]):
                ax = cast(plt.Axes, ax)
                # ax.set_yticklabels([])

    @property
    def largest_magnitude_noninf_eval_score(self) -> int:
        largest_score_magnitudes = []
        inf_like_values = [np.iinfo(PointsT).max, np.iinfo(PointsT).min]
        for df in self.search_stats_dfs.values():
            if df is not None:
                assert df["eval_score"].dtype == PointsT
                non_inf_like_values = df[
                    ~df["eval_score"].isin(inf_like_values)
                ]["eval_score"]
                largest_score_magnitudes.append(
                    np.abs(non_inf_like_values.min())
                )
                largest_score_magnitudes.append(
                    np.abs(non_inf_like_values.max())
                )
        return max(largest_score_magnitudes)

    @staticmethod
    def symmetric_winsorize(
        data: pd.Series, magnitude: int | float
    ) -> pd.Series:
        assert magnitude >= 0
        return cast(
            pd.Series, np.clip(data, a_min=-1 * magnitude, a_max=magnitude)
        )

    def plot_eval_score(
        self,
        player: core.PieceColor,
        ax: plt.Axes,
        evaluating_player: bool = True,
    ):
        if self.has_minimax_data(player=player):
            df = self.search_stats_dfs[player.name]
            if evaluating_player:
                line_color = self.evaluating_player_line_colors[player]
                line_style = "solid"
            else:
                line_color = self.non_evaluating_player_line_colors[player]
                line_style = "dotted"
            ax.plot(
                df.index,
                self.symmetric_winsorize(
                    data=df["eval_score"],
                    magnitude=1.5 * self.largest_magnitude_noninf_eval_score,
                ),
                color=line_color,
                linestyle=line_style,
                label=self.player_text_labels[player],
            )


    def plot_eval_scores(self, player: core.PieceColor, axes: np.ndarray):
        plot_grid_col = self.player_plot_col[player]
        ax = cast(plt.Axes, axes[plot_grid_col])
        self.plot_eval_score(player=player, ax=ax)
        self.plot_eval_score(
            player=core.opponent_of(player), ax=ax, evaluating_player=False
        )
        print("setting legend")
        ax.legend(loc="upper left")

            # if self.has_minimax_data(player=player):
            #     df = self.search_stats_dfs[player.name]
            #     plot_grid_col = self.player_plot_col[player]
            #     ax = cast(plt.Axes, axes[plot_grid_col])
            #     ax.plot(
            #         df.index,
            #         self.symmetric_winsorize(
            #             data=df["eval_score"],
            #             magnitude=1.5
            #             * self.largest_magnitude_noninf_eval_score,
            #         ),
            #         color=self.evaluating_player_line_colors[player],
            #     )

    @staticmethod
    def match_y_limits(axes_row: np.ndarray):
        y_limits_low = []
        y_limits_high = []
        for col_idx in range(axes_row.shape[0]):
            cur_ax = cast(plt.Axes, axes_row[col_idx])
            y_limits_low.append(cur_ax.get_ylim()[0])
            y_limits_high.append(cur_ax.get_ylim()[1])
        for col_idx in range(axes_row.shape[0]):
            cur_ax = cast(plt.Axes, axes_row[col_idx])
            cur_ax.set_ylim((min(y_limits_low), max(y_limits_high)))

    def match_y_limits_all_rows(self, axes_grid: np.ndarray):
        for row_idx in range(axes_grid.shape[0]):
            self.match_y_limits(axes_grid[row_idx, :])

    def plot_all_search_results_by_type(self):
        plt.style.use("bmh")
        fig, axes = self._set_search_results_by_type_layout()
        for player in [core.PieceColor.kRed, core.PieceColor.kBlk]:
            if self.has_minimax_data(player=player):
                self.plot_player_search_results_by_type(
                    player=player, axes=axes
                )
                self.plot_player_search_times(
                    player=player, axes=axes[(2, 3), :]
                )
                self.plot_eval_scores(player=player, axes=axes[4, :])

        self.apply_all_search_results_by_type_axes_settings(axes=axes)
        self.match_y_limits_all_rows(axes_grid=axes)
        self.set_search_results_by_type_legend(axes=axes)
        plt.show()


if __name__ == "__main__":
    game_summary_path_depth_6 = (
        Path(__file__).parent.parent.parent
        / "data"
        / "20240923111251726641.json"
    )

    game_summary_path_depth_3 = (
        Path(__file__).parent.parent.parent
        / "data"
        / "20240923104026139692.json"
    )

    my_game_summary = import_game_summary(path=game_summary_path_depth_6)

    new_plotter = NewGameSummaryPlotter(game_summary=my_game_summary)
    new_plotter.plot_all_search_results_by_type()
    print("pause")

    # old_search_results_by_type_plotter = stp.SearchResultByTypePlotter(
    #     game_summary=my_game_summary
    # )
    # print("pause")
    # old_search_results_by_type_plotter.plot()
    old_search_stats_plotter = ssp.SearchStatsPlotter(
        game_summary=my_game_summary
    )

    old_search_stats_plotter.plot()
