//! @file board_state_summarizer_interface.hpp
//! Definition of BoardStateSummarizer

#pragma once

#include <evaluator_data_structs.hpp>

using namespace gameboard;

//! CRTP Interface with methods to calculate / read / update hash values representing a
//! board state; and storing and retreiving data from completed calculations of scores
//! associated with specific board states (concrete example =
//! boardstate::HashCalculator).
template <typename ConcreteBoardStateSummarizer, typename KeyType>
class BoardStateSummarizer {
public:
  typedef KeyType ZobristKey_t;
  void FullBoardStateCalc(const BoardMap_t &board_map) {
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
      moveselection::EqualScoreMoves &similar_moves
  ) {
    return static_cast<ConcreteBoardStateSummarizer *>(this)
        ->ImplementRecordTrData(search_depth, result_type, similar_moves);
  }

  moveselection::TranspositionTableSearchResult GetTrData(int search_depth) {
    return static_cast<ConcreteBoardStateSummarizer *>(this)->ImplementGetTrData(
        search_depth
    );
  }

  moveselection::TranspositionTableSize GetTrTableSize() {
    return static_cast<ConcreteBoardStateSummarizer *>(this)->ImplementGetTrTableSize();
  }
};