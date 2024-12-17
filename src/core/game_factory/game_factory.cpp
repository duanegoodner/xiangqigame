#include <game_factory.hpp>

namespace game {

EvaluatorFactoryRetriever::EvaluatorFactoryRetriever(
    const PlayerSpec &evaluator_factory_info,
    std::shared_ptr<gameboard::GameBoardForConcepts> game_board
)
    : evaluator_factory_info_{evaluator_factory_info}
    , game_board_{game_board}
    , minimax_factories_{} {
  minimax_factories_.emplace(
      MinimaxTypeInfo{ZobristKeyType::k032, ZobristCalculatorCount::kOne},
      std::make_shared<moveselection::MinimaxMoveEvaluatorFactory<uint32_t, 0>>(
          game_board_,
          evaluator_factory_info.minimax_search_depth
      )
  );
  minimax_factories_.emplace(
      MinimaxTypeInfo{ZobristKeyType::k064, ZobristCalculatorCount::kOne},
      std::make_shared<moveselection::MinimaxMoveEvaluatorFactory<uint32_t, 0>>(
          game_board_,
          evaluator_factory_info.minimax_search_depth
      )
  );
  minimax_factories_.emplace(
      MinimaxTypeInfo{ZobristKeyType::k128, ZobristCalculatorCount::kOne},
      std::make_shared<moveselection::MinimaxMoveEvaluatorFactory<__uint128_t, 0>>(
          game_board_,
          evaluator_factory_info.minimax_search_depth
      )
  );
  minimax_factories_.emplace(
      MinimaxTypeInfo{ZobristKeyType::k032, ZobristCalculatorCount::kTwo},
      std::make_shared<moveselection::MinimaxMoveEvaluatorFactory<uint32_t, 1>>(
          game_board_,
          evaluator_factory_info.minimax_search_depth
      )
  );
  minimax_factories_.emplace(
      MinimaxTypeInfo{ZobristKeyType::k064, ZobristCalculatorCount::kTwo},
      std::make_shared<moveselection::MinimaxMoveEvaluatorFactory<uint32_t, 1>>(
          game_board_,
          evaluator_factory_info.minimax_search_depth
      )
  );
  minimax_factories_.emplace(
      MinimaxTypeInfo{ZobristKeyType::k128, ZobristCalculatorCount::kOne},
      std::make_shared<moveselection::MinimaxMoveEvaluatorFactory<__uint128_t, 0>>(
          game_board_,
          evaluator_factory_info.minimax_search_depth
      )
  );
}

std::shared_ptr<MoveEvaluatorFactoryBase> EvaluatorFactoryRetriever::GetFactory() {
  std::shared_ptr<MoveEvaluatorFactoryBase> factory;

  if (evaluator_factory_info_.evaluator_type == EvaluatorType::kRandom) {
    factory = std::make_shared<moveselection::RandomMoveEvaluatorFactory>();
  }
  if (evaluator_factory_info_.evaluator_type == EvaluatorType::kHuman) {
    factory = std::make_shared<moveselection::HumanMoveEvaluatorFactory>(
        evaluator_factory_info_.input_stream
    );
  }
  if (evaluator_factory_info_.evaluator_type == EvaluatorType::kMinimax) {
    factory = minimax_factories_.at(evaluator_factory_info_.minimax_type_info);
  }
  return factory;
}

} // namespace game