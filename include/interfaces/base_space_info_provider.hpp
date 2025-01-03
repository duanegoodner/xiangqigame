#pragma once

#include <gameboard/board_data_structs.hpp>
#include <gameboard/move_data_structs.hpp>

class SpaceInfoProviderBase {
public:
  virtual ~SpaceInfoProviderBase() = default;
  virtual const gameboard::BoardMap_t& map() const = 0;
  virtual bool IsDraw() = 0;
  virtual bool IsInCheck(gameboard::PieceColor) = 0;
  virtual gameboard::MoveCollection CalcFinalMovesOf(gameboard::PieceColor) = 0;
  virtual gameboard::ExecutedMove ExecuteMove(const Move &move) = 0;
};