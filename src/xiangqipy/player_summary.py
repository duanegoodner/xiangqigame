"""
@file player_summary.py

Contains PlayerSummary class.
"""

from dataclasses import dataclass
from typing import Dict, List

import numpy as np
import pandas as pd
import xiangqi_bindings as bindings
from pandas.core.sample import process_sampling_size

import xiangqipy.core_dataclass_mirrors as cdm
from xiangqipy.enums import PlayerType, EvaluatorType
from xiangqipy.core_dataclass_mirrors import PointsT


@dataclass
class PlayerSummary:
    """
    Data container for data from one xiangqipy.game_interfaces.Player in a Game.

    @param color xiangi_bindings.PieceColoar: color of Player the summary belongs to.
    @param player_type xiangipy.enums.PlayerType: type of the Player.
    @param move_evaluator_type xiangqipy.enums.EvaluatorType: type of Player's move evaluator.
    @param max_search_depth int: maximum depth that a Minimax move evaluator will search.
    @param search_summaries xiangqipy.core_dataclass_mirrors.SearchSummaries:
    """

    color: bindings.PieceColor
    player_type: PlayerType
    move_evaluator_type: EvaluatorType = EvaluatorType.NULL
    max_search_depth: int = None
    zobrist_key_size: int = None
    search_summaries: cdm.SearchSummaries = None

    @property
    def has_search_summaries(self) -> bool:
        return self.search_summaries is not None

    @property
    def player_move_count(self) -> int:
        return len(self.search_summaries.first_searches)

    @property
    def game_move_numbers(self) -> List[int]:
        if self.color == bindings.PieceColor.kRed:
            return list(range(1, 2 * self.player_move_count, 2))
        if self.color == bindings.PieceColor.kBlk:
            return list(range(2, 2 * self.player_move_count + 1, 2))

    @property
    def first_searches_by_type_and_depth(
        self,
    ) -> Dict[str, pd.DataFrame] | None:
        """
        Dictionary with strings corresponding to
        xiangqi_bindings.MinimaxResultType values as keys, and DataFrame of
        result type counts as values.

        Each row of data frame -> move number, each col -> a value of
        remaining search depth when result was obtained.
        """
        if not self.has_search_summaries:
            return None
        result = {}
        for name, value in bindings.MinimaxResultType.__members__.items():
            new_df = pd.DataFrame(
                [
                    search_summary.result_depth_counts[value, :]
                    for search_summary in self.search_summaries.first_searches
                ]
            )
            new_df.index = self.game_move_numbers
            new_df.index.name = "game_move_numbers"
            new_df.columns = [
                f"remaining_depth={col_idx}"
                for col_idx in range(new_df.shape[1])
            ]
            result[name] = new_df

        return result

    @property
    def first_searches_by_type(self) -> pd.DataFrame | None:
        """
        Dataframe with row -> move number, col -> xiangqi_bindings.MinimaxResultType.
        """
        if not self.has_search_summaries:
            return None

        # Get the list of MinimaxResultType names
        result_type_names = list(bindings.MinimaxResultType.__members__.keys())
        num_result_types = len(result_type_names)

        # Preallocate the DataFrame with zeros (dtype=int64)
        num_first_searches = len(self.search_summaries.first_searches)
        df = pd.DataFrame(
            np.zeros(
                (num_first_searches, num_result_types), dtype=np.int64
            ),  # Preallocate with zeros as integers
            index=self.game_move_numbers,
            columns=result_type_names,
        )
        df.index.name = "game_move_numbers"

        # Populate the DataFrame by itescore over result types
        for idx, (name, value) in enumerate(
            bindings.MinimaxResultType.__members__.items()
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
    def tr_table_size_at_first_known_collision(
        self,
    ) -> cdm.TranspositionTableSize | None:
        if not self.has_search_summaries:
            return None
        if self.search_summaries.extra_searches:
            return self.search_summaries.extra_searches[
                min(self.search_summaries.extra_searches)
            ].tr_table_size_initial

    @property
    def tr_table_size_final(self) -> cdm.TranspositionTableSize | None:
        if not self.has_search_summaries:
            return None
        return self.search_summaries.first_searches[-1].tr_table_size_final

    @property
    def first_search_stats(self) -> pd.DataFrame | None:
        """
        Dataframe with row -> move number; cols -> number of nodes explored,
        total time for move selection, average time per node, and minimax
        evaluation score of the selected move.
        """
        if not self.has_search_summaries:
            return None

        num_nodes = np.array(
            [
                search_summary.num_nodes
                for search_summary in self.search_summaries.first_searches
            ]
        )

        search_time_s = np.array(
            [
                search_summary.time.total_seconds()
                for search_summary in self.search_summaries.first_searches
            ]
        )

        mean_time_per_node_ns = np.array(
            [
                search_summary.mean_time_per_node_ns
                for search_summary in self.search_summaries.first_searches
            ]
        )

        eval_score = np.array(
            [
                search_summary.similar_moves.shared_score
                for search_summary in self.search_summaries.first_searches
            ],
            dtype=PointsT,
        )

        tr_table_num_states = [
            search_summary.tr_table_size_final.num_states
            for search_summary in self.search_summaries.first_searches
        ]

        tr_table_num_entries = [
            search_summary.tr_table_size_final.num_entries
            for search_summary in self.search_summaries.first_searches
        ]

        returned_illegal_move = np.array(
            [
                search_summary.returned_illegal_move
                for search_summary in self.search_summaries.first_searches
            ]
        )

        cumulative_illegal_moves = np.cumsum(returned_illegal_move)

        df = pd.DataFrame(
            {
                "num_nodes": num_nodes,
                "search_time_s": search_time_s,
                "mean_time_per_node_ns": mean_time_per_node_ns,
                "eval_score": eval_score,
                "tr_table_num_states": tr_table_num_states,
                "tr_table_num_entries": tr_table_num_entries,
                "returned_illegal_move": returned_illegal_move,
                "cumulative_illegal_moves": cumulative_illegal_moves,
            },
            index=self.game_move_numbers,
        )
        df.index.name = "game_move_numbers"

        return df

    @property
    def selecton_stats(self) -> pd.Series | None:
        """
        Pandas Series with mean & max nodes per move, mean & max time per move,
        and number of known hash collisions.
        """

        if self.has_search_summaries:
            nodes_per_move = self.first_search_stats["num_nodes"].mean()
            time_per_move_s = self.first_search_stats["search_time_s"].mean()
            time_per_node_ns = self.first_search_stats[
                "mean_time_per_node_ns"
            ].mean()
            collisions_per_move = (
                len(self.search_summaries.extra_searches)
                / self.player_move_count
            )
            collisions_per_node = (
                len(self.search_summaries.extra_searches)
                / self.first_search_stats["num_nodes"].sum()
            )

            return pd.Series(
                [
                    self.max_search_depth,
                    self.zobrist_key_size,
                    nodes_per_move,
                    time_per_move_s,
                    time_per_node_ns,
                    collisions_per_move,
                    collisions_per_node,
                    (
                        self.tr_table_size_at_first_known_collision.num_states
                        if self.tr_table_size_at_first_known_collision
                        else None
                    ),
                    (
                        self.tr_table_size_at_first_known_collision.num_entries
                        if self.tr_table_size_at_first_known_collision
                        else None
                    ),
                    (
                        self.tr_table_size_final.num_states
                        if self.tr_table_size_final
                        else None
                    ),
                    (
                        self.tr_table_size_final.num_entries
                        if self.tr_table_size_final
                        else None
                    ),

                ],
                index=[
                    "search_depth",
                    "zobrist_key_size",
                    "nodes_per_move",
                    "time_per_move_s",
                    "time_per_node_ns",
                    "collisions_per_move",
                    "collisions_per_node",
                    "tr_table_num_states_first_known_collision",
                    "tr_table_num_entries_first_known_collision",
                    "tr_table_num_states_final",
                    "tr_table_num_entries_final",
                ],
            )
