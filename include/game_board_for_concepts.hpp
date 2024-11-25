#pragma once

#include <board_data_structs.hpp>
#include <board_state_tracker_concept.hpp>
#include <functional>
#include <map>
#include <memory>
#include <move_calculator.hpp>
#include <move_data_structs.hpp>

namespace gameboard {

extern const BoardMapInt_t kStandardInitialBoard;
extern const int kRepeatPeriodsToCheck[3];
extern const int kRepeatPeriodsMaxAllowed;
extern const int kMaxMovesWithoutCapture;

//! Must comply with SpaceInfoProviderConcept; stores piece positions, and exposes
//! methods for calculating, executing, an un-doing moves..
class GameBoardForConcepts {
public:
  // GameBoardForConcepts();
  GameBoardForConcepts(const BoardMapInt_t starting_board = kStandardInitialBoard);
  std::vector<BoardSpace> GetAllSpacesOccupiedBy(PieceColor color) const;
  PieceColor GetColor(const BoardSpace &space) const;
  PieceType GetType(const BoardSpace &space) const;
  MoveCollection CalcFinalMovesOf(PieceColor color);
  bool IsInCheck(PieceColor color);
  ExecutedMove ExecuteMove(const Move &move);
  bool IsCaptureMove(const ExecutedMove &executed_move) const;
  void UndoMove(const ExecutedMove &executed_move);
  GamePiece GetOccupantAt(const BoardSpace &space) const;
  const BoardMap_t &map() const;
  // void AttachMoveCallback(const std::function<void(const ExecutedMove &)> &callback);
  void AttachMoveCallback(void (*callback)(const ExecutedMove &));
  bool IsDraw();
  const std::map<PieceColor, std::vector<ExecutedMove>> &move_log() const;

private:
  //! 2-D array of GamePiece objects.
  BoardMap_t board_map_;

  //! Encapsulates all calculations of allowed moves.
  MoveCalculator move_calculator_;

  //! Stores functions that are called after any change in board config to keep
  //! boardstate::SingleZobristCoordinator objects updated.
  // std::vector<function<void(const ExecutedMove &)>> move_callbacks_;
  std::vector<void (*)(const ExecutedMove&)> move_callbacks_;

  

  //! Vectors of all moves that have been executed (and not un-done) by each player.
  std::map<PieceColor, std::vector<ExecutedMove>> move_log_;

  //! Number of moves executed since last time a piece was captured.
  MoveCountType moves_since_last_capture_;

  void UpdateStateTracker(const ExecutedMove &executed_move);
  void SetOccupantAt(const BoardSpace &space, GamePiece piece);
  void AddToMoveLog(const ExecutedMove &executed_move);
  void RemoveFromMoveLog(const ExecutedMove &executed_move);
  bool ViolatesRepeatRule(PieceColor color);
};

} // namespace gameboard