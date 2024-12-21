#include <integer_types.hpp>
#include <moveselection/move_evaluator_random_for_concepts.hpp>

namespace moveselection {

// RandomMoveEvaluatorForConcepts methods

RandomMoveEvaluatorForConcepts::RandomMoveEvaluatorForConcepts(
    gameboard::PieceColor evaluating_player
)
    : evaluating_player_{evaluating_player}
    , prng_{std::mt19937_64{std::random_device{}()}} {}

std::unique_ptr<RandomMoveEvaluatorForConcepts> RandomMoveEvaluatorForConcepts::Create(
    gameboard::PieceColor evaluating_player
) {
  return std::unique_ptr<RandomMoveEvaluatorForConcepts>(
      new RandomMoveEvaluatorForConcepts(evaluating_player)
  );
}

gameboard::Move RandomMoveEvaluatorForConcepts::SelectMove(
    const gameboard::MoveCollection &allowed_moves
) {
  std::uniform_int_distribution<size_t> distr(0, allowed_moves.Size() - 1);
  auto selected_move_index = distr(prng_);
  return allowed_moves.moves[selected_move_index];
}

void RandomMoveEvaluatorForConcepts::NotifyIllegalMove() {
  throw std::runtime_error("RandomMoveEvaluator selected an illegal move");
}

// RandomMoveEvaluator factory methods

std::unique_ptr<MoveEvaluatorBase> RandomMoveEvaluatorFactory::Create(
    gameboard::PieceColor evaluating_player
) {
  return RandomMoveEvaluatorForConcepts::Create(evaluating_player);
}

} // namespace moveselection