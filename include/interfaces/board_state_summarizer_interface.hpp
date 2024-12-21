//! @file interfaces/board_state_summarizer_interface.hpp
//! Definition of BoardStateCoordinator

#pragma once

#include <moveselection/evaluator_data_structs.hpp>
#include <utilities/integer_types.hpp>

using namespace gameboard;

//! CRTP Interface with methods to calculate / read / update hash values representing a
//! board state; and storing and retreiving data from completed calculations of scores
//! associated with specific board states (concrete example =
//! boardstate::SingleZobristCoordinator).
template <typename ConcreteBoardStateCoordinator, typename KeyType>
class BoardStateCoordinator {
public:
  typedef KeyType ZobristKey_t;
  void FullBoardStateCalc(const BoardMap_t &board_map) {
    static_cast<ConcreteBoardStateCoordinator *>(this)->ImplementFullBoardStateCalc(
        board_map
    );
  }

  void UpdateBoardState(const ExecutedMove &move) {
    return static_cast<ConcreteBoardStateCoordinator *>(this)->ImplementUpdateBoardState(
        move
    );
  }

  ZobristKey_t GetState() {
    return static_cast<ConcreteBoardStateCoordinator *>(this)->ImplementGetState();
  }

  void RecordTrData(
      DepthType search_depth,
      moveselection::MinimaxResultType result_type,
      moveselection::EqualScoreMoves &similar_moves,
      MoveCountType access_index
  ) {
    return static_cast<ConcreteBoardStateCoordinator *>(this)
        ->ImplementRecordTrData(search_depth, result_type, similar_moves, access_index);
  }

  moveselection::TranspositionTableSearchResult GetTrData(
      DepthType search_depth,
      MoveCountType access_index
  ) {
    return static_cast<ConcreteBoardStateCoordinator *>(this)->ImplementGetTrData(
        search_depth,
        access_index
    );
  }

  size_t GetTrTableSize() {
    return static_cast<ConcreteBoardStateCoordinator *>(this)->ImplementGetTrTableSize();
  }

  void UpdateMoveCounter() {
    return static_cast<ConcreteBoardStateCoordinator *>(this)->ImplementUpdateMoveCounter(
    );
  }
};