from dataclasses import dataclass
from enum import Enum, auto
from typing import Dict, List

import numpy as np
import pandas as pd
import xiangqigame_core as core

import xiangqigame.core_dataclass_mirrors as cdm
from xiangqigame.enums import PlayerType, EvaluatorType
from xiangqigame.core_dataclass_mirrors import PointsT


@dataclass
class PlayerSummary:
    color: core.PieceColor
    player_type: PlayerType
    move_evaluator_type: EvaluatorType = EvaluatorType.NULL
    max_search_depth: int = None
    zobrist_key_size: int = None
    search_summaries: cdm.SearchSummaries = None

    @property
    def has_search_summaries(self) -> bool:
        return self.search_summaries is not None

    def get_game_move_numbers(self, player_move_count: int) -> List[int]:
        if self.color == core.PieceColor.kRed:
            return list(range(1, 2 * player_move_count, 2))
        if self.color == core.PieceColor.kBlk:
            return list(range(2, 2 * player_move_count + 1, 2))

    @property
    def first_searches_by_type_and_depth(
        self,
    ) -> Dict[str, pd.DataFrame] | None:
        if not self.has_search_summaries:
            return None
        result = {}
        for name, value in core.MinimaxResultType.__members__.items():
            new_df = pd.DataFrame(
                [
                    search_summary.result_depth_counts[value, :]
                    for search_summary in self.search_summaries.first_searches
                ]
            )
            new_df.index = self.get_game_move_numbers(
                player_move_count=len(new_df)
            )
            new_df.index.name = "game_move_numbers"
            new_df.columns = [
                f"remaining_depth={col_idx}"
                for col_idx in range(new_df.shape[1])
            ]
            result[name] = new_df

        return result

    @property
    def first_searches_by_type(self) -> pd.DataFrame | None:
        if not self.has_search_summaries:
            return None

        # Get the list of MinimaxResultType names
        result_type_names = list(core.MinimaxResultType.__members__.keys())
        num_result_types = len(result_type_names)

        # Preallocate the DataFrame with zeros (dtype=int64)
        num_first_searches = len(self.search_summaries.first_searches)
        df = pd.DataFrame(
            np.zeros(
                (num_first_searches, num_result_types), dtype=np.int64
            ),  # Preallocate with zeros as integers
            index=self.get_game_move_numbers(
                player_move_count=num_first_searches
            ),
            columns=result_type_names,
        )
        df.index.name = "game_move_numbers"

        # Populate the DataFrame by iterating over result types
        for idx, (name, value) in enumerate(
            core.MinimaxResultType.__members__.items()
        ):
            for search_idx, search_summary in enumerate(
                self.search_summaries.first_searches
            ):
                # Sum the result_depth_counts and set the value in the DataFrame
                df.iloc[search_idx, idx] = sum(
                    search_summary.result_depth_counts[value][:]
                )

        assert df["Unknown"].sum() == 0
        df.drop(columns=["Unknown"], inplace=True)

        return df

    @property
    def first_search_stats(self) -> pd.DataFrame | None:
        if not self.has_search_summaries:
            return None

        num_nodes = np.array([
            search_summary.num_nodes
            for search_summary in self.search_summaries.first_searches
        ])

        search_time_s = np.array([
            search_summary.time.total_seconds()
            for search_summary in self.search_summaries.first_searches
        ])

        mean_time_per_node_ns = np.array([
            search_summary.mean_time_per_node_ns
            for search_summary in self.search_summaries.first_searches
        ])

        eval_score = np.array([
            search_summary.similar_moves.shared_score
            for search_summary in self.search_summaries.first_searches
        ], dtype=PointsT)

        assert (
            len(num_nodes)
            == len(search_time_s)
            == len(mean_time_per_node_ns)
            == len(eval_score)
        )

        df = pd.DataFrame(
            {
                "num_nodes": num_nodes,
                "search_time_s": search_time_s,
                "mean_time_per_node_ns": mean_time_per_node_ns,
                "eval_score": eval_score,
            },
            index=self.get_game_move_numbers(player_move_count=len(num_nodes)),
        )
        df.index.name = "game_move_numbers"

        return df

