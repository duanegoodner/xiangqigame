from pathlib import Path

import matplotlib.pyplot as plt
import numpy as np
from matplotlib.gridspec import GridSpec
import xiangqigame_core as core

from xiangqigame.game_summary import GameSummary
from xiangqigame.game_summary_io import import_game_summary
from xiangqigame.game_summary_plotters import (
    SearchResultsByTypePlotter,
    SearchTimePlotter,
    EvalScorePlotter,
)


class GameSummaryPlotManger:
    def __init__(self, game_summary: GameSummary):
        plt.style.use("bmh")
        self.game_summary = game_summary
        self.fig = plt.figure(figsize=(12, 18))
        self.search_type_plots = np.empty(
            shape=(2, self.num_players_with_minimax_data), dtype=object
        )
        self.search_time_plots = np.empty(
            shape=(2, self.num_players_with_minimax_data), dtype=object
        )
        self.eval_score_plots = np.empty(
            shape=(1, self.num_players_with_minimax_data), dtype=object
        )
        self.gs = GridSpec(
            nrows=7,
            ncols=self.num_players_with_minimax_data,
            figure=self.fig,
            height_ratios=[1, 1, 0.55, 1, 1, 0.45, 1],
        )
        self.build_layout()

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

    def build_layout(self):
        for gs_row in range(2):
            for gs_col in range(self.num_players_with_minimax_data):
                self.search_type_plots[gs_row, gs_col] = self.fig.add_subplot(
                    self.gs[gs_row, gs_col],
                )

        for gs_row in range(3, 5):
            for gs_col in range(self.num_players_with_minimax_data):
                self.search_time_plots[gs_row - 3, gs_col] = (
                    self.fig.add_subplot(
                        self.gs[gs_row, gs_col],
                    )
                )

        for gs_row in range(6, 7):
            for gs_col in range(self.num_players_with_minimax_data):
                self.eval_score_plots[gs_row - 6, gs_col] = (
                    self.fig.add_subplot(
                        self.gs[gs_row, gs_col],
                    )
                )

        plt.subplots_adjust(
            left=0.08,
            right=0.73,
            top=0.90,
            bottom=0.05,
            hspace=0.075,
            wspace=0.075,
        )

        self.fig.text(
            x=0.05, y=0.97, s=f"Summary plots for Game ID ____", fontsize=22
        )

        self.fig.text(
            x=0.05,
            y=0.93,
            s="Minimax Node Counts by Search Result Type",
            fontsize=18,
        )

        self.fig.text(
            x=0.05,
            y=0.57,
            s="Move Selection Time Data",
            fontsize=18,
        )

        self.fig.text(
            x=0.05,
            y=0.20,
            s="Minimax Evaulation Scores",
            fontsize=18,
        )

        self.fig.add_artist(
            plt.Line2D(
                [0.05, 0.95],
                [0.955, 0.955],
                transform=self.fig.transFigure,
                color="black",
            )
        )

        self.fig.add_artist(
            plt.Line2D(
                [0.05, 0.95],
                [0.595, 0.595],
                transform=self.fig.transFigure,
                color="black",
            )
        )

        self.fig.add_artist(
            plt.Line2D(
                [0.05, 0.95],
                [0.23, 0.23],
                transform=self.fig.transFigure,
                color="black",
            )
        )

    def plot(self):
        search_results_by_type_plotter = SearchResultsByTypePlotter(
            axes=self.search_type_plots,
            red_data=self.game_summary.get_player_summary(
                player=core.PieceColor.kRed
            ).first_searches_by_type,
            black_data=self.game_summary.get_player_summary(
                player=core.PieceColor.kBlk
            ).first_searches_by_type,
        )
        search_results_by_type_plotter.plot()

        search_time_plotter = SearchTimePlotter(
            axes=self.search_time_plots,
            red_data=self.game_summary.get_player_summary(
                player=core.PieceColor.kRed
            ).first_search_stats,
            black_data=self.game_summary.get_player_summary(
                player=core.PieceColor.kBlk
            ).first_search_stats,
        )
        search_time_plotter.plot()

        eval_score_plotter = EvalScorePlotter(
            axes=self.eval_score_plots,
            red_data=self.game_summary.get_player_summary(
                player=core.PieceColor.kRed
            ).first_search_stats,
            black_data=self.game_summary.get_player_summary(
                player=core.PieceColor.kBlk
            ).first_search_stats,
            add_plot_column_titles=False
        )
        eval_score_plotter.plot()

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
    plot_manager = GameSummaryPlotManger(game_summary=my_game_summary)
    plot_manager.plot()
