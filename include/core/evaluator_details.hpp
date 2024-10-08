//! @file evaluator_details.hpp
//! Data structs used by moveselection::MinimaxEvaluator

#pragma once

#include <board_data_structs.hpp>
#include <move_data_structs.hpp>
#include <piece_points_bpo.hpp>
#include <chrono>
#include <map>

using namespace gameboard;
using namespace piece_points;

namespace moveselection {

//! A moves::MoveCollection in which all moves::Move have the same value. 
struct EqualScoreMoves {
  Points_t shared_score;
  moves::MoveCollection similar_moves;
};

struct RatedMove {
  moves::Move move;
  Points_t rating;
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

struct TranspositionTableEntry {
  int remaining_search_depth;
  MinimaxResultType result_type;
  EqualScoreMoves similar_moves;

  Points_t Score() { return similar_moves.shared_score; }
};

struct TranspositionTableSearchResult {
  TranspositionTableEntry table_entry;
  bool found;
};

inline EqualScoreMoves evaluate_win_leaf(PieceColor cur_player, PieceColor initiating_player) {
  auto empty_similar_moves = moves::MoveCollection();

  if (cur_player == initiating_player) {
    return EqualScoreMoves{numeric_limits<Points_t>::min(), empty_similar_moves};
  } else {
    return EqualScoreMoves{numeric_limits<Points_t>::max(), empty_similar_moves};
  }
}

// TODO: consider changing data from 2-D std::vector to 2-D std::array
struct ResultDepthCounts {

  ResultDepthCounts(int max_search_depth) {
    data.reserve(MinimaxResultType::kMax);
    for (auto idx = 0; idx <= MinimaxResultType::kMax; idx++) {
      data.emplace_back(max_search_depth + 1, 0);
    }
  }

  void Update(MinimaxResultType result_type, int search_depth) {
    data[result_type][search_depth]++;
  }

  std::vector<std::vector<int>> data;
};

struct SearchSummary {
  SearchSummary(int max_search_depth)
      : num_nodes{}
      , result_depth_counts{ResultDepthCounts(max_search_depth)}
      , transposition_table_hits(ResultDepthCounts(max_search_depth)) {
    // reserve a "row" for each result type
    // result_depth_counts.reserve(MinimaxResultType::kMax);
    // for (auto idx = 0; idx <= MinimaxResultType::kMax; idx++) {
    // for each "row", create vector long enough to hold each possible search depth
    // result_depth_counts.emplace_back(max_search_depth + 1, 0);
  }

  void Update(MinimaxResultType result_type, int search_depth, EqualScoreMoves similar_moves) {
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
  void SetEqualScoreMoves(EqualScoreMoves similar_moves) { this->similar_moves = similar_moves; }
  void SetSelectedMove(moves::Move selected_move) {
    this->selected_move = selected_move;
  }
  std::vector<std::vector<int>> GetResultDepthCounts() {
    return result_depth_counts.data;
  }
  std::vector<std::vector<int>> GetTranspositionTableHits() {
    return transposition_table_hits.data;
  }

  int num_nodes;
  std::chrono::duration<double, std::nano> time;
  // row -> node result_type; col -> node depth
  ResultDepthCounts result_depth_counts;
  ResultDepthCounts transposition_table_hits;
  EqualScoreMoves similar_moves;
  moves::Move selected_move;
};

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
} // namespace move_selection
