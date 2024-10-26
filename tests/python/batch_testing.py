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

    def get_transposition_table_events(self, color: bindings.PieceColor):
        result_list = []

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

    def scan_tr_event_size(
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
    def tr_event_size_data(self) -> dict[str, pd.DataFrame]:
        result = {}
        for color in [bindings.PieceColor.kRed, bindings.PieceColor.kBlk]:
            result[color.name] = self.scan_tr_event_size(color=color)
        return result

    def plot_tr_event_size_data(self):
        for color in [bindings.PieceColor.kRed, bindings.PieceColor.kBlk]:
            if any(
                [
                    game_summary.get_player_summary(
                        player=color
                    ).has_search_summaries
                    for game_summary in self.game_summaries
                ]
            ):
                df = self.tr_event_size_data[color.name]
                fig, ax1 = plt.subplots()
                ax1.set_ylabel("Fraction remaining tables at size")
                ax1.plot(df["table_size"], df["fraction_orig_tables"])
                ax2 = ax1.twinx()
                ax2.set_ylabel("Fraction tables with collision")
                ax2.plot(df["table_size"], df["fraction_tables_with_collision"])

                plt.show()


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

    full_batch_summary_032_bit.plot_tr_event_size_data()
    full_batch_summary_064_bit.plot_tr_event_size_data()
    full_batch_summary_128_bit.plot_tr_event_size_data()

    print("pause")
