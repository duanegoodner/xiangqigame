from dataclasses import dataclass
from typing import Literal

import numpy as np
import pandas as pd
import xiangqi_bindings as bindings
from matplotlib import pyplot as plt
from scipy.optimize import curve_fit

from xiangqipy.game_summary import GameSummary


def game_summaries_have_search_summary(
    color: bindings.PieceColor, game_summaries: list[GameSummary]
) -> bool:
    return any(
        [
            game_summary.get_player_summary(player=color).has_search_summaries
            for game_summary in game_summaries
        ]
    )


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
        if game_summaries_have_search_summary(
            color=color, game_summaries=self.game_summaries
        ):

            df = self.stacked_collision_data[color.name]
            sorted_df = df.sort_values(
                by="tr_table_num_entries", ascending=True
            )

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

    def plot_all_collision_data(self):
        for color in [bindings.PieceColor.kRed, bindings.PieceColor.kBlk]:
            self.plot_player_collision_data(color=color)


@dataclass
class TranspositionTableEvent:
    game_id: str
    event_type: Literal["end_game", "first_collision"]
    size_type: Literal["num_entries", "num_states"]
    table_size: int

    def __lt__(self, other):
        if self.table_size != other.table_size:
            return self.table_size < other.table_size
        else:
            return self.event_type == "first_collision"


class TrSizeEventScanner:
    def __init__(self, game_summaries: list[GameSummary]):
        self.game_summaries = game_summaries

    def get_sorted_tr_size_event_info(
        self,
        color: bindings.PieceColor,
        size_type: Literal["num_entries", "num_states"] = "num_states",
    ) -> tuple[TranspositionTableEvent, ...]:
        result_list = []
        for game_summary in self.game_summaries:
            player_summary = game_summary.get_player_summary(player=color)
            events_info = player_summary.tr_table_sizes_at_events
            result_list.append(
                TranspositionTableEvent(
                    game_id=game_summary.game_id,
                    event_type="first_collision",
                    size_type=size_type,
                    table_size=getattr(events_info.first_collision, size_type),
                )
            )
            result_list.append(
                TranspositionTableEvent(
                    game_id=game_summary.game_id,
                    event_type="end_game",
                    size_type=size_type,
                    table_size=getattr(events_info.end_game, size_type),
                )
            )
        result_list = [
            item for item in result_list if item.table_size is not None
        ]

        result_list.sort(reverse=False)

        return tuple(result_list)

    def scan_event_size(
        self,
        color: bindings.PieceColor,
        size_type: Literal["num_entries", "num_states"] = "num_states",
    ) -> pd.DataFrame:

        event_info = self.get_sorted_tr_size_event_info(
            color=color, size_type=size_type
        )
        first_collisions_dict = {
            item.game_id: item.table_size
            for item in event_info
            if item.event_type == "first_collision"
        }

        num_rows = len(event_info) + 1
        num_cols = 3
        result = np.zeros(shape=(num_rows, num_cols), dtype=np.int64)
        table_size = num_tables_with_collision = 0
        num_tables = len(event_info) - len(first_collisions_dict)
        result[0, :] = [table_size, num_tables, num_tables_with_collision]

        for idx, event in enumerate(event_info):
            table_size = event.table_size
            if event.event_type == "first_collision":
                num_tables_with_collision += 1
            if event.event_type == "end_game":
                num_tables -= 1
                if (
                    event.game_id in first_collisions_dict
                    and first_collisions_dict[event.game_id] <= table_size
                ):
                    num_tables_with_collision -= 1
            result[idx + 1, :] = [
                table_size,
                num_tables,
                num_tables_with_collision,
            ]
        df = pd.DataFrame(
            result,
            columns=["table_size", "num_tables", "num_tables_with_collision"],
        )
        df["fraction_orig_tables"] = df["num_tables"] / (
            len(event_info) - len(first_collisions_dict)
        )
        df["fraction_tables_with_collision"] = (
            df["num_tables_with_collision"] / df["num_tables"]
        )

        return df

    @property
    def event_size_scan_data(self) -> dict[str, pd.DataFrame]:
        result = {}
        for color in [bindings.PieceColor.kRed, bindings.PieceColor.kBlk]:
            result[color.name] = self.scan_event_size(color=color)
        return result

    def plot_player_event_size_scan_data(self, color: bindings.PieceColor):
        if game_summaries_have_search_summary(
            color=color, game_summaries=self.game_summaries
        ):
            df = self.event_size_scan_data[color.name]
            fig, ax1 = plt.subplots()
            ax1.set_ylabel("Fraction remaining tables at size")
            ax1.plot(df["table_size"], df["fraction_orig_tables"])
            ax2 = ax1.twinx()
            ax2.set_ylabel("Fraction tables with collision")
            ax2.plot(df["table_size"], df["fraction_tables_with_collision"])

            plt.show()

    def plot_all_event_size_scan_data(self):
        for color in [bindings.PieceColor.kRed, bindings.PieceColor.kBlk]:
            self.plot_player_event_size_scan_data(color=color)
