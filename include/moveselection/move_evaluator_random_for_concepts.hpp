#pragma once

#include <interfaces/base_evaluator_factory.hpp>
#include <interfaces/base_move_evaluator.hpp>
#include <gameboard/game_piece.hpp>
#include <memory>
#include <gameboard/move_data_structs.hpp>
#include <random>
#include <utilities/utility_functs.hpp>

//! @todo Consider giving RandomMoveEvaluatorForConcepts its own random generator that
//! gets used during each call to SelectMove.

namespace moveselection {

//! Complies with MoveEvaluatorConcept. Randomly chooses one of legal moves
//! available to moveselection::RandomMoveEvaluator.evaluating_player_.
class RandomMoveEvaluatorForConcepts : public MoveEvaluatorBase {
  gameboard::PieceColor evaluating_player_;
  std::mt19937_64 prng_;

public:
  //! Instantiates RandomMoveEvaluatorForConcepts in heap.
  static std::unique_ptr<RandomMoveEvaluatorForConcepts> Create(
      gameboard::PieceColor evaluating_player
  );

  //! Randomly selects move from collection of allowed moves.
  gameboard::Move SelectMove(const gameboard::MoveCollection &allowed_moves);

  void NotifyIllegalMove();

private:
  //! Constructor is private to force use of static Create method.
  RandomMoveEvaluatorForConcepts(gameboard::PieceColor evaluating_player);
};

class RandomMoveEvaluatorFactory : public MoveEvaluatorFactoryBase{
public:
  std::unique_ptr<MoveEvaluatorBase> Create(
      gameboard::PieceColor evaluating_player
  ) override;
};

} // namespace moveselection