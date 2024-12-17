#pragma once

#include <base_evaluator_factory.hpp>
#include <base_move_evaluator.hpp>
#include <base_space_info_provider.hpp>
#include <builders.hpp>
#include <concept_composite_concepts.hpp>
#include <concept_move_evaluator.hpp>
#include <game.hpp>
#include <game_board_for_concepts.hpp>
#include <memory>
#include <move_evaluator_human_for_concepts.hpp>
#include <move_evaluator_minimax_for_concepts.hpp>
#include <move_evaluator_random_for_concepts.hpp>
#include <unordered_map>

namespace game {
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

struct EvaluatorFactoryInfo {
  EvaluatorType evaluator_type;
  MinimaxTypeInfo minimax_type_info;
  size_t minimax_search_depth;
  std::istream &input_stream;

  EvaluatorFactoryInfo(
      EvaluatorType evaluator_type = EvaluatorType::kRandom,
      MinimaxTypeInfo minimax_type_info = MinimaxTypeInfo{},
      size_t minimax_search_depth = 0,
      std::istream &input_stream = std::cin
  )
      : evaluator_type{evaluator_type}
      , input_stream{input_stream}
      , minimax_type_info{minimax_type_info}
      , minimax_search_depth{minimax_search_depth} {}
};

class EvaluatorFactoryRetriever {
  std::unordered_map<
      MinimaxTypeInfo,
      std::shared_ptr<MoveEvaluatorFactoryBase>,
      MinimaxTypeInfoHash>
      minimax_factories_;
  const EvaluatorFactoryInfo &evaluator_factory_info_;
  std::shared_ptr<gameboard::GameBoardForConcepts> game_board_;

public:
  EvaluatorFactoryRetriever(
      const EvaluatorFactoryInfo &evaluator_factory_info,
      std::shared_ptr<gameboard::GameBoardForConcepts> game_board
  );

  std::shared_ptr<MoveEvaluatorFactoryBase> GetFactory();
};

class GameFactory {
  gameboard::GameBoardFactory game_board_factory_;
  EvaluatorFactoryInfo red_evaluator_factory_info_;
  EvaluatorFactoryInfo black_evaluator_factory_info_;

public:
  GameFactory(
      EvaluatorFactoryInfo red_evaluator_factory_info,
      EvaluatorFactoryInfo black_evaluator_factory_info
  )
      : red_evaluator_factory_info_{red_evaluator_factory_info}
      , black_evaluator_factory_info_{black_evaluator_factory_info}
      , game_board_factory_{gameboard::GameBoardFactory{}} {}

  std::unique_ptr<Game> Create() {
    auto game_board = game_board_factory_.Create();
    std::unordered_map<gameboard::PieceColor, std::unique_ptr<MoveEvaluatorBase>>
        evaluators;

    auto evaluator_factory_retriever_red =
        EvaluatorFactoryRetriever{red_evaluator_factory_info_, game_board};
    auto evalutor_factory_red = evaluator_factory_retriever_red.GetFactory();
    evaluators.emplace(
        gameboard::PieceColor::kRed,
        evalutor_factory_red->Create(gameboard::PieceColor::kRed)
    );

    auto evaluator_factory_retriever_black =
        EvaluatorFactoryRetriever{black_evaluator_factory_info_, game_board};
    auto evalutor_factory_black = evaluator_factory_retriever_black.GetFactory();
    evaluators.emplace(
        gameboard::PieceColor::kBlk,
        evalutor_factory_red->Create(gameboard::PieceColor::kBlk)
    );

    return std::make_unique<Game>(game_board, std::move(evaluators));
  }
};

} // namespace game
