#pragma once

#include <board_data_structs.hpp>
#include <move_data_structs.hpp>

class SpaceInfoProviderBase {
public:
  virtual ~SpaceInfoProviderBase() = default;
  virtual gameboard::BoardMap_t map() = 0;
  virtual bool IsDraw() = 0;
  virtual gameboard::MoveCollection CalcFinalMovesOf(gameboard::PieceColor) = 0;
  virtual gameboard::ExecutedMove ExecuteMove(const Move &move) = 0;
};