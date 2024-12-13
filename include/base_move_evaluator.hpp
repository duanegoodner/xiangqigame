#pragma once

#include <move_data_structs.hpp>

class MoveEvaluatorBase {
public:
  virtual gameboard::Move SelectMove(gameboard::MoveCollection &allowed_moves) = 0;
  virtual void NotifyIllegalMove() = 0;
  virtual ~MoveEvaluatorBase() = default;
};