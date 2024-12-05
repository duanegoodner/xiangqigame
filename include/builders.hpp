#pragma once

#include <concept_board_state_calculator_registry.hpp>
#include <concept_single_board_state_provider.hpp>
#include <game_board_for_concepts.hpp>
#include <memory>
#include <move_evaluators_for_concepts.hpp>
#include <piece_position_points_for_concepts.hpp>
#include <zobrist_for_concepts.hpp>

namespace moveselection {

class MinimaxMoveEvaluatorBuilder {
public:
  template <
      BoardStateCalculatorConcept C,
      typename KeyType,
      SpaceInfoProviderConcept G,
      BoardStateCoordinatorConcept H,
      PieceValueProviderConcept P>
  std::unique_ptr<MinimaxMoveEvaluatorForConcept<KeyType, G, H, P>> build(
      gameboard::PieceColor evaluating_player,
      DepthType search_depth,
      std::shared_ptr<G> game_board,
      size_t num_conf_calcs,
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
