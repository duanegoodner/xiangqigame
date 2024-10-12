//! @file evaluator_data_structs.hpp
//! Data structs used by moveselection::MinimaxEvaluator

#pragma once

#include <board_data_structs.hpp>
#include <chrono>
#include <map>
#include <move_data_structs.hpp>
#include <piece_points_bpo.hpp>

using namespace gameboard;
using namespace piecepoints;

namespace moveselection {

//! Holds a gameboard::MoveCollection in which all gameboard::Move have the same value (as
//! perceived by a MoveEvaluator), and the value of the shared score.
struct EqualScoreMoves {
  Points_t shared_score;
  MoveCollection similar_moves;
};

//! A gameboard::Move, and an associated score calculated by a MoveEvaluator.
struct ScoredMove {
  Move move;
  Points_t score;
};

enum MinimaxResultType : size_t {
  kUnknown = 0,
  kTrTableHit = 1,
  kEvaluatorLoses = 4,
  kEvaluatorWins = 5,
  kFullyEvaluatedNode = 6,
  kStandardLeaf = 7,
  kAlphaPrune = 8,
  kBetaPrune = 9,
  kMin = kUnknown,
  kMax = kBetaPrune
};
const size_t kNumResultTypes{7};

//! Data structure that holds a moveselection::EqualScoreMoves and other search-related
//! info obtained from a call to moveselection::MinimaxMoveEvaluator.MinimaxRec.
struct TranspositionTableEntry {
  int remaining_search_depth;
  MinimaxResultType result_type;
  EqualScoreMoves similar_moves;

  Points_t Score() { return similar_moves.shared_score; }
};

//! Container for storing a moveselection::TranspositionTableEntry retrieved by a call to
//! boardstate::HashCalculator.ImplementGetTrData.
struct TranspositionTableSearchResult {
  TranspositionTableEntry table_entry;
  bool found;
};

inline EqualScoreMoves evaluate_win_leaf(
    PieceColor cur_player,
    PieceColor initiating_player
) {
  auto empty_similar_moves = MoveCollection();

  if (cur_player == initiating_player) {
    return EqualScoreMoves{numeric_limits<Points_t>::min(), empty_similar_moves};
  } else {
    return EqualScoreMoves{numeric_limits<Points_t>::max(), empty_similar_moves};
  }
}

//! Array of vectors for storing counts of moveselection::MinimaxResultType for each
//! posible remaining search depth. Outer (array) index ->
//! moveslection::MinimaxResultType; inner (vector) index -> remaining search depth when
//! result was obtained.
typedef std::array<std::vector<int>, MinimaxResultType::kMax + 1>
    ResultDepthCountsData_t;

//! Container for storing and updating data in a moveselection::ResultDepthCountsData_t
//! array of vectors.
struct ResultDepthCounts {

  ResultDepthCounts(int max_search_depth) {
    for (auto &vec : data) {
      vec.resize(max_search_depth + 1);
    }
  }

  void Update(MinimaxResultType result_type, int search_depth) {
    data[result_type][search_depth]++;
  }

  ResultDepthCountsData_t data;
};

//! Stores data collected during a single call to
//! moveselection::MinimaxMoveEvaluator.ImplementSelectMove. Includes total number of
//! nodes explored, a moveseelection::ResultDepthCounts object for
//! moveselection::MinimaxResultType::kTrTableHit search results, and another
//! moveseelection::ResultDepthCounts  for all other search result types.
struct SearchSummary {
  SearchSummary(int max_search_depth)
      : num_nodes{}
      , result_depth_counts{ResultDepthCounts(max_search_depth)}
      , transposition_table_hits(ResultDepthCounts(max_search_depth)) {}

  void Update(
      MinimaxResultType result_type,
      int search_depth,
      EqualScoreMoves similar_moves
  ) {
    // result_depth_counts[result_type][search_depth]++;
    result_depth_counts.Update(result_type, search_depth);
    num_nodes++;
    SetEqualScoreMoves(similar_moves);
  }

  void UpdateTranspositionTableHits(MinimaxResultType result_type, int search_depth) {
    transposition_table_hits.Update(result_type, search_depth);
  }

  void SetTime(std::chrono::duration<double, std::nano> search_time) {
    time = search_time;
  }
  void SetEqualScoreMoves(EqualScoreMoves similar_moves) {
    this->similar_moves = similar_moves;
  }
  void SetSelectedMove(Move selected_move) {
    this->selected_move = selected_move;
  }
  ResultDepthCountsData_t GetResultDepthCounts() { return result_depth_counts.data; }
  ResultDepthCountsData_t GetTranspositionTableHits() {
    return transposition_table_hits.data;
  }

  int num_nodes;
  std::chrono::duration<double, std::nano> time;
  ResultDepthCounts result_depth_counts;
  ResultDepthCounts transposition_table_hits;
  EqualScoreMoves similar_moves;
  Move selected_move;
};

//! Stores a moveselection::SearchSummary for each
//! moveselection::MinimaxMoveEvaluator.ImplementSelectMove made for a particular player
//! during a game.
//! If a second call to moveselection::MinimaxMoveEvaluator.RunMinimax is needed (due to
//! a hash collision causing in illegal move to be returned by the first call to to
//! moveselection::MinimaxMoveEvaluator.RunMinimax), a moveselection::SearchSummary for
//! the second search is stored in moveselection::SearchSummaries.extra_searches.
struct SearchSummaries {
  std::vector<SearchSummary> first_searches;
  std::map<int, SearchSummary> extra_searches;

  SearchSummary &NewFirstSearch(int search_depth) {
    first_searches.emplace_back(SearchSummary(search_depth));
    return first_searches.back();
  }

  SearchSummary &NewExtraSearch(int search_depth, int search_number) {
    auto new_search_entry =
        extra_searches.emplace(search_number, SearchSummary(search_depth));
    return new_search_entry.first->second;
  }
};
} // namespace moveselection
