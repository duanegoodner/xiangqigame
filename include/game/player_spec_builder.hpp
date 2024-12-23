#pragma once

#include <any>
#include <game/game_data_structs.hpp>
#include <game/player_input_translator.hpp>
#include <gameboard/board_data_structs.hpp>
#include <gameboard/game_piece.hpp>
#include <optional>
#include <string>
#include <utilities/integer_types.hpp>

namespace game {

class PlayerSpecBuilder {
  gameboard::PieceColor color_;
  EvaluatorType evaluator_type_;
  ZobristKeyType zobrist_key_type_;
  ZobristCalculatorCount zobrist_calculator_count_;
  DepthType minimax_search_depth_;
  PlayerInputTranslator input_translator_;

public:
  PlayerSpecBuilder()
      : color_{gameboard::PieceColor::kNul}
      , evaluator_type_{EvaluatorType::kMinimax}
      , zobrist_key_type_{ZobristKeyType::k064BitKey}
      , zobrist_calculator_count_{ZobristCalculatorCount::kTwoZCalc}
      , minimax_search_depth_{4} {}

  PlayerSpecBuilder &SetColor(gameboard::PieceColor color) {
    color_ = color;
    return *this;
  }

  PlayerSpecBuilder &SetColor(std::string color_str) {
    auto translated_color = input_translator_.GetPieceColorFromString(color_str);
    color_ = translated_color;
    return *this;
  }

  PlayerSpecBuilder &SetEvaluatorType(EvaluatorType evaluator_type) {
    evaluator_type_ = evaluator_type;
    return *this;
  }

  PlayerSpecBuilder &SetZobristKeyType(ZobristKeyType zobrist_key_type) {
    zobrist_key_type_ = zobrist_key_type;
    return *this;
  }

  PlayerSpecBuilder &SetZobristKeyType(size_t key_size_bits) {
    auto translated_key_type = input_translator_.GetZobristKeyTypeFromInt(key_size_bits);
    zobrist_key_type_ = translated_key_type;
    return *this;
  }

  PlayerSpecBuilder &SetEvaluatorType(std::string evaluator_type_str) {
    auto translated_evaluator_type =
        input_translator_.GetEvaluatorTypeFromString(evaluator_type_str);
    evaluator_type_ = translated_evaluator_type;
    return *this;
  }

  PlayerSpecBuilder &SetZobristCalculatorCount(
      ZobristCalculatorCount zobrist_calculator_count
  ) {
    zobrist_calculator_count_ = zobrist_calculator_count;
  }

  PlayerSpecBuilder &SetZobristCalculatorCount(size_t zobrist_calculator_count) {
    auto translated_count =
        input_translator_.GetZobristCalculatorCountFromInt(zobrist_calculator_count);
    zobrist_calculator_count_ = translated_count;
  }

  PlayerSpecBuilder &SetMinimaxSearchDepth(DepthType depth) {
    minimax_search_depth_ = depth;
    return *this;
  }

  PlayerSpecBuilder &SetMultipleAttributes(
      std::unordered_map<std::string, std::any> player_input
  ) {

    auto color = std::any_cast<gameboard::PieceColor>(player_input.at("color"));
    SetColor(color);

    if (player_input.count("evaluator_type") > 0) {
      auto evaluator_type_str =
          std::any_cast<EvaluatorType>(player_input.at("evaluator_type"));
      SetEvaluatorType(evaluator_type_str);
    }

    if (player_input.count("key_size_bits") > 0) {
        auto key_size_bits = std::any_cast<size_t>(player_input.at("key_size_bits"));
        SetZobristKeyType(key_size_bits);
    }

    if (player_input.count("num_zobrist_calculators") > 0) {
        auto num_calculators = std::any_cast<size_t>(player_input.at("num_zobrist_calculators"));
        SetZobristCalculatorCount(num_calculators);
    }

    if (player_input.count("minimax_search_depth") > 0) {
        auto search_depth = std::any_cast<DepthType>(player_input.at("minimax_search_depth"));
        SetMinimaxSearchDepth(search_depth);
    }

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