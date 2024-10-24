import re
from dataclasses import dataclass
from datetime import datetime
from pathlib import Path
from statistics import covariance
from typing import Any, Dict, List

import numpy as np
import pandas as pd
import xiangqi_bindings as bindings
from matplotlib import pyplot as plt
from scipy.interpolate import UnivariateSpline
from scipy.optimize import curve_fit

import xiangqipy.app as app
from xiangqipy.game_summary import GameSummary
from xiangqipy.game_summary_io import import_game_summary

DEFAULT_NUM_GAMES = 10


@dataclass
class BatchTestConditions:
    run_kwargs: Dict[str, Any]
    num_games: int = DEFAULT_NUM_GAMES

    @classmethod
    def from_abbreviation(cls, abbreviation: str) -> "BatchTestConditions":
        abbrev_split = abbreviation.split("-")
        num_games = int(abbrev_split[0])
        red_strength = int(re.sub(f"[a-zA-Z]", "", abbrev_split[2]))
        red_key_size = int(re.sub(f"[a-zA-Z]", "", abbrev_split[3]))
        black_strength = int(re.sub(f"[a-zA-Z]", "", abbrev_split[5]))
        black_key_size = int(re.sub(f"[a-zA-Z]", "", abbrev_split[6]))
        run_kwargs = {
            "red_strength": red_strength,
            "red_key_size": red_key_size,
            "black_strength": black_strength,
            "black_key_size": black_key_size,
            "save_summary": True,
            "output_dir_suffix": abbreviation,
        }
        return cls(num_games=num_games, run_kwargs=run_kwargs)


class BatchTester:
    def __init__(
        self,
        app_run_kwargs: Dict[str, Any],
        num_games: int,
        output_root: Path = None,
    ):
        self.app_run_kwargs = app_run_kwargs
        self.num_games = num_games
        self.game_collection_id = "".join(
            char for char in str(datetime.now()) if char.isdigit()
        )
        if output_root is None:
            output_root = self.create_output_root_dir()
        self.output_root = output_root

        self.game_summaries: List[GameSummary] = []

    @property
    def output_dir(self) -> Path:
        return Path(__file__).parent.parent.joinpath()

    def create_output_root_dir(self) -> Path:
        output_dir_name = f"{self.game_collection_id}-BATCH"
        output_dir_suffix = self.app_run_kwargs.get("output_dir_suffix")
        if output_dir_suffix:
            output_dir_name += f"-{output_dir_suffix}"
        output_root = (
            Path(__file__).parent.parent.parent
            / "data"
            / "game_summaries"
            / output_dir_name
        )
        output_root.mkdir(parents=True, exist_ok=True)

        return output_root

    def run_game(self):
        game_summary = app.run(
            custom_output_root=self.output_root, **self.app_run_kwargs
        )
        self.game_summaries.append(game_summary)

    @property
    def games_basic_stats(self) -> pd.DataFrame:
        return pd.DataFrame(
            [game_summary.basic_stats for game_summary in self.game_summaries],
            index=[
                game_summary.game_id for game_summary in self.game_summaries
            ],
        )

    def save_basic_stats(self):
        self.games_basic_stats.to_feather(
            self.output_root / f"{self.game_collection_id}.feather"
        )

    def run_all_tests(self):
        for game_idx in range(self.num_games):
            self.run_game()
            self.save_basic_stats()


