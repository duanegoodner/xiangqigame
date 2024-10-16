from dataclasses import dataclass
from datetime import datetime
from pathlib import Path
from typing import Any, Dict, List

import pandas as pd

import xiangqipy.app as app
from xiangqipy.game_summary import GameSummary
from xiangqipy.game_summary_io import import_game_summary


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
    def __init__(self, batch_id: str, custom_data_dir: Path = None):
        self.batch_id = batch_id
        self.custom_data_dir = custom_data_dir
        self.data_dir = self.get_data_dir()
        self.batch_df = pd.read_feather(
            path=self.data_dir / f"{self.batch_id}.feather"
        )

    def get_data_dir(self) -> Path:
        if self.custom_data_dir:
            return self.custom_data_dir
        else:
            batch_data_dir = None
            game_summaries_root = (
                Path(__file__).parent.parent.parent / "data" / "game_summaries"
            )
            for path in game_summaries_root.iterdir():
                if path.name.startswith(self.batch_id):
                    batch_data_dir = path
            if batch_data_dir is None:
                raise FileNotFoundError(
                    f"Directory with {self.batch_id} in name not found."
                )
            return batch_data_dir

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
        black_depth = self.batch_df["kBlk_search_depth"].mean()
        black_key_size = self.batch_df["kBlk_zobrist_key_size"].mean()
        black_nodes_per_move = self.batch_df["kBlk_nodes_per_move"].mean()
        black_time_per_move_s = self.batch_df["kBlk_time_per_move_s"].mean()
        black_time_per_node_ns = self.batch_df["kBlk_time_per_node_ns"].mean()
        black_collisions_per_move = self.batch_df[
            "kBlk_collisions_per_move"
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
                black_depth,
                black_key_size,
                black_nodes_per_move,
                black_time_per_move_s,
                black_time_per_node_ns,
                black_collisions_per_move,
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
                "black_depth",
                "black_key_size",
                "black_nodes_per_move",
                "black_time_per_move_s",
                "black_time_per_node_ns",
                "black_collisions_per_move",
            ],
        )


class MultiBatchDataSummarizer:
    def __init__(self, batch_ids: List[str]):
        self.batch_ids = batch_ids
        self.batch_data_summarizers: List[BatchDataSummarizer] = [
            BatchDataSummarizer(batch_id=batch_id) for batch_id in batch_ids
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
    def __init__(self, game_collection_id: str):
        self.game_collection_id = game_collection_id
        self.game_collection_dir = self.get_game_collection_dir()
        self.all_game_summaries: List[GameSummary] = (
            self.load_all_game_summaries()
        )
        self.batch_data_summarizer = BatchDataSummarizer(
            batch_id=game_collection_id
        )

    def get_game_collection_dir(self) -> Path:
        output_root = (
            Path(__file__).parent.parent.parent / "data" / "game_summaries"
        )

        game_collection_dir = None
        for path in output_root.iterdir():
            if path.name.startswith(self.game_collection_id):
                game_collection_dir = path
        if game_collection_dir is None:
            raise FileNotFoundError(
                f"Directory with {self.game_collection_id} in name not found."
            )
        return game_collection_dir

    def get_game_summary_paths(self) -> Dict[str, Path]:
        paths = {}
        for path in self.game_collection_dir.iterdir():
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


if __name__ == "__main__":
    # settings_tester = BatchTester(
    #     app_run_kwargs={
    #         "red_strength": 2,
    #         "black_strength": 2,
    #         "save_summary": True,
    #         "output_dir_suffix": "testing-batch-03",
    #     },
    #     num_games=3,
    # )
    #
    # settings_tester.run_all_tests()

    # my_game_collection_id = "20241016122642665682"
    # full_batch_summary = FullBatchSummary(
    #     game_collection_id=my_game_collection_id
    # )
    #
    # batch_data_summarizer = BatchDataSummarizer(batch_id=my_game_collection_id)
    #
    my_batch_ids = [
        "20241016145630274882",
        "20241016145638717717",
        "20241016145647309458",
    ]

    multi_batch_summarizer = MultiBatchDataSummarizer(batch_ids=my_batch_ids)

    print("pause")
