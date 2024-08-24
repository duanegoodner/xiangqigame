// Filename: game_board.hpp
// Author: Duane Goodner
// Created: 2022-11-14
// Last Modified: 2024-08-16

// Description:
// Definition of GameBoard class, and the BoardStateSummarizer CRTP interface that
// a GameBoard requires a HashCalculator to comply with.

#ifndef _GAME_BOARD_
#define _GAME_BOARD_

#include <board_components.hpp>
#include <common.hpp>
#include <game_board_details.hpp>
#include <minimax_evaluator.hpp>
#include <move_calculator.hpp>
#include <move_selector.hpp>
#include <vector>

using namespace std;
using namespace board_components;

// CRTP INTERFACE: GameBoard <- BoardStateSummarizer (concrete example = HashCalculator)
template <typename ConcreteBoardStateSummarizer>
class BoardStateSummarizer {
public:
  void CalcInitialBoardState(BoardMap_t &board_map) {
    static_cast<ConcreteBoardStateSummarizer *>(this)
        ->ImplementCalcInitialBoardState(board_map);
  }
  void CalcNewBoardState(const ExecutedMove &move) {
    return static_cast<ConcreteBoardStateSummarizer *>(this)
        ->ImplementCalcNewBoardState(move);
  }
  zkey_t GetState() {
    return static_cast<ConcreteBoardStateSummarizer *>(this)->ImplementGetState();
  }
};

// Template class for a GameBoard that has a ConcreteBoardStateSummarizer, and
// implements the following interfaces: MoveTracker (specified by MoveSelector)
// SpaceInfoProvider (specified by MinimaxEvaluator)
template <typename ConcreteBoardStateSummarizer>
class GameBoard : public MoveTracker<
                      GameBoard<ConcreteBoardStateSummarizer>,
                      ConcreteBoardStateSummarizer>,
                  public SpaceInfoProvider<GameBoard<ConcreteBoardStateSummarizer>> {

public:
  GameBoard();
  GameBoard(const BoardMapInt_t starting_board);
  bool IsOccupied(BoardSpace space);
  GamePiece GetOccupant(BoardSpace space);
  ExecutedMove ImplementExecuteMove(Move move);
  void ImplementUndoMove(ExecutedMove executed_move);
  vector<BoardSpace> ImplementGetAllSpacesOccupiedBy(PieceColor color);
  MoveCollection ImplementCalcFinalMovesOf(PieceColor color);
  bool IsInCheck(PieceColor color);
  PieceColor ImplementGetColor(BoardSpace space);
  PieceType ImplementGetType(BoardSpace space);
  TranspositionTableSearchResult ImplementSearchTranspositionTable(
      PieceColor color,
      int search_depth
  );
  void ImplementRecordCurrentStateScore(
      PieceColor color,
      int search_depth,
      MinimaxResultType result_type,
      BestMoves &best_moves
  );
  const BoardMap_t &map() const { return board_map_; }

private:
  BoardMap_t board_map_;
  MoveCalculator move_calculator_;
  ConcreteBoardStateSummarizer hash_calculator_;
  std::map<PieceColor, std::map<zkey_t, vector<TranspositionTableEntry>>>
      transposition_tables_;
  void UpdateHashCalculator(ExecutedMove executed_move);
  void SetOccupant(BoardSpace space, GamePiece piece);
  std::map<PieceColor, vector<ExecutedMove>> move_log_;
  void AddToMoveLog(ExecutedMove move);
  void RemoveFromMoveLog(ExecutedMove move);
  bool ViolatesRepeatRule(PieceColor color);
};

#include <game_board.tpp>

#endif // _GAME_BOARD_