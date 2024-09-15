#pragma once

#include <board_components.hpp>
#include <chrono>
#include <common.hpp>
#include <map>

using namespace board_components;

struct BestMoves {
  Points_t best_eval;
  MoveCollection best_moves;
};

struct RatedMove {
  Move move;
  Points_t rating;
};

enum MinimaxResultType : size_t {
  kUnknown = 0,
  kTrTableHitStandard = 1,
  kTrTableHitEvaluatorLoses = 2,
  kTrTableHitEvaluatorWins = 3,
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
  int search_depth;
  MinimaxResultType result_type;
  BestMoves best_moves;

  Points_t Score() { return best_moves.best_eval; }
};

struct TranspositionTableSearchResult {
  TranspositionTableEntry table_entry;
  bool found;
};

inline BestMoves evaluate_win_leaf(PieceColor cur_player, PieceColor initiating_player) {
  auto empty_best_moves = MoveCollection();

  if (cur_player == initiating_player) {
    return BestMoves{numeric_limits<Points_t>::min(), empty_best_moves};
  } else {
    return BestMoves{numeric_limits<Points_t>::max(), empty_best_moves};
  }
}

struct SearchSummary {
  SearchSummary(int max_search_depth)
      : num_nodes{}
      , result_depth_counts{} {
    // reserve a "row" for each result type
    result_depth_counts.reserve(MinimaxResultType::kMax);
    for (auto idx = 0; idx <= MinimaxResultType::kMax; idx++) {
      // for each "row", create vector long enough to hold each possible search depth
      result_depth_counts.emplace_back(max_search_depth + 1, 0);
    }
  }

  void Update(MinimaxResultType result_type, int search_depth) {
    result_depth_counts[result_type][search_depth]++;
    num_nodes++;
  }

  void SetTime(std::chrono::duration<double, std::nano> search_time) {
    time = search_time;
  }

  int num_nodes;
  std::chrono::duration<double, std::nano> time;

  // row -> node result_type; col -> node depth
  std::vector<std::vector<int>> result_depth_counts;
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
