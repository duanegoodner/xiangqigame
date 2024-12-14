#pragma once

#include <base_move_evaluator.hpp>
#include <game_board_for_concepts.hpp>
#include <game_piece.hpp>
#include <integer_types.hpp>
#include <memory>

class EvaluatorFactoryBase {
public:
  virtual std::unique_ptr<MoveEvaluatorBase> Create(
      gameboard::PieceColor evaluating_player
  ) = 0;
  virtual ~EvaluatorFactoryBase() = default;
};