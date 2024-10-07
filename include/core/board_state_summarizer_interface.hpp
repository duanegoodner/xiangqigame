#pragma once

#include <evaluator_details.hpp>

using  namespace gameboard;
using namespace moves;


// CRTP Interface: Evaluator <- BoardStateSummarizer (e.g. HashCalculator)
template <typename ConcreteBoardStateSummarizer, typename KeyType>
class BoardStateSummarizer {
public:
  typedef KeyType ZobristKey_t;
  void FullBoardStateCalc(BoardMap_t &board_map) {
    static_cast<ConcreteBoardStateSummarizer *>(this)->ImplementFullBoardStateCalc(
        board_map
    );
  }

  void UpdateBoardState(const ExecutedMove &move) {
    return static_cast<ConcreteBoardStateSummarizer *>(this)->ImplementUpdateBoardState(
        move
    );
  }

  ZobristKey_t GetState() {
    return static_cast<ConcreteBoardStateSummarizer *>(this)->ImplementGetState();
  }

  void RecordTrData(
      int search_depth,
      moveselection::MinimaxResultType result_type,
      moveselection::BestMoves &best_moves
  ) {
    return static_cast<ConcreteBoardStateSummarizer *>(this)
        ->ImplementRecordTrData(search_depth, result_type, best_moves);
  }

  moveselection::TranspositionTableSearchResult GetTrData(int search_depth) {
    return static_cast<ConcreteBoardStateSummarizer *>(this)->ImplementGetTrData(
        search_depth
    );
  }
};