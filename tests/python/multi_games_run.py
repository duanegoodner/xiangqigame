from datetime import datetime
from pathlib import Path
from typing import Any, Dict, List

import pandas as pd

import xiangqipy.app as app
from xiangqipy.game_summary import GameSummary
from xiangqipy.game_summary_io import import_game_summary


class SingleSettingsTester:
    def __init__(self, app_run_kwargs: Dict[str, Any], num_games: int):
        self.app_run_kwargs = app_run_kwargs
        self.num_games = num_games
        self.game_collection_id = "".join(
            char for char in str(datetime.now()) if char.isdigit()
        )
        self.game_summaries: List[GameSummary] = []

    @property
    def output_dir(self) -> Path:
        return Path(__file__).parent.parent.joinpath()

    def run_game(self):
        game_summary = app.run(
            game_collection_id=self.game_collection_id, **self.app_run_kwargs
        )
        self.game_summaries.append(game_summary)

    def run_all_tests(self):
        for game_idx in range(self.num_games):
            self.run_game()

    @property
    def games_basic_stats(self) -> pd.DataFrame:
        return pd.DataFrame(
            [game_summary.basic_stats for game_summary in self.game_summaries],
            index=[
                game_summary.game_id for game_summary in self.game_summaries
            ],
        )

    def save_basic_stats(self):
        output_dir_name = self.game_collection_id
        output_dir_suffix = self.app_run_kwargs.get("output_dir_suffix")
        if output_dir_suffix:
            output_dir_name += f"-{output_dir_suffix}"
        output_dir = (
            Path(__file__).parent.parent.parent
            / "data"
            / "game_summaries"
            / output_dir_name
        )
        output_dir.mkdir(parents=True, exist_ok=True)
        self.games_basic_stats.to_feather(
            output_dir / f"{self.game_collection_id}.feather"
        )


class GameSummariesCollection:
    def __init__(self, game_collection_id: str):
        self.game_collection_id = game_collection_id
        self.multi_game_summary: pd.DataFrame = self.load_multi_game_summary()
        self.all_game_summaries: List[GameSummary] = (
            self.load_all_game_summaries()
        )

    def load_multi_game_summary(self) -> pd.DataFrame:
        output_root = (
            Path(__file__).parent.parent.parent
            / "data"
            / "multi_game_summaries"
        )
        result_dir = None
        for path in output_root.iterdir():
            if path.name.startswith(self.game_collection_id):
                result_dir = path
        if result_dir is None:
            raise FileNotFoundError(
                f"Directory with {self.game_collection_id} in name not found."
            )

        result_df = pd.read_feather(
            Path(result_dir / f"{self.game_collection_id}.feather")
        )
        return result_df

    def get_game_summary_paths(self) -> Dict[str, Path]:
        paths = {}
        game_ids = list(self.multi_game_summary.index)
        game_summaries_output_root = (
            Path(__file__).parent.parent.parent / "data" / "game_summaries"
        )
        for path in game_summaries_output_root.iterdir():
            path_game_id = path.name.split("-")[0]
            if path_game_id in game_ids:
                paths[path_game_id] = path / f"{path_game_id}.json"

        if len(paths) != len(game_ids):
            raise FileNotFoundError(
                "Number of Paths does not match number of game IDs"
            )

        return paths

    def load_all_game_summaries(self) -> List[GameSummary]:
        game_summary_paths = self.get_game_summary_paths()

        game_summaries = []
        for game_id, path in game_summary_paths.items():
            game_summaries.append(import_game_summary(path=path))

        return game_summaries

    @property
    def collection_summary_series(self) -> pd.Series:
        num_games = len(self.multi_game_summary)
        num_moves_mean = self.multi_game_summary["move_counts"].mean()
        fraction_black_won = (
            self.multi_game_summary["game_state"] == "BLACK_WON"
        ).mean()
        red_depth = self.multi_game_summary["kRed_search_depth"].mean()
        red_key_size = self.multi_game_summary["kRed_zobrist_key_size"].mean()
        red_nodes_per_move = self.multi_game_summary[
            "kRed_nodes_per_move"
        ].mean()
        red_time_per_move_s = self.multi_game_summary[
            "kRed_time_per_move_s"
        ].mean()
        red_time_per_node_ns = self.multi_game_summary[
            "kRed_time_per_node_ns"
        ].mean()
        red_collisions_per_move = self.multi_game_summary[
            "kRed_collisions_per_move"
        ].mean()
        black_depth = self.multi_game_summary["kBlk_search_depth"].mean()
        black_key_size = self.multi_game_summary[
            "kBlk_zobrist_key_size"
        ].mean()
        black_nodes_per_move = self.multi_game_summary[
            "kBlk_nodes_per_move"
        ].mean()
        black_time_per_move_s = self.multi_game_summary[
            "kBlk_time_per_move_s"
        ].mean()
        black_time_per_node_ns = self.multi_game_summary[
            "kBlk_time_per_node_ns"
        ].mean()
        black_collisions_per_move = self.multi_game_summary[
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


if __name__ == "__main__":
    settings_tester = SingleSettingsTester(
        app_run_kwargs={
            "red_strength": 3,
            "black_strength": 3,
            "save_summary": True,
            "output_dir_suffix": "testing-batch",
        },
        num_games=3,
    )

    settings_tester.run_all_tests()
    settings_tester.save_basic_stats()

    # my_game_collection_id = "20241016072621833003"
    # my_game_summaries_collection = GameSummariesCollection(
    #     game_collection_id=my_game_collection_id
    # )

    print("pause")
