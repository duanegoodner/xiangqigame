#pragma once

#include <base_move_evaluator.hpp>
#include <base_space_info_provider.hpp>
#include <concept_board_state_coordinator.hpp>
#include <concept_composite_concepts.hpp>
#include <concept_move_evaluator.hpp>
#include <memory>

namespace game {

// template <
//     SpaceInfoProviderAndCalculatorRegistryConcept G,
//     MoveEvaluatorConcept RedEvaluator,
//     MoveEvaluatorConcept BlackEvaluator>
class Game {
  std::shared_ptr<SpaceInfoProviderBase> game_board_;
  std::shared_ptr<MoveEvaluatorBase> red_evaluator_;
  std::shared_ptr<MoveEvaluatorBase> black_evaluator_;

public:
  Game(
      std::shared_ptr<SpaceInfoProviderBase> game_board,
      std::shared_ptr<MoveEvaluatorBase> red_evaluator,
      std::shared_ptr<MoveEvaluatorBase> black_evaluator
  )
      : game_board_{game_board}
      , red_evaluator_{red_evaluator}
      , black_evaluator_{black_evaluator} {}
};

} // namespace game