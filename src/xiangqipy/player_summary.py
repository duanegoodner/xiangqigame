"""
Contains PlayerSummary class.
"""

from dataclasses import dataclass
from typing import Dict, List

import numpy as np
import pandas as pd
import xiangqi_bindings as xb

import xiangqipy.core_dataclass_mirrors as cdm
from xiangqipy.enums import PlayerType, EvaluatorType
from xiangqipy.core_dataclass_mirrors import PointsT


@dataclass
class PlayerSummary:
    """
    Data container for data from one xiangqipy.game_interfaces.Player in a Game.

    @param color xiangi_xb.PieceColoar: color of Player the summary belongs to.
    @param player_type xiangipy.enums.PlayerType: type of the Player.
    @param move_evaluator_type xiangqipy.enums.EvaluatorType: type of Player's move evaluator.
    @param max_search_depth int: maximum depth that a Minimax move evaluator will search.
    @param search_summaries xiangqipy.core_dataclass_mirrors.SearchSummaries:
    container with summary data of each Move selected by Player.
    """

    color: xb.PieceColor
    player_type: PlayerType
    move_evaluator_type: EvaluatorType = EvaluatorType.NULL
    max_search_depth: int = None
    zobrist_key_size: int = None
    zkeys_seed: int = None
    search_summaries: cdm.SearchSummaries = None

    @property
    def has_search_summaries(self) -> bool:
        """
        Property indicating if PlayerSummary object has search summaries.

        Expect to be True for MinimaxMoveEvaluators, False otherwise.
        """
        return self.search_summaries is not None

    @property
    def player_move_count(self) -> int:
        """
        Number of moves selected by Player during Game.
        """
        return len(self.search_summaries.first_searches)

    @property
    def game_move_numbers(self) -> List[int]:
        """
        Converts Players' move numbers in to overall Game Move numbers
        (red = odd ints, black = even ints).
        """
        if self.color == xb.PieceColor.kRed:
            return list(range(1, 2 * self.player_move_count, 2))
        if self.color == xb.PieceColor.kBlk:
            return list(range(2, 2 * self.player_move_count + 1, 2))

    @property
    def first_searches_by_type_and_depth(
        self,
    ) -> Dict[str, pd.DataFrame] | None:
        """
        Dictionary with strings corresponding to
        xiangqi_xb.MinimaxResultType values as keys, and DataFrame of
        result type counts as values.

        Each row of data frame -> move number, each col -> a value of
        remaining search depth when result was obtained.
        """
        if not self.has_search_summaries:
            return None
        result = {}
        for name, value in xb.MinimaxResultType.__members__.items():
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
        Dataframe with row -> move number, col -> xiangqi_xb.MinimaxResultType.

        Named 'first_searches...' in constrast to 'extra_searches...' or
        'second_searches...' which would occur if first_search of
        MinimaxMoveEvaluator.select_move returns an illegal move. After fixing bug
        that was allowing moves that violated repeated move rule to be returned
        by first search, have never recorded any second searches.
        """
        if not self.has_search_summaries:
            return None

        # Get the list of MinimaxResultType names
        result_type_names = list(xb.MinimaxResultType.__members__.keys())
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
            xb.MinimaxResultType.__members__.items()
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
    def tr_table_size_first_illegal_move_request(
        self,
    ) -> int | None:
        """
        Transposition table size the first time any illegal move was returned
        by first search. This has been NaN after fixing first search's repeat
        move rule bug.
        """
        if self.has_search_summaries and self.search_summaries.extra_searches:
            return self.search_summaries.extra_searches[
                min(self.search_summaries.extra_searches)
            ].tr_table_size_initial

    @property
    def tr_table_size_end_game(self) -> int | None:
        """
        Size of transposition table at end of game.
        """
        if self.has_search_summaries:
            return self.search_summaries.first_searches[-1].tr_table_size_final

    @property
    def tr_table_sizes_at_events(self) -> cdm.TranspositionTableSizesAtEvents:
        """
        Transposition table size at first illegal move and end of game wrapped
        into single convenience property.
        """
        return cdm.TranspositionTableSizesAtEvents(
            first_illegal_move_request=self.tr_table_size_first_illegal_move_request,
            end_game=self.tr_table_size_end_game,
        )

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
                search_summary.equal_score_moves.shared_score
                for search_summary in self.search_summaries.first_searches
            ],
            dtype=PointsT,
        )

        tr_table_size = [
            search_summary.tr_table_size_final
            for search_summary in self.search_summaries.first_searches
        ]

        returned_illegal_move = np.array(
            [
                search_summary.returned_illegal_move
                for search_summary in self.search_summaries.first_searches
            ]
        )

        num_collisions = np.array(
            [
                search_summary.num_collisions
                for search_summary in self.search_summaries.first_searches
            ]
        )

        # cumulative_illegal_moves = np.cumsum(returned_illegal_move)

        df = pd.DataFrame(
            {
                "num_nodes": num_nodes,
                "search_time_s": search_time_s,
                "mean_time_per_node_ns": mean_time_per_node_ns,
                "eval_score": eval_score,
                "tr_table_size": tr_table_size,
                # "tr_table_num_entries": tr_table_num_entries,
                "returned_illegal_move": returned_illegal_move,
                "num_collisions": num_collisions,
                # "cumulative_illegal_moves": cumulative_illegal_moves,
            },
            index=self.game_move_numbers,
        )
        df.index.name = "game_move_numbers"

        return df

    @property
    def selection_stats(self) -> pd.Series | None:
        """
        Pandas Series with mean & max nodes per move, mean & max time per move,
        and number of illegal move requests.
        """

        if self.has_search_summaries:
            nodes_per_move = self.first_search_stats["num_nodes"].mean()
            time_per_move_s = self.first_search_stats["search_time_s"].mean()
            time_per_node_ns = self.first_search_stats[
                "mean_time_per_node_ns"
            ].mean()
            num_illegal_move_requests = len(
                self.search_summaries.extra_searches
            )
            num_collisions = self.first_search_stats["num_collisions"].sum()
            collisions_per_move = (
                self.first_search_stats["num_collisions"]
            ).sum() / self.player_move_count
            collisions_per_node = (
                self.first_search_stats["num_collisions"].sum()
                / self.first_search_stats["num_nodes"].sum()
            )

            return pd.Series(
                [
                    self.max_search_depth,
                    self.zobrist_key_size,
                    self.zkeys_seed,
                    nodes_per_move,
                    time_per_move_s,
                    time_per_node_ns,
                    num_illegal_move_requests,
                    num_collisions,
                    collisions_per_move,
                    collisions_per_node,
                    # self.tr_table_size_first_illegal_move_request,
                    self.tr_table_size_first_illegal_move_request,
                    # self.tr_table_size_end_game,
                    self.tr_table_size_end_game,
                ],
                index=[
                    "search_depth",
                    "zobrist_key_size",
                    "zkeys_seed",
                    "nodes_per_move",
                    "time_per_move_s",
                    "time_per_node_ns",
                    "num_illegal_move_requests",
                    "num_collisions",
                    "collisions_per_move",
                    "collisions_per_node",
                    # "tr_table_num_states_first_illegal_move_request",
                    "tr_table_size_first_illegal_move_request",
                    # "tr_table_num_states_end_game",
                    "tr_table_size_end_game",
                ],
            )
