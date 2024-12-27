#pragma once

#include <array>
#include <boardstate/zobrist_for_concepts.hpp>
#include <concepts/board_state_calculator_registry.hpp>
#include <concepts/composite_concepts.hpp>
#include <concepts/single_board_state_provider.hpp>
#include <functional>
#include <gameboard/game_board_for_concepts.hpp>
#include <interfaces/base_evaluator_factory.hpp>
#include <memory>
#include <moveselection/move_evaluator_minimax_for_concepts.hpp>
#include <piecepoints/piece_position_points_for_concepts.hpp>
#include <random>
#include <string>

namespace boardstate {

template <typename KeyType, BoardStateCalculatorRegistryConcept G>
class ZobristCalculatorFactory {
public:
  using ZobristCalculatorType = boardstate::ZobristCalculatorForConcepts<KeyType>;
  //! Creates a new ZobristCalculator, initializes its state using pre-existing
  //! GameBoard, and registers it with the GameBoard.
  std::shared_ptr<ZobristCalculatorType> CreateRegisteredCalculator(
      std::shared_ptr<G> game_board,
      uint32_t prng_seed = std::random_device{}()
  ) {
    auto new_calculator = ZobristCalculatorType::Create(prng_seed);
    new_calculator->FullBoardStateCalc(game_board->map());
    game_board->AttachMoveCallback(std::bind(
        &ZobristCalculatorType::UpdateBoardState,
        new_calculator,
        std::placeholders::_1
    ));

    return new_calculator;
  }

  std::shared_ptr<ZobristCalculatorType> CreateUnregistereCalculator(
      uint32_t prng_seed = std::random_device{}()
  ) {
    return ZobristCalculatorType::Create(prng_seed);
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
      std::shared_ptr<G> game_board,
      uint32_t prng_seed = std::random_device{}()
  ) {
    std::mt19937 prng{prng_seed};

    auto primary_calculator =
        zobrist_calculator_factory_.CreateRegisteredCalculator(game_board, prng());
    std::array<std::shared_ptr<ZobristCalculatorType>, NumConfKeys>
        confirmation_calculators;
    for (auto idx = 0; idx < NumConfKeys; ++idx) {
      confirmation_calculators[idx] =
          zobrist_calculator_factory_.CreateRegisteredCalculator(game_board, prng());
    }
    return ZobristComponentType::Create(
        primary_calculator,
        confirmation_calculators,
        prng_seed
    );
  }

  std::shared_ptr<ZobristComponentType> CreateUnregisteredComponent(
      std::shared_ptr<ZobristCalculatorType> primary_calculator,
      ConfCalculatorsArrayType confirmation_calculators,
      uint32_t prng_seed = 0
  ) {
    ZobristComponentType::Create(primary_calculator, confirmation_calculators, prng_seed);
  }

  std::shared_ptr<ZobristComponentType> CreateUnregisteredComponent(
      uint32_t prng_seed = std::random_device{}()
  ) {
    std::mt19937 prng{prng_seed};

    auto primary_calculator = zobrist_calculator_factory_.CreateUnregistereCalculator(prng());
    std::array<std::shared_ptr<ZobristCalculatorType>, NumConfKeys>
        confirmation_calculators;
    for (auto idx = 0; idx < NumConfKeys; ++idx) {
      confirmation_calculators[idx] =
          zobrist_calculator_factory_.CreateUnregistereCalculator(prng());
    }
    return ZobristComponentType::Create(primary_calculator, confirmation_calculators, prng_seed);
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
