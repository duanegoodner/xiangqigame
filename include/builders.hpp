#pragma once

#include <concept_board_state_calculator.hpp>
#include <game_board_for_concepts.hpp>
#include <memory>
#include <move_evaluators_for_concepts.hpp>
#include <piece_position_points_for_concepts.hpp>
#include <zobrist_for_concepts.hpp>



namespace moveselection {

// class RandomMoveEvalutorBuilder {
// public:
//   template <SpaceInfoProviderConcept G>
//   std::unique_ptr<RandomMoveEvaluatorForConcept<G>> build(
//       gameboard::PieceColor evaluating_player,
//       std::shared_ptr<G> game_board
//   ) {
//     return std::make_unique<RandomMoveEvaluatorForConcept<G>>(
//         evaluating_player,
//         game_board
//     );
//   }
// };

class MinimaxMoveEvaluatorBuilder {
public:
  template <
      typename KeyType,
      SpaceInfoProviderConcept G,
      BoardStateCoordinatorConcept<KeyType> H,
      PieceValueProviderConcept P>
  std::unique_ptr<MinimaxMoveEvaluatorForConcept<KeyType, G, H, P>> build(
      gameboard::PieceColor evaluating_player,
      DepthType search_depth,
      std::shared_ptr<G> game_board,
      std::shared_ptr<P> game_position_points,
      std::shared_ptr<H> hash_calculator
  ) {
    return std::make_unique<MinimaxMoveEvaluatorForConcept<KeyType, G, H, P>>(
        evaluating_player,
        search_depth,
        game_board,
        game_position_points,
        hash_calculator
    );
  }
};

} // namespace moveselection
