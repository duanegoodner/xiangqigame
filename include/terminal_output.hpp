#pragma once

#include <game_data_structs.hpp>
#include <iostream>
#include <string>
#include <unordered_map>

namespace terminalout {

class PlayerInfoReporter {
  const game::PlayerSpec &player_spec_;

public:
  PlayerInfoReporter(const game::PlayerSpec &player_spec);
  void DisplaySummaryStr() { std::cout << SummaryStr() << std::endl; }

private:
  static const std::unordered_map<game::EvaluatorType, std::string>
      evaluator_to_player_type_;
  static const std::unordered_map<game::EvaluatorType, std::string> evaluator_names_;
  static const std::unordered_map<game::ZobristKeyType, std::string> key_type_strings_;

  std::string PlayerTypeStr();
  std::string EvaluatorTypeStr();
  std::string SearchDepthStr();
  std::string ZobristKeySizeStr();
  std::string SummaryStr();
};

class GameInfoReporter {

};

} // namespace terminalout