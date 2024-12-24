#pragma once

#include <gameboard/move_data_structs.hpp>
#include <iostream>
#include <moveselection/evaluator_data_structs.hpp>
#include <optional>
#include <utilities/integer_types.hpp>

namespace game {
enum GameState : int { kUnfinished = 0, kDraw = 1, kRedWon = 2, kBlkWon = 3 };

enum EvaluatorType : int { kRandom = 0, kMinimax = 1, kHuman };
enum ZobristKeyType : int { kNoKey = 0, k032BitKey = 1, k064BitKey = 2, k128BitKey = 3 };

enum ZobristCalculatorCount : int { kNoZCalcs = 0, kOneZCalc = 1, kTwoZCalc = 2 };

struct MinimaxTypeInfo {
  ZobristKeyType zobrist_key_type;
  ZobristCalculatorCount zobrist_calculator_count;

  MinimaxTypeInfo(
      ZobristKeyType zobrist_key_type = ZobristKeyType::kNoKey,
      ZobristCalculatorCount zobrist_calculator_count = ZobristCalculatorCount::kNoZCalcs
  )
      : zobrist_key_type{zobrist_key_type}
      , zobrist_calculator_count{zobrist_calculator_count} {}

  bool operator==(const MinimaxTypeInfo &other) const {
    return zobrist_key_type == other.zobrist_key_type &&
           zobrist_calculator_count == other.zobrist_calculator_count;
  }
};

struct MinimaxTypeInfoHash {
  std::size_t operator()(const MinimaxTypeInfo &key) const {
    std::size_t h1 = std::hash<int>{}(key.zobrist_key_type);
    std::size_t h2 = std::hash<int>{}(key.zobrist_calculator_count);
    // Combine the two hash values
    return h1 ^ (h2 << 1); // XOR and shift
  }
};

struct PlayerSpec {
  gameboard::PieceColor color;
  EvaluatorType evaluator_type;
  // ZobristKeyType zobrist_key_type;
  size_t zobrist_key_size_bits;
  // ZobristCalculatorCount zobrist_calculator_count;
  size_t zobrist_calculator_count;
  DepthType minimax_search_depth;

  PlayerSpec(
      gameboard::PieceColor color,
      EvaluatorType evaluator_type = EvaluatorType::kMinimax,
      // ZobristKeyType zobrist_key_type = ZobristKeyType::k064BitKey,
      size_t zobrist_key_size_bits = 64,
      size_t zobrist_calculator_count = 2,
      DepthType minimax_search_depth = 0
  )
      : color{color}
      , evaluator_type{evaluator_type}
      // , zobrist_key_type{zobrist_key_type}
      , zobrist_key_size_bits{zobrist_key_size_bits}
      , zobrist_calculator_count{zobrist_calculator_count}
      , minimax_search_depth{minimax_search_depth} {}
};

struct GameStatus {
  game::GameState game_state;
  const std::vector<gameboard::ExecutedMove> &move_log;
  gameboard::PieceColor whose_turn;
  bool is_in_check;
  const gameboard::BoardMap_t &board_map;
};

struct GameSummary {
  std::string game_id;
  game::GameState game_state;
  std::vector<ExecutedMove> move_log;
  std::unordered_map<gameboard::PieceColor, PlayerSpec> player_specs;
  std::unordered_map<gameboard::PieceColor, moveselection::SearchSummaries>
      search_summaries;
};

} // namespace game