class BatchDataSummarizer:
    def __init__(
        self,
        batch_dir: str | Path,
    ):
        if Path(batch_dir).is_absolute():
            self.batch_dir = Path(batch_dir)
        else:
            self.batch_dir = (
                Path(__file__).parent.parent.parent
                / "data"
                / "game_summaries"
                / str(batch_dir)
            )
        self.batch_id = self.batch_dir.name.split("-")[0]
        self.batch_df = pd.read_feather(
            path=self.batch_dir / f"{self.batch_id}.feather"
        )

    @property
    def batch_summary_series(self) -> pd.Series:
        num_games = len(self.batch_df)
        num_moves_mean = self.batch_df["move_counts"].mean()
        fraction_black_won = (
            self.batch_df["game_state"] == "BLACK_WON"
        ).mean()
        red_depth = self.batch_df["kRed_search_depth"].mean()
        red_key_size = self.batch_df["kRed_zobrist_key_size"].mean()
        red_nodes_per_move = self.batch_df["kRed_nodes_per_move"].mean()
        red_time_per_move_s = self.batch_df["kRed_time_per_move_s"].mean()
        red_time_per_node_ns = self.batch_df["kRed_time_per_node_ns"].mean()
        red_collisions_per_move = self.batch_df[
            "kRed_collisions_per_move"
        ].mean()
        red_collisions_per_node = self.batch_df[
            "kRed_collisions_per_node"
        ].mean()
        black_depth = self.batch_df["kBlk_search_depth"].mean()
        black_key_size = self.batch_df["kBlk_zobrist_key_size"].mean()
        black_nodes_per_move = self.batch_df["kBlk_nodes_per_move"].mean()
        black_time_per_move_s = self.batch_df["kBlk_time_per_move_s"].mean()
        black_time_per_node_ns = self.batch_df["kBlk_time_per_node_ns"].mean()
        black_collisions_per_move = self.batch_df[
            "kBlk_collisions_per_move"
        ].mean()
        black_collisions_per_node = self.batch_df[
            "kBlk_collisions_per_node"
        ].mean()

        return pd.Series(
            data=[
                num_games,
                num_moves_mean,
                fraction_black_won,
                red_depth,
                red_key_size,
                red_nodes_per_move,
                red_time_per_move_s,
                red_time_per_node_ns,
                red_collisions_per_move,
                red_collisions_per_node,
                black_depth,
                black_key_size,
                black_nodes_per_move,
                black_time_per_move_s,
                black_time_per_node_ns,
                black_collisions_per_move,
                black_collisions_per_node,
            ],
            index=[
                "num_games",
                "num_moves_mean",
                "fraction_black_won",
                "red_depth",
                "red_key_size",
                "red_nodes_per_move",
                "red_time_per_move_s",
                "red_time_per_node_ns",
                "red_collisions_per_move",
                "red_collisions_per_node",
                "black_depth",
                "black_key_size",
                "black_nodes_per_move",
                "black_time_per_move_s",
                "black_time_per_node_ns",
                "black_collisions_per_move",
                "black_collisions_per_node",
            ],
        )


class MultiBatchDataSummarizer:
    def __init__(self, batch_dirs: List[Path | str]):
        self.batch_dirs = batch_dirs
        self.batch_data_summarizers: List[BatchDataSummarizer] = [
            BatchDataSummarizer(batch_dir=batch_dir)
            for batch_dir in batch_dirs
        ]

    @property
    def summary_df(self) -> pd.DataFrame:
        return pd.DataFrame(
            [
                summarizer.batch_summary_series
                for summarizer in self.batch_data_summarizers
            ]
        )


