#include <terminal_output.hpp>

namespace terminalout {

// Definition of static constexpr maps
const std::unordered_map<game::EvaluatorType, std::string>
    PlayerInfoReporter::evaluator_to_player_type_ = {
        {game::EvaluatorType::kHuman, "Human"},
        {game::EvaluatorType::kMinimax, "AI"},
        {game::EvaluatorType::kRandom, "AI"}
};

const std::unordered_map<game::EvaluatorType, std::string>
    PlayerInfoReporter::evaluator_names_ = {
        {game::EvaluatorType::kHuman, ""},
        {game::EvaluatorType::kMinimax, "Minimax"},
        {game::EvaluatorType::kRandom, "Random"}
};

const std::unordered_map<game::ZobristKeyType, std::string>
    PlayerInfoReporter::key_type_strings_ = {
        {game::ZobristKeyType::k032, "32"},
        {game::ZobristKeyType::k064, "64"},
        {game::ZobristKeyType::k128, "128"}
};

PlayerInfoReporter::PlayerInfoReporter(const game::PlayerSpec &player_spec)
    : player_spec_{player_spec} {}

std::string PlayerInfoReporter::PlayerTypeStr() {
  return "Player type = " + evaluator_to_player_type_.at(player_spec_.evaluator_type);
}

std::string PlayerInfoReporter::EvaluatorTypeStr() {
  return "Move Evaluator = " + evaluator_names_.at(player_spec_.evaluator_type);
}

std::string PlayerInfoReporter::SearchDepthStr() {
  return "Max Search Depth = " + std::to_string(player_spec_.minimax_search_depth);
}

std::string PlayerInfoReporter::ZobristKeySizeStr() {
  return "Zobrist Key Size = " +
         key_type_strings_.at(player_spec_.minimax_type_info.zobrist_key_type);
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