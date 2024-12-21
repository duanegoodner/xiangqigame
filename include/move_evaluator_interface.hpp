//! @file move_evaluator_interface.hpp
//! Definition of MoveEvaluator interface.

#pragma once

#include <gameboard/move_data_structs.hpp>


//! CRTP interface with a method for selecting a gameboard::Move (concrete examples =
//! moveselection::MinimaxMoveEvaluator, moveselection::RandomMoveEvaluator).
template <typename ConcreteMoveEvaluator>
class MoveEvaluator {
public:
  gameboard::Move SelectMove(MoveCollection &allowed_moves) {
    return static_cast<ConcreteMoveEvaluator *>(this)->ImplementSelectMove(allowed_moves);
  }
};