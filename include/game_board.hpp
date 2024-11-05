//! @file game_board.hpp
//! Definition of gameboard::GameBoard class

#pragma once

#include <board_data_structs.hpp>
#include <functional>
#include <move_calculator.hpp>
#include <move_evaluators.hpp>
#include <vector>

using namespace std;
using namespace gameboard;

namespace gameboard {

extern const BoardMapInt_t kStartingBoard;
extern const int kRepeatPeriodsToCheck[3];
extern const int kRepeatPeriodsMaxAllowed;
extern const int kMaxMovesWithoutCapture;

//! Implements SpaceInfoProvider interface; stores piece positions, and exposes methods
//! for calculating, executing, an un-doing moves..
class GameBoard : public SpaceInfoProvider<GameBoard> {
public:
  GameBoard();
  GameBoard(const BoardMapInt_t starting_board);
  vector<BoardSpace> ImplementGetAllSpacesOccupiedBy(PieceColor color) const;
  PieceColor ImplementGetColor(const BoardSpace &space) const;
  PieceType ImplementGetType(const BoardSpace &space) const;
  MoveCollection ImplementCalcFinalMovesOf(PieceColor color);
  bool IsInCheck(PieceColor color);
  ExecutedMove ImplementExecuteMove(const Move &move);
  bool IsCaptureMove(const ExecutedMove &executed_move) const;
  void ImplementUndoMove(const ExecutedMove &executed_move);
  GamePiece GetOccupantAt(const BoardSpace &space) const;
  const BoardMap_t &map() const;
  void ImplementAttachMoveCallback(const function<void(const ExecutedMove&)>& callback);
  bool ImplementIsDraw();
  const std::map<PieceColor, vector<ExecutedMove>>& move_log() const;

private:
  //! 2-D array of GamePiece objects.
  BoardMap_t board_map_;

  //! Encapsulates all calculations of allowed moves.
  MoveCalculator move_calculator_;

  //! Stores functions that are called after any change in board config to keep
  //! boardstate::SingleZobristTracker objects updated.
  vector<function<void(const ExecutedMove&)>> move_callbacks_;

  //! Vectors of all moves that have been executed (and not un-done) by each player.
  std::map<PieceColor, vector<ExecutedMove>> move_log_;

  //! Number of moves executed since last time a piece was captured. 
  int moves_since_last_capture_; 

  void UpdateStateTracker(const ExecutedMove &executed_move);
  void SetOccupantAt(const BoardSpace &space, GamePiece piece);
  void AddToMoveLog(const ExecutedMove &executed_move);
  void RemoveFromMoveLog(const ExecutedMove &executed_move);
  bool ViolatesRepeatRule(PieceColor color);
};

} // namespace gameboard