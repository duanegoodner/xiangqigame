from dataclasses import dataclass
from turtledemo.forest import start
from typing import Any, Dict, List, cast, Tuple

import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import xiangqigame_core as core
from matplotlib.colors import LinearSegmentedColormap, Colormap
from typing import cast
from pandas import DataFrame

from xiangqigame.core_dataclass_mirrors import PointsT
from xiangqigame.game import GameSummary
from xiangqigame.game_interfaces import PlayerSummary
from xiangqigame.gist_python_enums import color


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


class SearchResultsByDepthPlotter:

    color_ranges = {
        core.PieceColor.kRed: ["#ff9999", "#990000"],
        core.PieceColor.kBlk: ["#d9d9d9", "#000000"],
    }

    def __init__(self, game_summary: GameSummary):
        self.game_summary = game_summary

    def get_colormap(
        self, player: core.PieceColor, num_shades: int
    ) -> LinearSegmentedColormap:
        return LinearSegmentedColormap.from_list(
            name=f"{int}_shades_of_{player.name}",
            colors=self.color_ranges[player],
            N=num_shades,
        )

    def plot_single_result_type_stacked(
        self,
        player: core.PieceColor,
        result_type: core.MinimaxResultType,
        ax: plt.Axes,
        y_label: str,
    ):
        df = self.game_summary.get_player_summary(
            player=player
        ).first_searches_by_type_and_depth[result_type.name]

        # cmap = plt.get_cmap("tab10")
        cmap = self.get_colormap(player=player, num_shades=df.shape[1])
        column_colors = {col: cmap(i) for i, col in enumerate(df.columns)}
        df_sorted = df[sorted(df.columns, key=lambda col: df[col].sum())]
        colors = [column_colors[col] for col in df_sorted.columns]
        ax.stackplot(
            df.index, df_sorted.T, labels=df_sorted.columns, colors=colors
        )
        ax.set_ylabel(y_label)
        ax.set_yscale("log")

        handles = [
            plt.Line2D([0], [0], color=column_colors[col], lw=4)
            for col in df.columns
        ]
        ax.legend(
            handles, df.columns, loc="upper right"
        )  # Legend in the same order as original DataFrame

    def plot_multiple_search_result_types_by_depth(
        self,
        player: core.PieceColor,
        result_types: List[core.MinimaxResultType],
    ):
        fig, ax = plt.subplots(
            nrows=len(result_types),
            ncols=2,
            figsize=(12, 6 * len(result_types)),
        )
        for i, result_type in enumerate(result_types):
            self.plot_single_result_type_stacked(
                player=player,
                result_type=result_type,
                ax=ax[i][0],
                y_label=result_type.name,
            )

        plt.show()


class GameSummaryPlotter:

    color_ranges = {
        core.PieceColor.kRed: ["#ff9999", "#990000"],
        core.PieceColor.kBlk: ["#d9d9d9", "#000000"],
    }

    def __init__(
        self, game_summary: GameSummary, winsorize_multiplier: float = 2.0
    ):
        self.game_summary = game_summary
        self.winsorize_multiplier = winsorize_multiplier

    def get_colormap(
        self, player: core.PieceColor, num_shades: int
    ) -> LinearSegmentedColormap:
        return LinearSegmentedColormap.from_list(
            name=f"{int}_shades_of_{player.name}",
            colors=self.color_ranges[player],
            N=num_shades,
        )

    def plot_data(
        self,
        player_data: Dict[core.PieceColor, List[np.ndarray | List]],
        ax: plt.Axes,
    ):
        if player_data.get(core.PieceColor.kRed, None) is not None:
            for series in player_data.get(core.PieceColor.kRed, []):
                ax.plot(
                    self.game_summary.move_numbers[core.PieceColor.kRed],
                    series,
                )
        if player_data.get(core.PieceColor.kBlk, None) is not None:
            for series in player_data.get(core.PieceColor.kBlk, []):
                ax.plot(
                    self.game_summary.move_numbers[core.PieceColor.kBlk],
                    series,
                )

    @staticmethod
    def plot_df_stacked_sorted(
        df: pd.DataFrame,
        cmap: Colormap,
        ax: plt.Axes,
        add_legend: bool = False,
        bbox_to_anchor: Tuple[float, float] = (0.0, 1.0),
    ):
        column_colors = {col: cmap(idx) for idx, col in enumerate(df.columns)}
        df_sorted = df[sorted(df.columns, key=lambda col: df[col].sum())]
        sorted_colors = [column_colors[col] for col in df_sorted.columns]
        ax.stackplot(
            df.index,
            df_sorted.T,
            labels=df_sorted.columns,
            colors=sorted_colors,
        )

        if add_legend:
            handles = [
                plt.Line2D([0], [0], color=column_colors[col], lw=4)
                for col in df.columns
            ]
            ax.legend(
                handles, df.columns, bbox_to_anchor=bbox_to_anchor
            )  # Legend in the same order as original DataFrame

    def plot_first_search_by_type(self, player: core.PieceColor, ax: plt.Axes):
        df = self.game_summary.get_player_summary(
            player=player
        ).first_searches_by_type_df
        cmap = plt.get_cmap("tab10")
        self.plot_df_stacked_sorted(df=df, cmap=cmap, ax=ax)

    def plot_end_game_leaves(self, player: core.PieceColor, ax: plt.Axes):
        search_type_info = self.game_summary.get_player_summary(
            player=player
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
            red_data=self.game_summary.get_player_summary(
                player=core.PieceColor.kRed
            ).first_searches_eval_scores,
            black_data=self.game_summary.get_player_summary(
                player=core.PieceColor.kBlk
            ).first_searches_eval_scores,
        ).get_all_winsorized_score_data()

        self.plot_data(player_data=data_to_plot, ax=ax)

    def plot_endgame_leaf_counts(self, ax: plt.Axes):
        endgame_keys = [
            core.MinimaxResultType.EvaluatorLoses.name,
            core.MinimaxResultType.EvaluatorWins.name,
        ]

        red_series = [
            self.game_summary.get_player_summary(
                player=core.PieceColor.kRed
            ).first_searches_by_type[key]
            for key in endgame_keys
        ]

        black_series = [
            self.game_summary.get_player_summary(
                player=core.PieceColor.kBlk
            ).first_searches_by_type[key]
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
                self.game_summary.get_player_summary(
                    player=core.PieceColor.kRed
                ).first_searches_by_type[
                    core.MinimaxResultType.TrTableHitStandard
                ]
            ],
            core.PieceColor.kBlk: [
                self.game_summary.get_player_summary(
                    player=core.PieceColor.kBlk
                ).first_searches_by_type[
                    core.MinimaxResultType.TrTableHitStandard
                ]
            ],
        }
        self.plot_data(player_data=data_to_plot, ax=ax)

    def plot_pruning_counts(self, ax: plt.Axes):
        keys = [
            core.MinimaxResultType.AlphaPrune.name,
            core.MinimaxResultType.BetaPrune.name,
        ]

        data_to_plot = {
            core.PieceColor.kRed: [
                self.game_summary.get_player_summary(
                    player=core.PieceColor.kRed
                ).first_searches_by_type[key]
                for key in keys
            ],
            core.PieceColor.kBlk: [
                self.game_summary.get_player_summary(
                    player=core.PieceColor.kBlk
                ).first_searches_by_type[key]
                for key in keys
            ],
        }

        self.plot_data(player_data=data_to_plot, ax=ax)
