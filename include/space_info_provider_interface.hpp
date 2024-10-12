//! @file space_info_provider_interface.hpp
//! Definition of SpaceInfoProvider CRTP interface.

#pragma once

#include <board_data_structs.hpp>

using namespace gameboard;

//! CRTP interface with methods for obtaining information about gameboard::BoardSpace
//! objects, calculating legal gameboard::Move objects, executing a gameboard::Move, and un-doing
//! a gameboard::ExecutedMove (concrete example = gameboard::GameBoard).
template <typename ConcreteSpaceInfoProvider>
class SpaceInfoProvider {
public:
  vector<BoardSpace> GetAllSpacesOccupiedBy(PieceColor color) {
    return static_cast<ConcreteSpaceInfoProvider *>(this)
        ->ImplementGetAllSpacesOccupiedBy(color);
  }

  PieceColor GetColor(BoardSpace space) {
    return static_cast<ConcreteSpaceInfoProvider *>(this)->ImplementGetColor(space);
  }

  PieceType GetType(BoardSpace space) {
    return static_cast<ConcreteSpaceInfoProvider *>(this)->ImplementGetType(space);
  }

  MoveCollection CalcFinalMovesOf(PieceColor color) {
    return static_cast<ConcreteSpaceInfoProvider *>(this)->ImplementCalcFinalMovesOf(
        color
    );
  };

  ExecutedMove ExecuteMove(Move move) {
    return static_cast<ConcreteSpaceInfoProvider *>(this)->ImplementExecuteMove(move);
  }

  void UndoMove(ExecutedMove executed_move) {
    static_cast<ConcreteSpaceInfoProvider *>(this)->ImplementUndoMove(executed_move);
  }

  void AttachMoveCallback(function<void(ExecutedMove)> callback) {
    static_cast<ConcreteSpaceInfoProvider *>(this)->ImplementAttachMoveCallback(callback
    );
  }
};