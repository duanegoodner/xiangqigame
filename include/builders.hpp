#pragma once

#include <array>
#include <base_evaluator_factory.hpp>
#include <concept_board_state_calculator_registry.hpp>
#include <concept_composite_concepts.hpp>
#include <concept_single_board_state_provider.hpp>
#include <functional>
#include <game_board_for_concepts.hpp>
#include <memory>
#include <move_evaluator_minimax_for_concepts.hpp>
#include <piece_position_points_for_concepts.hpp>
#include <string>
#include <zobrist_for_concepts.hpp>

// namespace gameboard {

// } // namespace gameboard

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

namespace moveselection {

template <typename KeyType, size_t NumConfKeys>
class MinimaxMoveEvaluatorFactory : public MoveEvaluatorFactoryBase {
  using P = piecepoints::PiecePositionPointsForConcepts;
  using G = gameboard::GameBoardForConcepts;
  using Z = boardstate::ZobristCoordinatorFactory<KeyType, NumConfKeys, G>;
  Z zobrist_coordinator_factory_;
  std::shared_ptr<G> game_board_;
  DepthType search_depth_;
  const std::string &json_file_;

public:
  MinimaxMoveEvaluatorFactory(
      std::shared_ptr<G> game_board,
      DepthType search_depth,
      const std::string &json_file = piecepoints::kICGABPOPath
  )
      : zobrist_coordinator_factory_{Z{}}
      , game_board_{game_board}
      , search_depth_{search_depth}
      , json_file_{json_file} {}

  using ZobristCoordinatorType = typename boardstate::
      ZobristCoordinatorFactory<KeyType, NumConfKeys, G>::ZobristCoordinatorType;

  using MoveEvaluatorType =
      moveselection::MinimaxMoveEvaluatorForConcepts<ZobristCoordinatorType, G, P>;

  std::unique_ptr<MoveEvaluatorBase> Create(gameboard::PieceColor evaluating_player
  ) override {
    auto zobrist_coordinator =
        zobrist_coordinator_factory_.CreateRegisteredCoordinator(game_board_);

    auto game_position_points = P::Create();
    return std::make_unique<MoveEvaluatorType>(
        evaluating_player,
        search_depth_,
        game_board_,
        game_position_points,
        zobrist_coordinator
    );
  }
};

} // namespace moveselection
