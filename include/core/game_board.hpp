// Filename: game_board.hpp
// Author: Duane Goodner
// Created: 2022-11-14
// Last Modified: 2024-09-30

// Description:
// Definition of GameBoard class.

#ifndef _GAME_BOARD_
#define _GAME_BOARD_

#include <board_components.hpp>
#include <common.hpp>
#include <functional>
#include <move_calculator.hpp>
#include <move_evaluators.hpp>
#include <vector>

using namespace std;
using namespace board_components;

// Template for class GameBoard which implements interface
// SpaceInfoProvider, and uses a ConcreteBoardStateSummarizer

extern const BoardMapInt_t kStartingBoard;

extern const int kRepeatPeriodsToCheck[3];
extern const int kMaxAllowedRepeatPeriods;

class GameBoard : public SpaceInfoProvider<GameBoard> {
public:
  GameBoard();
  GameBoard(const BoardMapInt_t starting_board);
  vector<BoardSpace> ImplementGetAllSpacesOccupiedBy(PieceColor color);
  PieceColor ImplementGetColor(BoardSpace space);
  PieceType ImplementGetType(BoardSpace space);
  MoveCollection ImplementCalcFinalMovesOf(PieceColor color);
  bool IsInCheck(PieceColor color);
  ExecutedMove ImplementExecuteMove(Move move);
  void ImplementUndoMove(ExecutedMove executed_move);
  GamePiece GetOccupant(BoardSpace space);
  const BoardMap_t &map() const;
  void ImplementAttachMoveCallback(function<void(ExecutedMove)> callback);
  std::map<PieceColor, vector<ExecutedMove>> GetMoveLog();

private:
  BoardMap_t board_map_;
  MoveCalculator move_calculator_;
  vector<function<void(ExecutedMove)>> move_callbacks_;  // hash calc functs go here
  std::map<PieceColor, vector<ExecutedMove>> move_log_;
  
  void UpdateHashCalculator(ExecutedMove executed_move);
  void SetOccupant(BoardSpace space, GamePiece piece);
  void AddToMoveLog(ExecutedMove executed_move);
  void RemoveFromMoveLog(ExecutedMove executed_move);
  bool ViolatesRepeatRule(PieceColor color);

};

#endif // _GAME_BOARD_