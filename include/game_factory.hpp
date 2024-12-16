#pragma once

#include <base_evaluator_factory.hpp>
#include <base_move_evaluator.hpp>
#include <base_space_info_provider.hpp>
#include <builders.hpp>
#include <concept_composite_concepts.hpp>
#include <concept_move_evaluator.hpp>
#include <game.hpp>
#include <game_board_for_concepts.hpp>
#include <memory>

class GameFactory {
  std::shared_ptr<MoveEvaluatorFactoryBase> red_evaluator_factory_;
  std::shared_ptr<MoveEvaluatorFactoryBase> black_evaluator_factory_;
  std::shared_ptr<gameboard::GameBoardFactory> game_board_factory_;

public:
  GameFactory(
      std::shared_ptr<MoveEvaluatorFactoryBase> red_evaluator_factory,
      std::shared_ptr<MoveEvaluatorFactoryBase> black_evaluator_factory,
      std::shared_ptr<gameboard::GameBoardFactory> game_board_factory
  )
      : red_evaluator_factory_{red_evaluator_factory}
      , black_evaluator_factory_{black_evaluator_factory}
      , game_board_factory_{game_board_factory} {}





};