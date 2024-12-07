#pragma once

#include <array>
#include <concept_board_state_calculator_registry.hpp>
#include <concept_composite_concepts.hpp>
#include <concept_single_board_state_provider.hpp>
#include <functional>
#include <game_board_for_concepts.hpp>
#include <memory>
#include <move_evaluators_for_concepts.hpp>
#include <piece_position_points_for_concepts.hpp>
#include <string>
#include <zobrist_for_concepts.hpp>

namespace gameboard {
// template <BoardStateCalculatorConcept RC, BoardStateCalculatorConcept BC>
class GameBoardFactory {
public:
  // using GameBoardType = GameBoardForConcepts;
  std::shared_ptr<GameBoardForConcepts> Create(
      const BoardMapInt_t &starting_board = kStandardInitialBoard
  ) {
    return GameBoardForConcepts::Create(starting_board);
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
    // game_board->AttachCalculator(new_calculator, color);
    game_board->AttachMoveCallback(std::bind(
        &ZobristCalculatorType::UpdateBoardState,
        new_calculator,
        std::placeholders::_1
    ));

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

template <SpaceInfoProviderConcept G>
class RandomMoveEvaluatorFactory {
public:
  using MoveEvaluatorType = moveselection::RandomMoveEvaluatorForConcept<G>;

  std::unique_ptr<MoveEvaluatorType> Create(
      std::shared_ptr<G> game_board,
      gameboard::PieceColor evaluating_player
  ) {
    return MoveEvaluatorType::Create(game_board, evaluating_player);
  }
};

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

  using MoveEvaluatorType = moveselection::MinimaxMoveEvaluatorForConcept<
      ZobristCoordinatorType,
      G,
      piecepoints::PiecePositionPointsForConcepts>;

  std::unique_ptr<MoveEvaluatorType> Create(
      std::shared_ptr<G> game_board,
      gameboard::PieceColor evaluating_player,
      DepthType search_depth,
      std::string json_file = piecepoints::kICGABPOPath
  ) {
    auto zobrist_coordinator =
        zobrist_coordinator_factory_.Create(game_board, evaluating_player);

    auto game_position_points = piecepoints::PiecePositionPointsForConcepts::Create();
    return std::make_unique<MoveEvaluatorType>(
        evaluating_player,
        search_depth,
        game_board,
        game_position_points,
        zobrist_coordinator
    );
  }
};

} // namespace moveselection
