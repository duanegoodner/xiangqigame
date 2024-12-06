#pragma once

#include <array>
#include <concept_board_state_calculator_registry.hpp>
#include <concept_composite_concepts.hpp>
#include <concept_single_board_state_provider.hpp>
#include <game_board_for_concepts.hpp>
#include <memory>
#include <move_evaluators_for_concepts.hpp>
#include <piece_position_points_for_concepts.hpp>
#include <string>
#include <zobrist_for_concepts.hpp>

namespace moveselection {

class MinimaxMoveEvaluatorBuilder {
public:
  template <
      size_t NumConfKeys,
      SingleBoardStateProviderAndBoardStateCalculatorConcept C,
      SpaceInfoProviderAndCalculatorRegistryConcept G>

  std::unique_ptr<MinimaxMoveEvaluatorForConcept<
      G,
      boardstate::ZobristCoordinatorForConcepts<
          boardstate::ZobristComponentForConcepts<C, NumConfKeys>>,
      piecepoints::PiecePositionPointsForConcepts>>
  Build(
      gameboard::PieceColor evaluating_player,
      DepthType search_depth,
      std::shared_ptr<G> game_board,
      std::string json_file = piecepoints::kICGABPOPath

  ) {

    auto primary_calculator = C::Create();
    std::array<std::shared_ptr<C>, NumConfKeys> confirmation_calculators;
    for (auto idx = 0; idx < NumConfKeys; ++idx) {
      confirmation_calculators[idx] = C::Create();
    }

    auto zobrist_component =
        boardstate::ZobristComponentForConcepts<C, NumConfKeys>::Create(
            primary_calculator,
            confirmation_calculators
        );

    auto game_position_points = piecepoints::PiecePositionPointsForConcepts::Create();

    auto zobrist_coordinator = boardstate::ZobristCoordinatorForConcepts<
        boardstate::ZobristComponentForConcepts<C, NumConfKeys>>::
        Create(zobrist_component);

    return std::make_unique<MinimaxMoveEvaluatorForConcept<
        G,
        boardstate::ZobristCoordinatorForConcepts<
            boardstate::ZobristComponentForConcepts<C, NumConfKeys>>,
        piecepoints::PiecePositionPointsForConcepts>>(
        evaluating_player,
        search_depth,
        game_board,
        game_position_points,
        zobrist_coordinator
    );
  }
};

} // namespace moveselection
