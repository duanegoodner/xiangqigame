#pragma once

#include <any>
#include <game/game_data_structs.hpp>
#include <game/player_input_translator.hpp>
#include <gameboard/board_data_structs.hpp>
#include <gameboard/game_piece.hpp>
#include <optional>
#include <string>
#include <unordered_set>
#include <utilities/integer_types.hpp>
#include <variant>

namespace game {
using PlayerInputType = std::unordered_map<
    std::string,
    std::variant<std::string, size_t, DepthType>>;

class PlayerSpecBuilder {
  gameboard::PieceColor color_;
  EvaluatorType evaluator_type_;
  // ZobristKeyType zobrist_key_type_;
  size_t zobrist_key_size_bits_;
  // ZobristCalculatorCount zobrist_calculator_count_;
  size_t zobrist_calculator_count_;
  DepthType minimax_search_depth_;
  PlayerInputTranslator input_translator_;

  const std::unordered_set<std::string> allowed_keys_ = {
      "color",
      "evaluator_type",
      "key_size_bits",
      "num_zobrist_calculators",
      "minimax_search_depth"
  };

  // Helper function to extract and set values from the input map
  template <typename T, typename F>
  void TrySetAttribute(
      const PlayerInputType &input_map,
      const std::string &key,
      F setter
  ) {
    if (input_map.count(key) > 0) {
      try {
        setter(std::get<T>(input_map.at(key)));
      } catch (const std::bad_variant_access &e) {
        throw std::runtime_error("Invalid type for key: " + key);
      }
    }
  }

public:
  PlayerSpecBuilder()
      : color_{gameboard::PieceColor::kNul}
      , evaluator_type_{EvaluatorType::kMinimax}
      // , zobrist_key_type_{ZobristKeyType::k064BitKey}
      , zobrist_key_size_bits_{64}
      // , zobrist_calculator_count_{ZobristCalculatorCount::kTwoZCalc}
      , zobrist_calculator_count_{2}
      , minimax_search_depth_{4} {}

  PlayerSpecBuilder &SetMultipleAttributes(const PlayerInputType &player_input) {
    // Validate the presence of the required "color" key
    if (player_input.count("color") == 0) {
      throw std::runtime_error("Input validation failed: 'color' key is required.");
    }

    // Validate all keys are allowed
    for (const auto &[key, _] : player_input) {
      if (allowed_keys_.count(key) == 0) {
        throw std::runtime_error(
            "Input validation failed: Unrecognized key '" + key + "'."
        );
      }
    }

    // Use helper function to handle each attribute
    TrySetAttribute<std::string>(player_input, "color", [this](auto color_str) {
      auto color = input_translator_.GetPieceColorFromString(color_str);
      color_ = color;
    });

    TrySetAttribute<std::string>(
        player_input,
        "evaluator_type",
        [this](auto evaluator_type_str) {
          evaluator_type_ =
              input_translator_.GetEvaluatorTypeFromString(evaluator_type_str);
        }
    );

    TrySetAttribute<size_t>(player_input, "key_size_bits", [this](auto key_size_bits) {
      zobrist_key_size_bits_ = key_size_bits;
      // zobrist_key_type_ = input_translator_.GetZobristKeyTypeFromInt(key_size_bits);
    });

    TrySetAttribute<size_t>(
        player_input,
        "num_zobrist_calculators",
        [this](auto num_calculators) {
          zobrist_calculator_count_ = num_calculators;
          // zobrist_calculator_count_ =
          //     input_translator_.GetZobristCalculatorCountFromInt(num_calculators);
        }
    );

    TrySetAttribute<DepthType>(player_input, "minimax_search_depth", [this](auto depth) {
      minimax_search_depth_ = depth;
    });

    return *this;
  }

  PlayerSpec Build() const {
    return PlayerSpec{
        color_,
        evaluator_type_,
        zobrist_key_size_bits_,
        zobrist_calculator_count_,
        minimax_search_depth_
    };
  }
};

} // namespace game