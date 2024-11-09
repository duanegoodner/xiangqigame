import re
from _ast import Mult
from dataclasses import dataclass
from datetime import datetime
from pathlib import Path
from typing import Any, Literal

import numpy as np
import pandas as pd
import xiangqi_bindings as bindings
from batch_collision_analyzer import (
    game_summaries_have_search_summary,
    PoissonCollisionAnalyzer,
    TrSizeEventScanner,
)
from matplotlib import pyplot as plt
from scipy.optimize import curve_fit

import xiangqipy.app as app
import xiangqipy.core_dataclass_mirrors as cdm
from xiangqipy.game_interfaces import Player
from xiangqipy.game_summary import GameSummary
from xiangqipy.game_summary_io import import_game_summary

DEFAULT_NUM_GAMES = 10


@dataclass
class BatchTestConditions:
    run_kwargs: dict[str, Any]
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
            "red_number_zobrist_states": 2,
            "black_strength": black_strength,
            "black_key_size": black_key_size,
            "black_number_zobrist_states": 2,
            "save_summary": True,
            "output_dir_suffix": abbreviation,
        }
        return cls(num_games=num_games, run_kwargs=run_kwargs)


class BatchTester:
    def __init__(
        self,
        app_run_kwargs: dict[str, Any],
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

        self.game_summaries: list[GameSummary] = []

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

        self.game_summaries: list[GameSummary] = self.load_game_summaries()

        # self.poisson_collision_analyzer = PoissonCollisionAnalyzer(
        #     self.game_summaries
        # )
        # self.tr_size_event_scanner = TrSizeEventScanner(self.game_summaries)

    @property
    def players_with_data(self) -> list[bindings.PieceColor]:
        return [
            color
            for color in [bindings.PieceColor.kRed, bindings.PieceColor.kBlk]
            if game_summaries_have_search_summary(
                color=color, game_summaries=self.game_summaries
            )
        ]

    @property
    def player_data_col_prefixes(self) -> list[str]:
        return [player.name for player in self.players_with_data]

    def get_game_summary_paths(self) -> dict[str, Path]:
        paths = {}
        for path in self.batch_dir.iterdir():
            if path.is_dir():
                path_game_id = path.name.split("-")[0]
                paths[path_game_id] = path / f"{path_game_id}.json"

        return paths

    def load_game_summaries(self) -> list[GameSummary]:
        game_summary_paths = self.get_game_summary_paths()

        game_summaries = []
        for game_id, path in game_summary_paths.items():
            game_summaries.append(import_game_summary(path=path))

        return game_summaries

    @property
    def games_basic_stats(self) -> pd.DataFrame:
        df = pd.DataFrame(
            [game_summary.basic_stats for game_summary in self.game_summaries],
            index=[
                game_summary.game_id for game_summary in self.game_summaries
            ],
        )
        return df

    @property
    def num_games(self) -> pd.Series:
        return pd.Series(
            data=[len(self.games_basic_stats)], index=["num_games"]
        )

    @property
    def win_lose_draw_summary(self) -> pd.Series:
        series = self.games_basic_stats["game_state"].value_counts(
            normalize=True
        )

        series = series.rename(
            {item: f"fraction_{item}" for item in series.index}
        )
        return series

    @property
    def fraction_games_with_illegal_move_request(self) -> pd.Series:
        num_illegal_move_requests_cols = [
            f"{prefix}_num_illegal_move_requests"
            for prefix in self.player_data_col_prefixes
        ]
        series = self.games_basic_stats[num_illegal_move_requests_cols].astype(
            bool
        ).sum() / len(self.games_basic_stats)

        rename_dict = {
            f"{prefix}_num_illegal_move_requests": f"{prefix}_fraction_games_with_illegal_move_request"
            for prefix in self.player_data_col_prefixes
        }
        series = series.rename(rename_dict)
        return series

    @property
    def basic_stats_means(self) -> pd.Series:
        player_info_names_for_means = [
            "search_depth",
            "zobrist_key_size",
            "nodes_per_move",
            "time_per_move_s",
            "time_per_node_ns",
            "num_illegal_move_requests",
            "collisions_per_move",
            "collisions_per_node",
        ]

        players_data_cols_for_means = []
        for prefix in self.player_data_col_prefixes:
            for player_info_name in player_info_names_for_means:
                players_data_cols_for_means.append(
                    f"{prefix}_{player_info_name}"
                )

        series = self.games_basic_stats[
            ["move_counts"] + players_data_cols_for_means
        ].mean()

        series = series.rename({item: f"{item}_mean" for item in series.index})

        return series

    @property
    def batch_summary_series(self) -> pd.Series:
        return pd.concat(
            [
                self.num_games,
                self.win_lose_draw_summary,
                self.fraction_games_with_illegal_move_request,
                self.basic_stats_means,
            ]
        )

    # def plot_poisson_collision_fits(self):
    #     self.poisson_collision_analyzer.plot_all_collision_data()

    # def plot_tr_event_size_scan_data(self):
    #     self.tr_size_event_scanner.plot_all_event_size_scan_data()


class MultiBatchSummary:
    def __init__(self, batch_dirs: list[Path | str]):
        self.batch_dirs = batch_dirs
        self.batch_summaries = [
            FullBatchSummary(batch_dir=batch_dir) for batch_dir in batch_dirs
        ]

    @property
    def summary_df(self) -> pd.DataFrame:
        return pd.DataFrame(
            [
                batch_summary.batch_summary_series
                for batch_summary in self.batch_summaries
            ]
        )


if __name__ == "__main__":

    multi_batch_summary = MultiBatchSummary(
        batch_dirs=[
            "20241108173153768315-BATCH-2-R-d3-k064-B-d3-k064",
            "20241108173157830386-BATCH-2-R-d3-k064-B-d3-k064"
        ]
    )

    summary_df = multi_batch_summary.summary_df

    print("pause")
