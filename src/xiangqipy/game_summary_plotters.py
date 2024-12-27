"""
Contains the GameSummaryPlotter abstract base class, and
multiple subclasses that implement it.

Each sub-class plots a different portion of data from a GameSummary.
"""

import warnings
from abc import ABC, abstractmethod
from typing import Dict, List, Tuple, cast

import numpy as np
import pandas as pd
import xiangqi_bindings as bindings
from matplotlib import pyplot as plt

from xiangqipy.core_dataclass_mirrors import PointsT


class GameSummaryPlotter(ABC):
    """
    Abstract base class for plotting data stored in pandas
    dataframes (one df for each player) to a numpy array of matplotlib Axes
    """

    evaluating_player_line_colors = {
        bindings.PieceColor.kRed: "red",
        bindings.PieceColor.kBlk: "black",
    }

    player_text_labels = {
        bindings.PieceColor.kRed: "Red Player",
        bindings.PieceColor.kBlk: "Black Player",
    }

    non_evaluating_player_line_colors = {
        bindings.PieceColor.kRed: "lightcoral",
        bindings.PieceColor.kBlk: "darkgray",
    }

    def __init__(
        self,
        axes: np.ndarray,
        y_labels: Tuple[str, ...],
        log_scale_rows: int | Tuple[int, ...] = tuple(),
        red_data: pd.DataFrame = None,
        black_data: pd.DataFrame = None,
        add_plot_column_titles: bool = True,
    ):
        self.axes = axes
        self.y_labels = y_labels
        if type(log_scale_rows) == int:
            log_scale_rows = (log_scale_rows,)
        self.log_scale_rows = log_scale_rows
        self._red_data = red_data
        self._black_data = black_data
        self._add_plot_column_titles = add_plot_column_titles
        self.validate_dfs()
        self.set_y_axes()
        self.set_x_axes()

    @property
    def dfs(self) -> Dict[str, pd.DataFrame]:
        return {
            bindings.PieceColor.kRed.name: self._red_data,
            bindings.PieceColor.kBlk.name: self._black_data,
        }

    def has_data(self, player: bindings.PieceColor) -> bool:
        return self.dfs[player.name] is not None

    @property
    def num_players_with_data(self) -> int:
        return sum(
            [
                int(self.has_data(bindings.PieceColor.kRed)),
                int(self.has_data(bindings.PieceColor.kBlk)),
            ]
        )

    def validate_dfs(self):
        if self.num_players_with_data == 2:
            assert (
                self.dfs[bindings.PieceColor.kRed.name].columns
                == self.dfs[bindings.PieceColor.kBlk.name].columns
            ).all()

    def set_y_axes(self):
        for grid_row_idx, grid_row in enumerate(
            self.axes[self.log_scale_rows, :]
        ):
            for ax in grid_row:
                ax = cast(plt.Axes, ax)
                ax.set_yscale("log")

        for grid_row_idx, ax in enumerate(self.axes[:, 0]):
            ax = cast(plt.Axes, ax)
            ax.set_ylabel(self.y_labels[grid_row_idx], fontsize=14)
            ax.yaxis.set_label_coords(-0.15, 0.5)

    def set_x_axes(self):
        for grid_row_idx, grid_row in enumerate(self.axes[0:-1, :]):
            for ax in grid_row:
                ax = cast(plt.Axes, ax)
                ax.set_xticklabels([])

        for grid_col_idx, ax in enumerate(self.axes[-1, :]):
            ax = cast(plt.Axes, ax)
            ax.set_xlabel("Game Move Number", fontsize=14)

    @property
    def player_plot_col(self) -> Dict[bindings.PieceColor, int]:
        if self.num_players_with_data == 2:
            return {bindings.PieceColor.kRed: 0, bindings.PieceColor.kBlk: 1}
        elif self.has_data(player=bindings.PieceColor.kRed):
            return {
                bindings.PieceColor.kRed: 0,
                bindings.PieceColor.kBlk: None,
            }
        elif self.has_data(player=bindings.PieceColor.kBlk):
            return {
                bindings.PieceColor.kRed: None,
                bindings.PieceColor.kBlk: 0,
            }

    @property
    def data_columns(self) -> pd.Index:
        return [
            item.columns for item in self.dfs.values() if item is not None
        ][0]

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

    def match_y_limits_all_rows(self):
        for row_idx in range(self.axes.shape[0]):
            self.match_y_limits(self.axes[row_idx, :])

    @abstractmethod
    def plot_data(self):
        pass

    def remove_second_plot_col_ylabels(self):
        if self.axes.shape[1] == 2:
            for idx, ax in enumerate(self.axes[:, 1]):
                ax = cast(plt.Axes, ax)
                ax.set_yticklabels([])

    def set_plot_col_titles(self):
        for idx, ax in enumerate(self.axes[0, :]):
            ax = cast(plt.Axes, ax)
            if idx == self.player_plot_col[bindings.PieceColor.kRed]:
                ax.set_title(
                    self.player_text_labels[bindings.PieceColor.kRed],
                    fontsize=16,
                )
            if idx == self.player_plot_col[bindings.PieceColor.kBlk]:
                ax.set_title(
                    self.player_text_labels[bindings.PieceColor.kBlk],
                    fontsize=16,
                )

    def plot(self):
        self.plot_data()
        self.remove_second_plot_col_ylabels()
        if self._add_plot_column_titles:
            self.set_plot_col_titles()
        self.match_y_limits_all_rows()


