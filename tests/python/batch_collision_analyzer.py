import pandas as pd
import xiangqi_bindings as bindings
from matplotlib import pyplot as plt
from scipy.optimize import curve_fit

from xiangqipy.game_summary import GameSummary

class PoissonCollisionAnalyzer:
    tr_collision_cols = [
        "tr_table_num_states",
        "tr_table_num_entries",
        "returned_illegal_move",
        "cumulative_illegal_moves",
    ]

    plot_markers = ["o", "s", "^", ">", "v", "p", "*", "h", "x", "D"]
    num_plot_colors = 20
    plot_colors = plt.cm.get_cmap("tab20", num_plot_colors)

    def __init__(self, game_summaries: list[GameSummary]):
        self.game_summaries = game_summaries

    def get_styles(self, num_categories: int) -> list[tuple]:
        styles = []
        for marker in self.plot_markers:
            for idx in range(self.num_plot_colors):
                styles.append((self.plot_colors(idx), marker))
                if len(styles) >= num_categories:
                    break
            if len(styles) >= num_categories:
                break
        return styles

    def get_player_stacked_collision_data(
        self, color: bindings.PieceColor
    ) -> pd.DataFrame | None:
        player_summaries = [
            game_summary.get_player_summary(color)
            for game_summary in self.game_summaries
        ]

        game_ids = [
            game_summary.game_id for game_summary in self.game_summaries
        ]

        if any(
            [
                player_summary.has_search_summaries
                for player_summary in player_summaries
            ]
        ):
            single_game_dfs = []
            for game_id, player_summary in zip(game_ids, player_summaries):
                if player_summary.has_search_summaries:
                    df = player_summary.first_search_stats[
                        self.tr_collision_cols
                    ]
                    df["game_id"] = game_id
                    df.reset_index(drop=True, inplace=True)
                    single_game_dfs.append(df)

            all_games_df = pd.concat(single_game_dfs, axis=0)

            # Reorder columns to make 'game_id' the first column
            columns = ["game_id"] + [
                col for col in all_games_df.columns if col != "game_id"
            ]
            all_games_df[columns] = all_games_df[columns]

            return all_games_df

    @property
    def stacked_collision_data(self) -> dict[str, pd.DataFrame | None]:
        result = {}
        for color in [bindings.PieceColor.kBlk, bindings.PieceColor.kRed]:
            result[color.name] = self.get_player_stacked_collision_data(color)
        return result

    @staticmethod
    def poisson_collision_approximation(k, n):
        return k**2 / (2 * n)

    def poisson_fit(self, x_data, y_data):
        params, params_covariance, *_ = curve_fit(
            self.poisson_collision_approximation, x_data, y_data
        )
        return self.poisson_collision_approximation(x_data, *params)

    def plot_player_collision_data(
        self,
        color: bindings.PieceColor,
        legend_by_game_id: bool = True,
        poisson_fit: bool = True,
    ):
        df = self.stacked_collision_data[color.name]
        sorted_df = df.sort_values(by="tr_table_num_entries", ascending=True)

        x_data = sorted_df["tr_table_num_states"]
        y_data = sorted_df["cumulative_illegal_moves"]

        labels = sorted_df["game_id"].unique()
        styles = self.get_styles(len(labels))
        label_styles = dict(zip(labels, styles))

        if not legend_by_game_id:
            plt.scatter(x_data, y_data)
        else:
            for label, style in label_styles.items():
                plt.scatter(
                    x_data[sorted_df["game_id"] == label],
                    y_data[sorted_df["game_id"] == label],
                    color=style[0],
                    marker=style[1],
                    label=label,
                )

        if poisson_fit:
            fitted_collisions = self.poisson_fit(x_data, y_data)
            plt.plot(x_data, fitted_collisions)

        plt.show()
