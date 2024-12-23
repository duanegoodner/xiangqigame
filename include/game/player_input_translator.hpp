#pragma once

#include <game/game_data_structs.hpp>
#include <gameboard/game_piece.hpp>
#include <string>
#include <unordered_map>

namespace game {
class PlayerInputTranslator {

public:
  ZobristKeyType GetZobristKeyTypeFromInt(size_t input_bit_count) {
    return zkey_bitcount_to_enum_.at(input_bit_count);
  }

  ZobristCalculatorCount GetZobristCalculatorCountFromInt(size_t input_calculator_count
  ) {
    return num_zcalculators_to_enum_.at(input_calculator_count);
  }

  gameboard::PieceColor GetPieceColorFromString(std::string input_color_name) {
    return piece_color_name_to_enum_.at(input_color_name);
  }

  EvaluatorType GetEvaluatorTypeFromString(std::string input_evaluator_name) {
    return evaluator_name_to_enum_.at(input_evaluator_name);
  } 

private:
  inline static const std::unordered_map<size_t, ZobristKeyType> zkey_bitcount_to_enum_ =
      {{0, ZobristKeyType::kNoKey},
       {32, ZobristKeyType::k032BitKey},
       {64, ZobristKeyType::k064BitKey},
       {128, ZobristKeyType::k128BitKey}};

  inline static const std::unordered_map<size_t, ZobristCalculatorCount>
      num_zcalculators_to_enum_ = {
          {0, ZobristCalculatorCount::kNoZCalcs},
          {1, ZobristCalculatorCount::kOneZCalc},
          {2, ZobristCalculatorCount::kTwoZCalc}
  };

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