class SearchResultsByTypePlotter(GameSummaryPlotter):
    """
    Implements GameSummaryPlotter, and produces stacked plots of Minimax
    search result counts grouped by MinimaxResultType.
    """

    def __init__(
        self,
        axes: np.ndarray,
        y_labels: Tuple[str] = (
            "Node Counts",
            "Node Counts Log Scale",
        ),
        log_scale_rows: int | Tuple[int, int] = 1,
        cmap_name: str = "Accent",
        red_data: pd.DataFrame = None,
        black_data: pd.DataFrame = None,
        add_plot_column_titles: bool = True,
    ):
        super().__init__(
            axes=axes,
            y_labels=y_labels,
            log_scale_rows=log_scale_rows,
            red_data=red_data,
            black_data=black_data,
            add_plot_column_titles=add_plot_column_titles,
        )
        self.cmap = plt.get_cmap(cmap_name)

    @property
    def data_column_colors(
        self,
    ) -> Dict[str, Tuple[float, float, float, float]]:
        return {
            cast(str, col_name): self.cmap(idx)
            for idx, col_name in enumerate(self.data_columns)
        }

    def plot_search_results_by_type_stacked(
        self, player: bindings.PieceColor, ax: plt.Axes
    ):
        df = self.dfs[player.name]
        df_sorted = df[sorted(df.columns, key=lambda col: df[col].mean())]
        sorted_colors = [
            self.data_column_colors[col] for col in df_sorted.columns
        ]

        with warnings.catch_warnings():
            warnings.filterwarnings(
                "ignore",
                category=UserWarning,
                message="Data has no positive values, and therefore cannot be log-scaled.",
            )
            ax.stackplot(
                df.index,
                df_sorted.T,
                labels=df_sorted.columns,
                colors=sorted_colors,
            )

    def plot_player_data(self, player: bindings.PieceColor):
        for plot_row in range(2):
            plot_col = self.player_plot_col[player]
            ax = cast(plt.Axes, self.axes[plot_row, plot_col])
            self.plot_search_results_by_type_stacked(player=player, ax=ax)

    def add_legend(self):
        upper_right_plot = cast(plt.Axes, self.axes[0, -1])
        handles = [
            plt.Line2D(
                xdata=[0],
                ydata=[0],
                color=self.data_column_colors[col],
                lw=4,
            )
            for col in self.data_columns
        ]
        upper_right_plot.legend(
            handles,
            self.data_columns,
            bbox_to_anchor=(1.05, 0.50),
            fontsize=14,
            title="Node Type",
            title_fontsize=15,
            alignment="left",
        )

    def plot_data(self):

        for player in [bindings.PieceColor.kRed, bindings.PieceColor.kBlk]:
            if self.has_data(player=player):
                self.plot_player_data(player=player)
        self.add_legend()


