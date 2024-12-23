#pragma once

#include <any>
#include <game/game_data_structs.hpp>
#include <game/player_input_translator.hpp>
#include <gameboard/board_data_structs.hpp>
#include <gameboard/game_piece.hpp>
#include <optional>
#include <string>

namespace game {

class PlayerSpecBuilder {
  gameboard::PieceColor color_;
  EvaluatorType evaluator_type_;
  //   MinimaxTypeInfo minimax_type_info_;
  ZobristKeyType zobrist_key_type_;
  ZobristCalculatorCount zobrist_calculator_count_;
  size_t minimax_search_depth_;
  std::unordered_map<std::string, std::any> provided_inputs_;
  PlayerInputTranslator input_translator_;

  inline static const std::unordered_map<std::string, std::any> default_inputs_ = {
      {"evaluator_type", "minimax"},
      {"key_size_bits", 64},
      {"num_zobrist_calculators", 2},
      {"minimax_search_depth", 4}
  };

public:
  PlayerSpecBuilder(std::unordered_map<std::string, std::any> provided_inputs)
      : color_{gameboard::PieceColor::kNul}
      , evaluator_type_{EvaluatorType::kMinimax}
      , zobrist_key_type_{ZobristKeyType::k064BitKey}
      , zobrist_calculator_count_{ZobristCalculatorCount::kTwoZCalc}
      , minimax_search_depth_{4}
      , provided_inputs_{provided_inputs} {}

  PlayerSpecBuilder &SetColor(std::string color_str) {
    auto translated_color = input_translator_.GetPieceColorFromString(color_str);
    color_ = translated_color;
    return *this;
  }

  PlayerSpecBuilder &SetEvaluatorType(EvaluatorType evaluator_type) {
    evaluator_type_ = evaluator_type;
    return *this;
  }

  PlayerSpecBuilder &SetEvaluatorType(std::string evaluator_type_str) {
    auto translated_evaluator_type =
        input_translator_.GetEvaluatorTypeFromString(evaluator_type_str);
    evaluator_type_ = translated_evaluator_type;
    return *this;
  }

  PlayerSpecBuilder &SetZobristCalculatorCount(size_t zobrist_calculator_count) {
    auto translated_count =
        input_translator_.GetZobristCalculatorCountFromInt(zobrist_calculator_count);
  }

  PlayerSpecBuilder &SetMinimaxSearchDepth(size_t depth) {
    minimax_search_depth_ = depth;
    return *this;
  }

  PlayerSpec Build() const {
    return PlayerSpec{
        color_,
        evaluator_type_,
        zobrist_key_type_,
        zobrist_calculator_count_,
        minimax_search_depth_
    };
  }
};

} // namespace game