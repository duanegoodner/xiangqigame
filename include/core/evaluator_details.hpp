//! @file evaluator_details.hpp
//! Data structs used by moveselection::MinimaxEvaluator

#pragma once

#include <board_data_structs.hpp>
#include <chrono>
#include <map>
#include <move_data_structs.hpp>
#include <piece_points_bpo.hpp>

using namespace gameboard;
using namespace piece_points;

namespace moveselection {

//! Holds a moves::MoveCollection in which all moves::Move have the same value (as
//! perceived by a MoveEvaluator), and the value of the shared score.
struct EqualScoreMoves {
  Points_t shared_score;
  moves::MoveCollection similar_moves;
};

//! A moves::Move, and an associated score calculated by a MoveEvaluator.
struct ScoredMove {
  moves::Move move;
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
//! moveselection::BoardStateSummarizer.GetTrData.
struct TranspositionTableSearchResult {
  TranspositionTableEntry table_entry;
  bool found;
};

inline EqualScoreMoves evaluate_win_leaf(
    PieceColor cur_player,
    PieceColor initiating_player
) {
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
    // data.reserve(MinimaxResultType::kMax);
    for (auto &vec : data) {
      vec.resize(max_search_depth + 1);
    }

    // for (auto idx = 0; idx <= MinimaxResultType::kMax; idx++) {
    //   data.emplace_back(max_search_depth + 1, 0);
    // }
  }

  void Update(MinimaxResultType result_type, int search_depth) {
    data[result_type][search_depth]++;
  }

  std::array<std::vector<int>, MinimaxResultType::kMax + 1> data;
};

struct SearchSummary {
  SearchSummary(int max_search_depth)
      : num_nodes{}
      , result_depth_counts{ResultDepthCounts(max_search_depth)}
      , transposition_table_hits(ResultDepthCounts(max_search_depth)) {
  }

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
  void SetSelectedMove(moves::Move selected_move) {
    this->selected_move = selected_move;
  }
  std::array<std::vector<int>, MinimaxResultType::kMax + 1> GetResultDepthCounts() {
    return result_depth_counts.data;
  }
  std::array<std::vector<int>, MinimaxResultType::kMax + 1> GetTranspositionTableHits() {
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
} // namespace moveselection
