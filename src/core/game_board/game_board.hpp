// Filename: game_board.hpp
// Author: Duane Goodner
// Created: 2022-11-14
// Last Modified: 2024-08-16

// Description:
// Definition of GameBoard class, and the BoardStateSummarizer CRTP interface
// that a GameBoard requires a HashCalculator to comply with.

#ifndef _GAME_BOARD_
#define _GAME_BOARD_

#include <board_components.hpp>
#include <common.hpp>
#include <game_board_details.hpp>
#include <move_calculator.hpp>
#include <move_evaluators.hpp>
#include <vector>

using namespace std;
using namespace board_components;

// CRTP INTERFACE: GameBoard <- BoardStateSummarizer (concrete example =
// HashCalculator)
template <typename ConcreteBoardStateSummarizer, typename KeyType>
class BoardStateSummarizer {
public:
  typedef KeyType ZobristKey_t;
  void CalcInitialBoardState(BoardMap_t &board_map) {
    static_cast<ConcreteBoardStateSummarizer *>(this)
        ->ImplementCalcInitialBoardState(board_map);
  }

  void CalcNewBoardState(const ExecutedMove &move) {
    return static_cast<ConcreteBoardStateSummarizer *>(this)
        ->ImplementCalcNewBoardState(move);
  }

  ZobristKey_t GetState() {
    return static_cast<ConcreteBoardStateSummarizer *>(this)
        ->ImplementGetState();
  }

  void RecordCurrentStateMinimaxResult(
      int search_depth,
      MinimaxResultType result_type,
      BestMoves &best_moves
  ) {
    return static_cast<ConcreteBoardStateSummarizer *>(this)
        ->ImplementRecordCurrentStateMinimaxResult(
            search_depth,
            result_type,
            best_moves
        );
  }

  TranspositionTableSearchResult GetCurrentStateMinimaxResult(int search_depth
  ) {
    return static_cast<ConcreteBoardStateSummarizer *>(this)
        ->ImplementGetCurrentStateMinimaxResult(search_depth);
  }
};

// Template for class NewGameBoard which implements interface
// SpaceInfoProvider, and uses a ConcreteBoardStateSummarizer
template <
    typename ConcreteBoardStateSummarizer,
    typename ConcreteBoardStateSummarizerRed,
    typename ConcreteBoardStateSummarizerBlack>
class NewGameBoard : public SpaceInfoProvider<NewGameBoard<
                         ConcreteBoardStateSummarizer,
                         ConcreteBoardStateSummarizerRed,
                         ConcreteBoardStateSummarizerBlack>> {
public:
  NewGameBoard();
  NewGameBoard(const BoardMapInt_t starting_board);
  vector<BoardSpace> ImplementGetAllSpacesOccupiedBy(PieceColor color);
  PieceColor ImplementGetColor(BoardSpace space);
  PieceType ImplementGetType(BoardSpace space);

  // Transposition table search old & new
  TranspositionTableSearchResult ImplementSearchTranspositionTable(
      PieceColor color,
      int search_depth
  );

  // Transposition table write
  void ImplementRecordCurrentStateScore(
      PieceColor color,
      int search_depth,
      MinimaxResultType result_type,
      BestMoves &best_moves
  );

  MoveCollection ImplementCalcFinalMovesOf(PieceColor color);
  bool IsInCheck(PieceColor color);
  ExecutedMove ImplementExecuteMove(Move move);
  void ImplementUndoMove(ExecutedMove executed_move);
  GamePiece GetOccupant(BoardSpace space);
  const BoardMap_t &map() const { return board_map_; }

private:
  BoardMap_t board_map_;
  MoveCalculator move_calculator_;

  // ConcreteBoardStateSummarizer hash_calculator_;
  ConcreteBoardStateSummarizerRed hash_calculator_red_;
  ConcreteBoardStateSummarizerBlack hash_calculator_black_;

  // Retrieve info for current state //////////////////////////////////////////
  TranspositionTableSearchResult GetCurrentStateDetailsRed(int search_depth) {
    return hash_calculator_red_.GetCurrentStateMinimaxResult(search_depth);
  };
  TranspositionTableSearchResult GetCurrentStateDetailsBlack(int search_depth
  ) {
    return hash_calculator_black_.GetCurrentStateMinimaxResult(search_depth);
  }
  unordered_map<
      PieceColor,
      TranspositionTableSearchResult (NewGameBoard::*)(int)>
      state_details_dispatch_table_;
  //////////////////////////////////////////////////////////////////////////////

  // Record info for current state/////////////////////////////////////////////
  void RecordCurrentStateDetailsRed(
      int search_depth,
      MinimaxResultType result_type,
      BestMoves &best_moves
  ) {
    hash_calculator_red_.RecordCurrentStateMinimaxResult(
        search_depth,
        result_type,
        best_moves
    );
  };
  void RecordCurrentStateDetailsBlack(
      int search_depth,
      MinimaxResultType result_type,
      BestMoves &best_moves
  ) {
    hash_calculator_black_.RecordCurrentStateMinimaxResult(
        search_depth,
        result_type,
        best_moves
    );
  };

  unordered_map<
      PieceColor,
      void (NewGameBoard::*)(int, MinimaxResultType, BestMoves &)>
      write_state_details_dispatch_table_;
/////////////////////////////////////////////////////////////////////////////////

  std::map<PieceColor, vector<ExecutedMove>> move_log_;
  void UpdateHashCalculator(ExecutedMove executed_move) {
    // hash_calculator_.CalcNewBoardState(executed_move);
    hash_calculator_red_.CalcNewBoardState(executed_move);
    hash_calculator_black_.CalcNewBoardState(executed_move);
  };
  void SetOccupant(BoardSpace space, GamePiece piece);
  void AddToMoveLog(ExecutedMove move);
  void RemoveFromMoveLog(ExecutedMove move);
  bool ViolatesRepeatRule(PieceColor color);
};

#include <game_board.tpp>

#endif // _GAME_BOARD_