"""
@file game_summary_plot_manager.py

Contains the GameSummaryPlotManager class.
"""

from pathlib import Path

import matplotlib.pyplot as plt
import numpy as np
from matplotlib.gridspec import GridSpec
import xiangqi_bindings as bindings

from xiangqipy.game_summary import GameSummary
from xiangqipy.game_summary_io import import_game_summary
from xiangqipy.game_summary_plotters import (
    SearchResultsByTypePlotter,
    SearchTimePlotter,
    EvalScorePlotter,
)


class GameSummaryPlotManager:
    """
    Manages the layout and file output of a figure containing multiple
    plots of data from a GameSummary.
    """
    def __init__(self, game_summary: GameSummary, save_fig: bool = False):
        plt.style.use("bmh")
        self.game_summary = game_summary
        self.save_fig = save_fig
        self.fig = plt.figure(figsize=(12, 19), dpi=150)
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
            nrows=8,
            ncols=self.num_players_with_minimax_data,
            figure=self.fig,
            height_ratios=[0.89, 1, 1, 0.7, 1, 1, 0.55, 1],
        )
        self.build_layout()

    def has_minimax_data(self, player: bindings.PieceColor) -> bool:
        return self.game_summary.get_player_summary(
            player=player
        ).has_search_summaries

    @property
    def num_players_with_minimax_data(self) -> int:
        return sum(
            [
                int(self.has_minimax_data(player=bindings.PieceColor.kRed)),
                int(self.has_minimax_data(player=bindings.PieceColor.kBlk)),
            ]
        )

    def build_layout(self):
        for gs_row in range(1, 3):
            for gs_col in range(self.num_players_with_minimax_data):
                self.search_type_plots[gs_row - 1, gs_col] = (
                    self.fig.add_subplot(
                        self.gs[gs_row, gs_col],
                    )
                )

        for gs_row in range(4, 6):
            for gs_col in range(self.num_players_with_minimax_data):
                self.search_time_plots[gs_row - 4, gs_col] = (
                    self.fig.add_subplot(
                        self.gs[gs_row, gs_col],
                    )
                )

        for gs_row in range(7, 8):
            for gs_col in range(self.num_players_with_minimax_data):
                self.eval_score_plots[gs_row - 7, gs_col] = (
                    self.fig.add_subplot(
                        self.gs[gs_row, gs_col],
                    )
                )

        plt.subplots_adjust(
            left=0.08,
            right=0.73,
            top=0.89,
            bottom=0.05,
            hspace=0.075,
            wspace=0.075,
        )

        self.fig.text(
            x=0.05,
            y=0.97,
            s=f"Summary plots for Game ID # {self.game_summary.game_id}",
            fontsize=22,
        )

        self.fig.text(
            x=0.05,
            y=0.85,
            s=f"RED player type: {self.game_summary.get_player_summary(bindings.PieceColor.kRed).player_type}\n"
            f"RED max search depth: {self.game_summary.get_player_summary(bindings.PieceColor.kRed).max_search_depth}\n"
            f"RED Zobrist key size: {self.game_summary.get_player_summary(bindings.PieceColor.kRed).zobrist_key_size} bits\n\n"
            f"BLACK player type: {self.game_summary.get_player_summary(bindings.PieceColor.kBlk).player_type}\n"
            f"BLACK max search depth: {self.game_summary.get_player_summary(bindings.PieceColor.kBlk).max_search_depth}\n"
            f"BLACK Zobrist key size: {self.game_summary.get_player_summary(bindings.PieceColor.kBlk).zobrist_key_size} bits\n\n"
            f"Result = {self.game_summary.game_state.name}",
            fontsize=14,
        )

        self.fig.add_artist(
            plt.Line2D(
                [0.05, 0.95],
                [0.83, 0.83],
                transform=self.fig.transFigure,
                color="black",
            )
        )

        self.fig.text(
            x=0.05,
            y=0.81,
            s="Minimax Node Counts by Search Result Type",
            fontsize=18,
        )

        self.fig.add_artist(
            plt.Line2D(
                [0.05, 0.95],
                [0.51, 0.51],
                transform=self.fig.transFigure,
                color="black",
            )
        )

        self.fig.text(
            x=0.05,
            y=0.49,
            s="Move Selection Time",
            fontsize=18,
        )

        self.fig.add_artist(
            plt.Line2D(
                [0.05, 0.95],
                [0.19, 0.19],
                transform=self.fig.transFigure,
                color="black",
            )
        )

        self.fig.text(
            x=0.05,
            y=0.17,
            s="Minimax Evaulation Scores",
            fontsize=18,
        )

    def save_figure(self, path: Path):
        if path.exists():
            raise FileExistsError(f"{path} already exists")
        path.parent.mkdir(parents=True, exist_ok=True)
        plt.savefig(str(path), dpi=self.fig.dpi)
        print(f"Plots of summary data saved to:\n{str(path.resolve())}")

    def plot(
        self,
        show_plot: bool = True
    ):
        search_results_by_type_plotter = SearchResultsByTypePlotter(
            axes=self.search_type_plots,
            red_data=self.game_summary.get_player_summary(
                player=bindings.PieceColor.kRed
            ).first_searches_by_type,
            black_data=self.game_summary.get_player_summary(
                player=bindings.PieceColor.kBlk
            ).first_searches_by_type,
        )
        search_results_by_type_plotter.plot()

        search_time_plotter = SearchTimePlotter(
            axes=self.search_time_plots,
            red_data=self.game_summary.get_player_summary(
                player=bindings.PieceColor.kRed
            ).first_search_stats,
            black_data=self.game_summary.get_player_summary(
                player=bindings.PieceColor.kBlk
            ).first_search_stats,
        )
        search_time_plotter.plot()

        eval_score_plotter = EvalScorePlotter(
            axes=self.eval_score_plots,
            red_data=self.game_summary.get_player_summary(
                player=bindings.PieceColor.kRed
            ).first_search_stats,
            black_data=self.game_summary.get_player_summary(
                player=bindings.PieceColor.kBlk
            ).first_search_stats,
            add_plot_column_titles=False,
        )
        eval_score_plotter.plot()

        if show_plot:
            plt.show()


if __name__ == "__main__":

    my_game_summary_path = (
        Path(__file__).parent.parent.parent
        / "data"
        / "game_summaries"
        / "20241101234034770120-test"
        / "20241101234034770120.json"
    )

    my_game_summary = import_game_summary(path=my_game_summary_path)
    plot_manager = GameSummaryPlotManager(game_summary=my_game_summary)
    plot_manager.plot(show_plot=True)
