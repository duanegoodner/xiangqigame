//! @file interfaces/space_info_provider_interface.hpp
//! Definition of SpaceInfoProvider CRTP interface.

#pragma once

#include <gameboard/board_data_structs.hpp>
#include <concepts>

using namespace gameboard;

//! CRTP interface with methods for obtaining information about gameboard::BoardSpace
//! objects, calculating legal gameboard::Move objects, executing a gameboard::Move, and
//! un-doing a gameboard::ExecutedMove (concrete example = gameboard::GameBoard).
template <typename ConcreteSpaceInfoProvider>
class SpaceInfoProvider {
public:
  vector<BoardSpace> GetAllSpacesOccupiedBy(PieceColor color) {
    return static_cast<ConcreteSpaceInfoProvider *>(this)
        ->ImplementGetAllSpacesOccupiedBy(color);
  }

  PieceColor GetColor(const BoardSpace &space) {
    return static_cast<ConcreteSpaceInfoProvider *>(this)->ImplementGetColor(space);
  }

  PieceType GetType(const BoardSpace &space) {
    return static_cast<ConcreteSpaceInfoProvider *>(this)->ImplementGetType(space);
  }

  MoveCollection CalcFinalMovesOf(PieceColor color) {
    return static_cast<ConcreteSpaceInfoProvider *>(this)->ImplementCalcFinalMovesOf(
        color
    );
  };

  ExecutedMove ExecuteMove(const Move &move) {
    return static_cast<ConcreteSpaceInfoProvider *>(this)->ImplementExecuteMove(move);
  }

  void UndoMove(const ExecutedMove &executed_move) {
    static_cast<ConcreteSpaceInfoProvider *>(this)->ImplementUndoMove(executed_move);
  }

  void AttachMoveCallback(function<void(const ExecutedMove &)> callback) {
    static_cast<ConcreteSpaceInfoProvider *>(this)->ImplementAttachMoveCallback(callback
    );
  }

  bool IsDraw() {
    return static_cast<ConcreteSpaceInfoProvider *>(this)->ImplementIsDraw();
  }
};

