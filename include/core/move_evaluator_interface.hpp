//! @file move_evaluator_interface.hpp
//! Definition of MoveEvaluator interface. 

#pragma once

#include <board_data_structs.hpp>

// CRTP Interface: AIPlayer <- MoveEvaluator
// Currently not using since AI Player is currently in Python side of app.
// If/when implement AI Player in C++, will move this interface definition to
// C++ Player header file.
template <typename ConcreteMoveEvaluator>
class MoveEvaluator {
public:
  moves::Move SelectMove() {
    return static_cast<ConcreteMoveEvaluator *>(this)->ImplementSelectMove();
  }
};