#pragma once

#include <array>
#include <interfaces/base_evaluator_factory.hpp>
#include <concepts/board_state_calculator_registry.hpp>
#include <concepts/composite_concepts.hpp>
#include <concepts/single_board_state_provider.hpp>
#include <functional>
#include <gameboard/game_board_for_concepts.hpp>
#include <memory>
#include <moveselection/move_evaluator_minimax_for_concepts.hpp>
#include <piecepoints/piece_position_points_for_concepts.hpp>
#include <string>
#include <boardstate/zobrist_for_concepts.hpp>


namespace boardstate {

template <typename KeyType, BoardStateCalculatorRegistryConcept G>
class ZobristCalculatorFactory {
public:
  using ZobristCalculatorType = boardstate::ZobristCalculatorForConcepts<KeyType>;
  //! Creates a new ZobristCalculator, initializes its state using pre-existing
  //! GameBoard, and registers it with the GameBoard.
  std::shared_ptr<ZobristCalculatorType> CreateRegisteredCalculator(
      std::shared_ptr<G> game_board
  ) {
    auto new_calculator = ZobristCalculatorType::Create();
    new_calculator->FullBoardStateCalc(game_board->map());
    game_board->AttachMoveCallback(std::bind(
        &ZobristCalculatorType::UpdateBoardState,
        new_calculator,
        std::placeholders::_1
    ));

    return new_calculator;
  }

  std::shared_ptr<ZobristCalculatorType> CreateUnregistereCalculator() {
    return ZobristCalculatorType::Create();
  }
};

template <typename KeyType, size_t NumConfKeys, BoardStateCalculatorRegistryConcept G>
class ZobristComponentFactory {
public:
  using ZobristCalculatorType = boardstate::ZobristCalculatorForConcepts<KeyType>;
  using ZobristComponentType =
      boardstate::ZobristComponentForConcepts<ZobristCalculatorType, NumConfKeys>;
  using ConfCalculatorsArrayType =
      std::array<std::shared_ptr<ZobristCalculatorType>, NumConfKeys>;

  std::shared_ptr<ZobristComponentType> CreateRegisteredComponent(
      std::shared_ptr<G> game_board
  ) {
    auto primary_calculator =
        zobrist_calculator_factory_.CreateRegisteredCalculator(game_board);
    std::array<std::shared_ptr<ZobristCalculatorType>, NumConfKeys>
        confirmation_calculators;
    for (auto idx = 0; idx < NumConfKeys; ++idx) {
      confirmation_calculators[idx] =
          zobrist_calculator_factory_.CreateRegisteredCalculator(game_board);
    }
    return ZobristComponentType::Create(primary_calculator, confirmation_calculators);
  }

  std::shared_ptr<ZobristComponentType> CreateUnregisteredComponent(
      std::shared_ptr<ZobristCalculatorType> primary_calculator,
      ConfCalculatorsArrayType confirmation_calculators
  ) {
    ZobristComponentType::Create(primary_calculator, confirmation_calculators);
  }

  std::shared_ptr<ZobristComponentType> CreateUnregisteredComponent() {
    auto primary_calculator = zobrist_calculator_factory_.CreateUnregistereCalculator();
    std::array<std::shared_ptr<ZobristCalculatorType>, NumConfKeys>
        confirmation_calculators;
    for (auto idx = 0; idx < NumConfKeys; ++idx) {
      confirmation_calculators[idx] =
          zobrist_calculator_factory_.CreateUnregistereCalculator();
    }
    return ZobristComponentType::Create(primary_calculator, confirmation_calculators);
  }

private:
  ZobristCalculatorFactory<KeyType, G> zobrist_calculator_factory_;
};

template <typename KeyType, size_t NumConfKeys, BoardStateCalculatorRegistryConcept G>
class ZobristCoordinatorFactory {
public:
  using ZobristCalculatorType = ZobristCalculatorForConcepts<KeyType>;
  using ZobristComponentFactoryType = ZobristComponentFactory<KeyType, NumConfKeys, G>;
  using ZobristComponentType =
      ZobristComponentForConcepts<ZobristCalculatorType, NumConfKeys>;
  using ZobristCoordinatorType = ZobristCoordinatorForConcepts<ZobristComponentType>;

  std::shared_ptr<ZobristCoordinatorType> CreateRegisteredCoordinator(
      std::shared_ptr<G> game_board
  ) {

    auto zobrist_component =
        zobrist_component_factory_.CreateRegisteredComponent(game_board);

    return ZobristCoordinatorType::Create(zobrist_component);
  }

private:
  ZobristComponentFactoryType zobrist_component_factory_;
};
} // namespace boardstate

