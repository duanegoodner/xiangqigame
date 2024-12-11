
#include <iostream>
#include <memory>
#include <move_evaluator_human_for_concepts.hpp>

namespace moveselection {

// HumanMoveEvaluator methods

HumanMoveEvaluatorForConcepts::HumanMoveEvaluatorForConcepts(
    gameboard::PieceColor evaluating_player
)
    : evaluating_player_{evaluating_player} {}

std::unique_ptr<HumanMoveEvaluatorForConcepts> HumanMoveEvaluatorForConcepts::Create(
    gameboard::PieceColor evaluating_player
) {
  return std::unique_ptr<HumanMoveEvaluatorForConcepts>(
      new HumanMoveEvaluatorForConcepts(evaluating_player)
  );
}

gameboard::Move HumanMoveEvaluatorForConcepts::SelectMove(
    gameboard::MoveCollection &allowed_moves
) {
  return Move{{0, 0}, {0, 0}};
}





// Factory methods

std::unique_ptr<HumanMoveEvaluatorForConcepts> HumanMoveEvaluatorFactory::Create(
    gameboard::PieceColor evaluating_player
) {
  return HumanMoveEvaluatorForConcepts::Create(evaluating_player);
}

} // namespace moveselection