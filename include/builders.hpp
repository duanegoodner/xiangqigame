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

namespace gameboard {
template <typename KeyTypeRed, typename KeyTypeBlack>
class GameBoardFactory {
public:
  using GameBoardType = GameBoardForConcepts<
      boardstate::ZobristCalculatorForConcepts<KeyTypeRed>,
      boardstate::ZobristCalculatorForConcepts<KeyTypeBlack>>;
  std::shared_ptr<GameBoardType> Create(
      const BoardMapInt_t &starting_board = kStandardInitialBoard
  ) {
    return GameBoardType::Create(starting_board);
  }
};
} // namespace gameboard

namespace boardstate {

template <typename KeyType, BoardStateCalculatorRegistryConcept G>
class ZobristCalculatorFactory {
public:
  using ZobristCalculatorType = boardstate::ZobristCalculatorForConcepts<KeyType>;
  //! Creates a new ZobristCalculator, initializes its state using pre-existing
  //! GameBoard, and registers it with the GameBoard.
  std::shared_ptr<ZobristCalculatorType> Create(
      std::shared_ptr<G> game_board,
      gameboard::PieceColor color
  ) {
    auto new_calculator = ZobristCalculatorType::Create();
    new_calculator->FullBoardStateCalc(game_board->map());
    game_board->AttachCalculator(new_calculator, color);

    return new_calculator;
  }
};

template <
    typename KeyType,
    size_t NumConfKeys,
    SpaceInfoProviderAndCalculatorRegistryConcept G>
class ZobristCoordinatorFactory {
public:
  using ZobristCalculatorType = boardstate::ZobristCalculatorForConcepts<KeyType>;
  using ZobristComponentType =
      boardstate::ZobristComponentForConcepts<ZobristCalculatorType, NumConfKeys>;
  using ZobristCoordinatorType =
      boardstate::ZobristCoordinatorForConcepts<ZobristComponentType>;

  std::shared_ptr<ZobristCoordinatorType> Create(
      std::shared_ptr<G> game_board,
      gameboard::PieceColor color
  ) {

    ZobristCalculatorFactory<KeyType, G> zobrist_calculator_factory;

    auto primary_calculator = zobrist_calculator_factory.Create(game_board, color);

    std::array<std::shared_ptr<ZobristCalculatorType>, NumConfKeys>
        confirmation_calculators;
    for (auto idx = 0; idx < NumConfKeys; ++idx) {
      confirmation_calculators[idx] =
          zobrist_calculator_factory.Create(game_board, color);
    }

    auto zobrist_component =
        ZobristComponentType::Create(primary_calculator, confirmation_calculators);

    return ZobristCoordinatorType::Create(zobrist_component);
  }
};
} // namespace boardstate

namespace moveselection {

template <
    typename KeyType,
    size_t NumConfKeys,
    SpaceInfoProviderAndCalculatorRegistryConcept G>
class MinimaxMoveEvaluatorFactory {
  boardstate::ZobristCoordinatorFactory<KeyType, NumConfKeys, G>
      zobrist_coordinator_factory_;

public:
  using ZobristCoordinatorType = boardstate::
      ZobristCoordinatorFactory<KeyType, NumConfKeys, G>::ZobristCoordinatorType;

  using MinimaxMoveEvaluatorType = moveselection::MinimaxMoveEvaluatorForConcept<
      ZobristCoordinatorType,
      G,
      piecepoints::PiecePositionPointsForConcepts>;

  std::unique_ptr<MinimaxMoveEvaluatorType> Create(
      std::shared_ptr<G> game_board,
      gameboard::PieceColor evaluating_player,
      DepthType search_depth,
      std::string json_file = piecepoints::kICGABPOPath
  ) {
    auto zobrist_coordinator =
        zobrist_coordinator_factory_.Create(game_board, evaluating_player);

    auto game_position_points = piecepoints::PiecePositionPointsForConcepts::Create();
    return std::make_unique<MinimaxMoveEvaluatorType>(
        evaluating_player,
        search_depth,
        game_board,
        game_position_points,
        zobrist_coordinator
    );
  }

  // std::unique_ptr<MinimaxMoveEvaluatorForConcept<
  //     G,
  //     boardstate::ZobristCoordinatorForConcepts<
  //         boardstate::ZobristComponentForConcepts<C, NumConfKeys>>,
  //     piecepoints::PiecePositionPointsForConcepts>>
  // Build(
  //     gameboard::PieceColor evaluating_player,
  //     DepthType search_depth,
  //     std::shared_ptr<G> game_board,
  //     std::string json_file = piecepoints::kICGABPOPath

  // ) {

  //   auto primary_calculator = C::Create();
  //   std::array<std::shared_ptr<C>, NumConfKeys> confirmation_calculators;
  //   for (auto idx = 0; idx < NumConfKeys; ++idx) {
  //     confirmation_calculators[idx] = C::Create();
  //   }

  //   auto zobrist_component =
  //       boardstate::ZobristComponentForConcepts<C, NumConfKeys>::Create(
  //           primary_calculator,
  //           confirmation_calculators
  //       );

  //   auto game_position_points = piecepoints::PiecePositionPointsForConcepts::Create();

  //   auto zobrist_coordinator = boardstate::ZobristCoordinatorForConcepts<
  //       boardstate::ZobristComponentForConcepts<C, NumConfKeys>>::
  //       Create(zobrist_component);

  //   return std::make_unique<MinimaxMoveEvaluatorForConcept<
  //       G,
  //       boardstate::ZobristCoordinatorForConcepts<
  //           boardstate::ZobristComponentForConcepts<C, NumConfKeys>>,
  //       piecepoints::PiecePositionPointsForConcepts>>(
  //       evaluating_player,
  //       search_depth,
  //       game_board,
  //       game_position_points,
  //       zobrist_coordinator
  //   );
  // }
};

} // namespace moveselection
