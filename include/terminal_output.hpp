#pragma once

#include <game_data_structs.hpp>
#include <iostream>
#include <string>
#include <unordered_map>

namespace terminalout {

const std::unordered_map<game::EvaluatorType, std::string> evaluator_to_player_type =
    [] {
      std::unordered_map<game::EvaluatorType, std::string> temp;
      temp.insert(std::make_pair(game::EvaluatorType::kHuman, "Human"));
      temp.insert(std::make_pair(game::EvaluatorType::kMinimax, "AI"));
      temp.insert(std::make_pair(game::EvaluatorType::kRandom, "AI"));
      return temp;
    }();

const std::unordered_map<game::EvaluatorType, std::string> evaluator_names = [] {
  std::unordered_map<game::EvaluatorType, std::string> temp;
  temp.insert(std::make_pair(game::EvaluatorType::kHuman, ""));
  temp.insert(std::make_pair(game::EvaluatorType::kMinimax, "Minimax"));
  temp.insert(std::make_pair(game::EvaluatorType::kRandom, "Random"));
  return temp;
}();

const std::unordered_map<game::ZobristKeyType, std::string> key_type_strings = [] {
  std::unordered_map<game::ZobristKeyType, std::string> temp;
  temp.insert(std::make_pair(game::ZobristKeyType::k032, "32"));
  temp.insert(std::make_pair(game::ZobristKeyType::k064, "64"));
  temp.insert(std::make_pair(game::ZobristKeyType::k128, "128"));
  return temp;
}();

class PlayerInfoReporter {
  game::PlayerSpec player_spec_;

public:
  PlayerInfoReporter(game::PlayerSpec player_spec);
  void DisplaySummaryStr() { std::cout << SummaryStr() << std::endl; }

private:
  std::string PlayerTypeStr();
  std::string EvaluatorTypeStr();
  std::string SearchDepthStr();
  std::string ZobristKeySizeStr();
  std::string SummaryStr();
};

} // namespace terminalout