class FullBatchSummary:
    def __init__(self, batch_dir: str):
        if Path(batch_dir).is_absolute():
            self.batch_dir = Path(batch_dir)
        else:
            self.batch_dir = (
                Path(__file__).parent.parent.parent
                / "data"
                / "game_summaries"
                / str(batch_dir)
            )

        self.all_game_summaries: List[GameSummary] = (
            self.load_all_game_summaries()
        )
        self.batch_data_summarizer = BatchDataSummarizer(batch_dir=batch_dir)

    def get_game_summary_paths(self) -> Dict[str, Path]:
        paths = {}
        for path in self.batch_dir.iterdir():
            if path.is_dir():
                path_game_id = path.name.split("-")[0]
                paths[path_game_id] = path / f"{path_game_id}.json"

        return paths

    def load_all_game_summaries(self) -> List[GameSummary]:
        game_summary_paths = self.get_game_summary_paths()

        game_summaries = []
        for game_id, path in game_summary_paths.items():
            game_summaries.append(import_game_summary(path=path))

        return game_summaries

    @property
    def games_basic_stats(self) -> pd.DataFrame:
        return pd.DataFrame(
            [game_summary.basic_stats for game_summary in self.all_game_summaries],
            index=[
                game_summary.game_id for game_summary in self.all_game_summaries
            ],
        )

    def get_player_collision_data(
        self, color: bindings.PieceColor
    ) -> pd.DataFrame | None:
        player_summaries = [
            game_summary.get_player_summary(color)
            for game_summary in self.all_game_summaries
        ]

        tr_collision_cols = [
            "tr_table_num_states",
            "tr_table_num_entries",
            "returned_illegal_move",
            "cumulative_illegal_moves",
        ]

        if any(
            [
                player_summary.has_search_summaries
                for player_summary in player_summaries
            ]
        ):

            data_frames = []
            for summary, game_summary in zip(
                player_summaries, self.all_game_summaries
            ):
                if summary.has_search_summaries:
                    # Create a copy to avoid modifying the original DataFrame
                    df = summary.first_search_stats[tr_collision_cols].copy()
                    df["game_id"] = (
                        game_summary.game_id
                    )  # Append the game_id column
                    df.reset_index(drop=True, inplace=True)  # Reset the index
                    data_frames.append(df)

            # Concatenate all the prepared DataFrames
            batch_collision_df = pd.concat(data_frames, axis=0)

            # Reorder columns to make 'game_id' the first column
            columns = ["game_id"] + [
                col for col in batch_collision_df.columns if col != "game_id"
            ]
            batch_collision_df = batch_collision_df[columns]

            return batch_collision_df

    @property
    def all_collision_data(self) -> Dict[str, pd.DataFrame]:
        result = {}
        for color in [bindings.PieceColor.kBlk, bindings.PieceColor.kRed]:
            result[color.name] = self.get_player_collision_data(color)
        return result

    def plot_all_collision_data(
        self, color: bindings.PieceColor, by_game_id: bool = True
    ):
        df = self.all_collision_data[color.name]
        # fig, ax = plt.subplots()

        def poisson_collision_model(k, n):
            return k**2 / (2 * n)

        sorted_df = df.sort_values(by="tr_table_num_entries", ascending=True)

        x_data = sorted_df["tr_table_num_states"]
        y_data = sorted_df["cumulative_illegal_moves"]
        labels = sorted_df["game_id"].unique()
        data_point_colors = plt.cm.get_cmap("tab20", len(labels))

        params, params_covariance, *_ = curve_fit(
            poisson_collision_model, x_data, y_data
        )

        fitted_collisions = poisson_collision_model(x_data, *params)

        plt.figure(figsize=(10, 6))

        if by_game_id:
            for i, label in enumerate(labels):
                subset = sorted_df[sorted_df["game_id"] == label]
                plt.scatter(
                    subset["tr_table_num_states"],
                    subset["cumulative_illegal_moves"],
                    color=data_point_colors(i),
                    label=label,
                )
        else:
            plt.scatter(
                sorted_df["tr_table_num_states"],
                sorted_df["cumulative_illegal_moves"],
                color="blue",
            )
            # plt.scatter(x_data, y_data, color="blue", label="Data Points")
        plt.plot(
            x_data,
            fitted_collisions,
            label="Fitted Poisson Approximation Model",
            color="red",
        )

        plt.title("Exponential Fit to Data")
        plt.xlabel("X values")
        plt.ylabel("Cumulative Counts")
        # plt.legend()
        plt.show()


if __name__ == "__main__":
    my_batch_dirs = [
        "20241023153411705876-BATCH-100-R-d2-k032-B-d2-k032",
        "20241023153611738579-BATCH-100-R-d2-k064-B-d2-k064",
        "20241023153811028455-BATCH-100-R-d2-k128-B-d2-k128",
    ]

    full_batch_summary_032_bit = FullBatchSummary(
        batch_dir=my_batch_dirs[0],
    )
    full_batch_summary_064_bit = FullBatchSummary(
        batch_dir=my_batch_dirs[1],
    )
    full_batch_summary_128_bit = FullBatchSummary(
        batch_dir=my_batch_dirs[2],
    )

    multi_batch_summarizer = MultiBatchDataSummarizer(batch_dirs=my_batch_dirs)

    test_item = (
        full_batch_summary_032_bit.all_game_summaries[2]
        .get_player_summary(bindings.PieceColor.kBlk)
        .tr_table_size_at_first_known_collision
    )
    print("pause")
