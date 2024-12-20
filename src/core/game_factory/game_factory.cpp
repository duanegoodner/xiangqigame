#include <game_factory.hpp>

namespace game {

EvaluatorFactoryRetriever::EvaluatorFactoryRetriever(
    const PlayerSpec &player_spec,
    std::shared_ptr<gameboard::GameBoardForConcepts> game_board,
    std::istream &input_stream
)
    : player_spec_{player_spec}
    , game_board_{game_board}
    , input_stream_{input_stream}
    , minimax_factories_{} {
  minimax_factories_.emplace(
      MinimaxTypeInfo{ZobristKeyType::k032, ZobristCalculatorCount::kOne},
      std::make_shared<moveselection::MinimaxMoveEvaluatorFactory<uint32_t, 0>>(
          game_board_,
          player_spec.minimax_search_depth
      )
  );
  minimax_factories_.emplace(
      MinimaxTypeInfo{ZobristKeyType::k064, ZobristCalculatorCount::kOne},
      std::make_shared<moveselection::MinimaxMoveEvaluatorFactory<uint32_t, 0>>(
          game_board_,
          player_spec.minimax_search_depth
      )
  );
  minimax_factories_.emplace(
      MinimaxTypeInfo{ZobristKeyType::k128, ZobristCalculatorCount::kOne},
      std::make_shared<moveselection::MinimaxMoveEvaluatorFactory<__uint128_t, 0>>(
          game_board_,
          player_spec.minimax_search_depth
      )
  );
  minimax_factories_.emplace(
      MinimaxTypeInfo{ZobristKeyType::k032, ZobristCalculatorCount::kTwo},
      std::make_shared<moveselection::MinimaxMoveEvaluatorFactory<uint32_t, 1>>(
          game_board_,
          player_spec.minimax_search_depth
      )
  );
  minimax_factories_.emplace(
      MinimaxTypeInfo{ZobristKeyType::k064, ZobristCalculatorCount::kTwo},
      std::make_shared<moveselection::MinimaxMoveEvaluatorFactory<uint32_t, 1>>(
          game_board_,
          player_spec.minimax_search_depth
      )
  );
  minimax_factories_.emplace(
      MinimaxTypeInfo{ZobristKeyType::k128, ZobristCalculatorCount::kOne},
      std::make_shared<moveselection::MinimaxMoveEvaluatorFactory<__uint128_t, 0>>(
          game_board_,
          player_spec.minimax_search_depth
      )
  );
}

std::shared_ptr<MoveEvaluatorFactoryBase> EvaluatorFactoryRetriever::GetFactory() {
  std::shared_ptr<MoveEvaluatorFactoryBase> factory;

  if (player_spec_.evaluator_type == EvaluatorType::kRandom) {
    factory = std::make_shared<moveselection::RandomMoveEvaluatorFactory>();
  }
  if (player_spec_.evaluator_type == EvaluatorType::kHuman) {
    factory = std::make_shared<moveselection::HumanMoveEvaluatorFactory>(
        input_stream_
    );
  }
  if (player_spec_.evaluator_type == EvaluatorType::kMinimax) {
    factory = minimax_factories_.at(player_spec_.minimax_type_info);
  }
  return factory;
}

} // namespace game