class SearchTimePlotter(GameSummaryPlotter):
    """
    Implements GameSummaryPlotter, and produces plots showing time spent
    by core MinimaxMoveEvaluator(s) evaluating nodes and selecting Moves.
    """

    search_stats_time_cols = ["search_time_s", "mean_time_per_node_ns"]

    def __init__(
        self,
        axes: np.ndarray,
        log_scale_rows: int | Tuple[int, ...] = tuple(),
        red_data: pd.DataFrame = None,
        black_data: pd.DataFrame = None,
        add_plot_column_titles: bool = True,
    ):
        super().__init__(
            axes=axes,
            y_labels=(
                "Search Time (s)",
                "Time per Node (ns)",
            ),
            log_scale_rows=log_scale_rows,
            red_data=red_data,
            black_data=black_data,
            add_plot_column_titles=add_plot_column_titles,
        )

    def plot_player_search_times(self, player: bindings.PieceColor):
        df = self.dfs[player.name]
        plot_col = self.player_plot_col[player]
        for data_col_idx, data_col in enumerate(self.search_stats_time_cols):
            ax = cast(plt.Axes, self.axes[data_col_idx, plot_col])
            ax.plot(
                df.index,
                df[data_col],
                color=self.evaluating_player_line_colors[player],
            )

    def plot_data(self):
        for player in [bindings.PieceColor.kRed, bindings.PieceColor.kBlk]:
            if self.has_data(player=player):
                self.plot_player_search_times(player=player)


class EvalScorePlotter(GameSummaryPlotter):
    """
    Implements GameSummaryPlotter, and plots evaluated score of each move of
    each Player using a Minimax algorithm in a Game.
    """

    def __init__(
        self,
        axes: np.ndarray,
        log_scale_rows: int | Tuple[int, ...] = tuple(),
        red_data: pd.DataFrame = None,
        black_data: pd.DataFrame = None,
        add_plot_column_titles: bool = True,
    ):
        super().__init__(
            axes=axes,
            y_labels=("Evaluation Score",),
            log_scale_rows=log_scale_rows,
            red_data=red_data,
            black_data=black_data,
            add_plot_column_titles=add_plot_column_titles,
        )

    @property
    def largest_magnitude_noninf_eval_score(self) -> int:
        largest_score_magnitudes = []
        inf_like_values = [np.iinfo(PointsT).max, np.iinfo(PointsT).min]
        for df in self.dfs.values():
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

    def plot_player_data(
        self,
        player: bindings.PieceColor,
        ax: plt.Axes,
        is_evaluating_player: bool = True,
    ):
        if self.has_data(player=player):
            df = self.dfs[player.name]
            if is_evaluating_player:
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
            legend = ax.legend(loc="upper left", fontsize="12")
            legend.get_frame().set_facecolor("white")

    def plot_player_and_opponent_overlay(self, player: bindings.PieceColor):
        plot_grid_col = self.player_plot_col[player]
        ax = cast(plt.Axes, self.axes[0, plot_grid_col])
        self.plot_player_data(player=player, ax=ax)

        if self.has_data(player=bindings.opponent_of(player)):
            self.plot_player_data(
                player=bindings.opponent_of(player),
                ax=ax,
                is_evaluating_player=False,
            )

    def plot_data(self):
        for player in [bindings.PieceColor.kRed, bindings.PieceColor.kBlk]:
            if self.has_data(player=player):
                self.plot_player_and_opponent_overlay(player=player)
