// Filename: game_board.hpp
// Author: Duane Goodner
// Created: 2022-11-14
// Last Modified: 2024-08-16

// Description:
// Definition of GameBoard class, and the BoardStateTracker CRTP interface that
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

// CRTP Interface: GameBoard <- HashCalculator
template <typename ConcreteHashCalculator>
class BoardStateTracker {
public:
  void CalcInitialBoardState(BoardMap_t &board_map) {
    static_cast<ConcreteHashCalculator *>(this)
        ->ImplementCalcInitialBoardState(board_map);
  }
  void CalcNewBoardState(const ExecutedMove &move) {
    return static_cast<ConcreteHashCalculator *>(this)
        ->ImplementCalcNewBoardState(move);
  }
};

// Template class for a GameBoard that has a ConcreteHashCalculator, and
// implements the following interfaces: MoveTracker (specified by MoveSelector)
// SpaceInfoProvider (specified by MinimaxEvaluator)
template <typename ConcreteHashCalculator>
class GameBoard : public MoveTracker<
                      GameBoard<ConcreteHashCalculator>,
                      ConcreteHashCalculator>,
                  public SpaceInfoProvider<GameBoard<ConcreteHashCalculator>> {

public:
  GameBoard();
  GameBoard(const BoardMapInt_t starting_board);
  // void ImplementAttachHashCalculator(
  //     ConcreteHashCalculator *hash_calculator,
  //     size_t zcolor_idx
  // ) {
  //   hash_calculator->CalcInitialBoardState(board_map_);
  //   hash_calculators_[zcolor_idx] = hash_calculator;
  //   num_hash_calculators_++;
  // }
  bool IsOccupied(BoardSpace space);
  GamePiece GetOccupant(BoardSpace space);
  ExecutedMove ImplementExecuteMove(Move move);
  void ImplementUndoMove(ExecutedMove executed_move);
  vector<BoardSpace> ImplementGetAllSpacesOccupiedBy(PieceColor color);
  MoveCollection ImplementCalcFinalMovesOf(PieceColor color);
  bool IsInCheck(PieceColor color);
  PieceColor ImplementGetColor(BoardSpace space);
  PieceType ImplementGetType(BoardSpace space);
  const BoardMap_t &map() const { return board_map_; }

private:
  BoardMap_t board_map_;
  MoveCalculator move_calculator_;
  ConcreteHashCalculator red_hash_calculator_;
  ConcreteHashCalculator black_hash_calculator_;
  ConcreteHashCalculator *hash_calculators_[2];
  size_t num_hash_calculators_;
  void UpdateHashCalculators(ExecutedMove executed_move);
  void SetOccupant(BoardSpace space, GamePiece piece);
  std::map<PieceColor, vector<ExecutedMove>> move_log_;
  void AddToMoveLog(ExecutedMove move);
  void RemoveFromMoveLog(ExecutedMove move);
  bool ViolatesRepeatRule(PieceColor color);
};

#include <game_board.tpp>

#endif // _GAME_BOARD_