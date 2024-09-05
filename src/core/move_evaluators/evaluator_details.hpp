#pragma once

#include <board_components.hpp>
#include <common.hpp>

using namespace board_components;

struct BestMoves {
  Points_t best_eval;
  MoveCollection best_moves;
};

struct RatedMove {
  Move move;
  Points_t rating;
};

enum MinimaxResultType : int {
  kUnknown = 0,
  kFullyEvaluatedNode = 1,
  kStandardLeaf = 2,
  kEndGameLeaf = 3,
  kAlphaPrune = 4,
  kBetaPrune = 5
};

struct TranspositionTableEntry {
  // zkey_t state;
  int search_depth;
  MinimaxResultType result_type;
  BestMoves best_moves;

  Points_t Score() { return best_moves.best_eval; }
};

struct TranspositionTableSearchResult {
  TranspositionTableEntry table_entry;
  bool found;
};

inline BestMoves evaluate_win_leaf(
    PieceColor cur_player,
    PieceColor initiating_player
) {
  auto empty_best_moves = MoveCollection();

  if (cur_player == initiating_player) {
    return BestMoves{numeric_limits<Points_t>::min(), empty_best_moves};
  } else {
    return BestMoves{numeric_limits<Points_t>::max(), empty_best_moves};
  }
}
