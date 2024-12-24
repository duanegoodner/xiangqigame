#pragma once

#include <game/game_data_structs.hpp>
#include <gameboard/game_piece.hpp>
#include <string>
#include <unordered_map>

namespace game {
class PlayerInputTranslator {

public:
  gameboard::PieceColor GetPieceColorFromString(std::string input_color_name) {
    return piece_color_name_to_enum_.at(input_color_name);
  }

  EvaluatorType GetEvaluatorTypeFromString(std::string input_evaluator_name) {
    return evaluator_name_to_enum_.at(input_evaluator_name);
  }

private:
  inline static const std::unordered_map<std::string, gameboard::PieceColor>
      piece_color_name_to_enum_ = {
          {"red", gameboard::PieceColor::kRed},
          {"black", gameboard::PieceColor::kBlk}
  };

  inline static const std::unordered_map<std::string, EvaluatorType>
      evaluator_name_to_enum_ = {
          {"random", EvaluatorType::kRandom},
          {"minimax", EvaluatorType::kMinimax},
          {"person", EvaluatorType::kHuman}
  };
};
} // namespace game