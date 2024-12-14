#pragma once

#include <base_move_evaluator.hpp>
#include <game_board_for_concepts.hpp>
#include <game_piece.hpp>
#include <integer_types.hpp>
#include <memory>

class EvaluatorFactoryBase {
public:
  virtual std::unique_ptr<MoveEvaluatorBase> Create(
      std::shared_ptr<gameboard::GameBoardForConcepts> game_board,
      gameboard::PieceColor evaluating_player,
      DepthType search_depth,
      const std::string &json_file
  ) = 0;
  virtual ~EvaluatorFactoryBase() = default;
};