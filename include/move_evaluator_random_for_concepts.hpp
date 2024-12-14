#pragma once

#include <base_evaluator_factory.hpp>
#include <base_move_evaluator.hpp>
#include <game_piece.hpp>
#include <memory>
#include <move_data_structs.hpp>
#include <random>
#include <utility_functs.hpp>

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
  gameboard::Move SelectMove(gameboard::MoveCollection &allowed_moves);

  void NotifyIllegalMove();

private:
  //! Constructor is private to force use of static Create method.
  RandomMoveEvaluatorForConcepts(gameboard::PieceColor evaluating_player);
};

class RandomMoveEvaluatorFactory : public EvaluatorFactoryBase{
public:
  std::unique_ptr<MoveEvaluatorBase> Create(
      gameboard::PieceColor evaluating_player
  ) override;
};

} // namespace moveselection