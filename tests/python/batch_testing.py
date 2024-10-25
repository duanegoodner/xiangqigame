import re
from collections import OrderedDict
from dataclasses import dataclass
from datetime import datetime
from pathlib import Path
from typing import Any, Dict, List, Literal, Tuple

import numpy as np
import pandas as pd
import xiangqi_bindings as bindings
from batch_collision_analyzer import PoissonCollisionAnalyzer
from matplotlib import pyplot as plt
from scipy.optimize import curve_fit

import xiangqipy.app as app
import xiangqipy.core_dataclass_mirrors as cdm
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

        self.game_summaries: List[GameSummary] = self.load_game_summaries()
        # self.size_collision_analyzer = TrSizeCollisionAnalyzer(
        #     games_basic_stats=self.games_basic_stats
        # )
        # self.batch_data_summarizer = BatchDataSummarizer(batch_dir=batch_dir)

    def get_game_summary_paths(self) -> Dict[str, Path]:
        paths = {}
        for path in self.batch_dir.iterdir():
            if path.is_dir():
                path_game_id = path.name.split("-")[0]
                paths[path_game_id] = path / f"{path_game_id}.json"

        return paths

    @property
    def tr_table_sizes_at_events(
        self,
    ) -> Dict[str, Dict[str, cdm.TranspositionTableSizesAtEvents]]:
        results = {}
        for color in [bindings.PieceColor.kRed, bindings.PieceColor.kBlk]:
            player_result = {
                game_summary.game_id: game_summary.get_player_summary(
                    player=color
                ).tr_table_sizes_at_events
                for game_summary in self.game_summaries
            }
            results[color.name] = player_result

        return results

    def get_sorted_tr_size_event_info(
        self,
        color: bindings.PieceColor,
        event: Literal["end_game", "first_collision"],
        size_type: Literal["num_entries", "num_states"],
        reverse: bool = False,
    ) -> tuple[tuple[str, int], ...]:
        result_list = []
        for game_summary in self.game_summaries:
            player_summary = game_summary.get_player_summary(player=color)
            events_info = player_summary.tr_table_sizes_at_events
            single_event_info: cdm.TranspositionTableSize = getattr(
                events_info, event
            )
            value = getattr(single_event_info, size_type)
            if value is not None:
                result_list.append((game_summary.game_id, value))
        result_list.sort(key=lambda x: x[1], reverse=reverse)

        return tuple(result_list)

    def tr_event_size_scan(
        self,
        color: bindings.PieceColor,
        size_type: Literal["num_entries", "num_states"],
    ) -> np.ndarray:
        first_collision_tuple = self.get_sorted_tr_size_event_info(
            color=color, event="first_collision", size_type=size_type
        )
        first_collision_dict = dict(first_collision_tuple)

        end_game_tuple = self.get_sorted_tr_size_event_info(
            color=color, event="end_game", size_type=size_type
        )

        num_rows = len(first_collision_tuple) + len(end_game_tuple) + 1
        num_cols = 3
        result = np.zeros(shape=(num_rows, num_cols), dtype=np.int64)

        size = num_with_collision_at_size = 0
        num_tables_at_size = len(end_game_tuple)

        result_idx = collision_idx = end_game_idx = 0

        result[result_idx, :] = [
            size,
            num_tables_at_size,
            num_with_collision_at_size,
        ]

        for result_idx in range(1, num_rows):
            if collision_idx < len(first_collision_tuple):
                next_first_collision = first_collision_tuple[collision_idx]
                next_end_game = end_game_tuple[end_game_idx]
                if next_first_collision[1] <= next_end_game[1]:
                    size = next_first_collision[1]
                    num_with_collision_at_size += 1
                    collision_idx += 1
                else:
                    size = next_end_game[1]
                    num_tables_at_size -= 1
                    end_game_idx += 1
                    if next_end_game[0] in first_collision_dict:
                        num_with_collision_at_size -= 1
            else:
                next_end_game = end_game_tuple[end_game_idx]
                size = next_end_game[1]
                num_tables_at_size -= 1
                end_game_idx += 1
                if next_end_game[0] in first_collision_dict:
                    num_with_collision_at_size -= 1

            result[result_idx, :] = [
                size,
                num_tables_at_size,
                num_with_collision_at_size,
            ]
        return result

    def load_game_summaries(self) -> List[GameSummary]:
        game_summary_paths = self.get_game_summary_paths()

        game_summaries = []
        for game_id, path in game_summary_paths.items():
            game_summaries.append(import_game_summary(path=path))

        return game_summaries

    @property
    def games_basic_stats(self) -> pd.DataFrame:
        return pd.DataFrame(
            [game_summary.basic_stats for game_summary in self.game_summaries],
            index=[
                game_summary.game_id for game_summary in self.game_summaries
            ],
        )

    @property
    def tr_table_size_df(self) -> pd.DataFrame:
        return self.games_basic_stats[
            [
                "kRed_tr_table_num_states_first_known_collision",
                "kRed_tr_table_num_entries_first_known_collision",
                "kRed_tr_table_num_states_final",
                "kRed_tr_table_num_entries_final",
                "kBlk_tr_table_num_states_first_known_collision",
                "kBlk_tr_table_num_entries_first_known_collision",
                "kBlk_tr_table_num_states_final",
                "kBlk_tr_table_num_entries_final",
            ]
        ]


if __name__ == "__main__":
    my_batch_dirs = [
        "20241023221443600060-BATCH-20-R-d3-k032-B-d3-k032",
        "20241023221537744237-BATCH-20-R-d3-k064-B-d3-k064",
        "20241023221623930853-BATCH-20-R-d3-k128-B-d3-k128",
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

    # multi_batch_summarizer = MultiBatchDataSummarizer(batch_dirs=my_batch_dirs)
    #
    # collision_analyzer_032bit = BatchCollisionAnalyzer(
    #     game_summaries=full_batch_summary_032_bit.game_summaries
    # )
    # collision_analyzer_032bit.plot_player_collision_data(
    #     color=bindings.PieceColor.kBlk
    # )

    my_result_a = full_batch_summary_032_bit.get_sorted_tr_size_event_info(
        color=bindings.PieceColor.kRed,
        event="first_collision",
        size_type="num_states",
    )
    my_result_b = full_batch_summary_064_bit.get_sorted_tr_size_event_info(
        color=bindings.PieceColor.kRed,
        event="end_game",
        size_type="num_states",
    )
    my_result_c = full_batch_summary_032_bit.tr_event_size_scan(
        color=bindings.PieceColor.kRed, size_type="num_states"
    )

    print("pause")
