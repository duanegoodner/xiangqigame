from pathlib import Path

import matplotlib.pyplot as plt
import numpy as np
from matplotlib.gridspec import GridSpec
from numpy.ma.core import shape
import xiangqigame_core as core

from xiangqigame.game_summary import GameSummary
from xiangqigame.game_summary_io import import_game_summary
from xiangqigame.gist_gridspec import search_type_plots


class GameSummaryPlotManger:
    def __init__(self, game_summary: GameSummary):
        self.game_summary = game_summary
        self.fig = plt.figure(figsize=(12, 16))
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
            height_ratios=[1, 1, 0.2, 1, 1, 0.2, 1],
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
                self.search_type_plots[gs_row, gs_col] = (
                    self.fig.add_subplot(
                        self.gs[gs_row, gs_col],
                    )
                )

        for gs_row in range(3, 5):
            for gs_col in range(self.num_players_with_minimax_data):
                self.search_time_plots[gs_row - 3, gs_col] = self.fig.add_subplot(
                    self.gs[gs_row, gs_col],
                )

        for gs_row in range(6, 7):
            for gs_col in range(self.num_players_with_minimax_data):
                self.eval_score_plots[gs_row - 6, gs_col] = self.fig.add_subplot(
                    self.gs[gs_row, gs_col],
                )

    def plot(self):
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
