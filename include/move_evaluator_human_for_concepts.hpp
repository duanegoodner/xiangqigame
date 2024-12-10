#pragma once

#include <game_piece.hpp>
#include <memory>
#include <move_data_structs.hpp>

namespace moveselection {
class HumanMoveEvaluatorForConcepts {
  gameboard::PieceColor evaluating_player_;

public:
  static std::unique_ptr<HumanMoveEvaluatorForConcepts> Create(
      gameboard::PieceColor evaluating_player
  );

  gameboard::Move SelectMove(gameboard::MoveCollection &allowed_moves);

private:
  HumanMoveEvaluatorForConcepts(gameboard::PieceColor evaluating_player);
};
} // namespace moveselection
