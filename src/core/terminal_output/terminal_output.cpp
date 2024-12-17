#include <terminal_output.hpp>

namespace terminalout {

PlayerInfoReporter::PlayerInfoReporter(game::PlayerSpec player_spec)
    : player_spec_{player_spec} {}

std::string PlayerInfoReporter::PlayerTypeStr() {
  return "Player type = " + evaluator_to_player_type.at(player_spec_.evaluator_type);
}

std::string PlayerInfoReporter::EvaluatorTypeStr() {
  return "Move Evaluator = " + evaluator_names.at(player_spec_.evaluator_type);
}

std::string PlayerInfoReporter::SearchDepthStr() {
  return "Max Search Depth = " + std::to_string(player_spec_.minimax_search_depth);
}

std::string PlayerInfoReporter::ZobristKeySizeStr() {
  return "Zobrist Key Size = " +
         key_type_strings.at(player_spec_.minimax_type_info.zobrist_key_type);
}

std::string PlayerInfoReporter::SummaryStr() {
  std::string result;
  if (player_spec_.evaluator_type == game::EvaluatorType::kMinimax) {
    return PlayerTypeStr() + ", " + EvaluatorTypeStr() + ", " + SearchDepthStr() + ", " +
           ZobristKeySizeStr();
  }
  if (player_spec_.evaluator_type == game::EvaluatorType::kHuman) {
    result = PlayerTypeStr();
  }

  if (player_spec_.evaluator_type == game::EvaluatorType::kRandom) {
    result = PlayerTypeStr() + ", " + EvaluatorTypeStr();
  }
  return result;
}

} // namespace terminalout