#pragma once

#include <iostream>
#include <move_data_structs.hpp>
#include <optional>

namespace game {
enum GameState : int { kUnfinished = 0, kDraw = 1, kRedWon = 2, kBlkWon = 3 };

enum EvaluatorType : int { kRandom = 0, kMinimax = 1, kHuman };
enum ZobristKeyType : int { kNoKey = 0, k032 = 1, k064 = 2, k128 = 3 };

enum ZobristCalculatorCount : int { kNoZCalcs = 0, kOne = 1, kTwo = 2 };

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
  MinimaxTypeInfo minimax_type_info;
  size_t minimax_search_depth;
  std::istream &input_stream;

  PlayerSpec(
      gameboard::PieceColor color,
      EvaluatorType evaluator_type,
      MinimaxTypeInfo minimax_type_info = MinimaxTypeInfo{},
      size_t minimax_search_depth = 0,
      std::istream &input_stream = std::cin
  )
      : color{color}
      , evaluator_type{evaluator_type}
      , input_stream{input_stream}
      , minimax_type_info{minimax_type_info}
      , minimax_search_depth{minimax_search_depth} {}
};

struct GameStatus {
  const std::vector<gameboard::ExecutedMove> &move_log;
  gameboard::PieceColor whose_turn;
  bool is_in_check;
  const gameboard::BoardMap_t &board_map;
};


} // namespace game
