#include <base_evaluator_factory.hpp>
#include <base_move_evaluator.hpp>
#include <base_space_info_provider.hpp>
#include <builders.hpp>
#include <game.hpp>
#include <game_board_for_concepts.hpp>
#include <game_factory.hpp>
#include <game_piece.hpp>
#include <gtest/gtest.h>
#include <integer_types.hpp>
#include <iostream>
#include <memory>
#include <move_evaluator_human_for_concepts.hpp>
#include <move_evaluator_minimax_for_concepts.hpp>
#include <move_evaluator_random_for_concepts.hpp>
#include <optional>
#include <piece_position_points_for_concepts.hpp>
#include <unordered_map>
#include <vector>
#include <zobrist_for_concepts.hpp>

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
  )
      : evaluator_factory_info_{evaluator_factory_info}
      , game_board_{game_board}
      , minimax_factories_{} {
    minimax_factories_.emplace(
        MinimaxTypeInfo{ZobristKeyType::k032, ZobristCalculatorCount::kOne},
        std::make_shared<moveselection::MinimaxMoveEvaluatorFactory<uint32_t, 0>>(
            game_board_,
            evaluator_factory_info.minimax_search_depth
        )
    );
    minimax_factories_.emplace(
        MinimaxTypeInfo{ZobristKeyType::k064, ZobristCalculatorCount::kOne},
        std::make_shared<moveselection::MinimaxMoveEvaluatorFactory<uint32_t, 0>>(
            game_board_,
            evaluator_factory_info.minimax_search_depth
        )
    );
    minimax_factories_.emplace(
        MinimaxTypeInfo{ZobristKeyType::k128, ZobristCalculatorCount::kOne},
        std::make_shared<moveselection::MinimaxMoveEvaluatorFactory<__uint128_t, 0>>(
            game_board_,
            evaluator_factory_info.minimax_search_depth
        )
    );
    minimax_factories_.emplace(
        MinimaxTypeInfo{ZobristKeyType::k032, ZobristCalculatorCount::kTwo},
        std::make_shared<moveselection::MinimaxMoveEvaluatorFactory<uint32_t, 1>>(
            game_board_,
            evaluator_factory_info.minimax_search_depth
        )
    );
    minimax_factories_.emplace(
        MinimaxTypeInfo{ZobristKeyType::k064, ZobristCalculatorCount::kTwo},
        std::make_shared<moveselection::MinimaxMoveEvaluatorFactory<uint32_t, 1>>(
            game_board_,
            evaluator_factory_info.minimax_search_depth
        )
    );
    minimax_factories_.emplace(
        MinimaxTypeInfo{ZobristKeyType::k128, ZobristCalculatorCount::kOne},
        std::make_shared<moveselection::MinimaxMoveEvaluatorFactory<__uint128_t, 0>>(
            game_board_,
            evaluator_factory_info.minimax_search_depth
        )
    );
  }

  std::shared_ptr<MoveEvaluatorFactoryBase> GetFactory() {
    std::shared_ptr<MoveEvaluatorFactoryBase> factory;

    if (evaluator_factory_info_.evaluator_type == EvaluatorType::kRandom) {
      factory = std::make_shared<moveselection::RandomMoveEvaluatorFactory>();
    }
    if (evaluator_factory_info_.evaluator_type == EvaluatorType::kHuman) {
      factory = std::make_shared<moveselection::HumanMoveEvaluatorFactory>(
          evaluator_factory_info_.input_stream
      );
    }
    if (evaluator_factory_info_.evaluator_type == EvaluatorType::kMinimax) {
      factory = minimax_factories_.at(evaluator_factory_info_.minimax_type_info);
    }
    return factory;
  }
};

class GameTest : public ::testing::Test {
protected:
  gameboard::GameBoardFactory game_board_factory_;
  std::shared_ptr<gameboard::GameBoardForConcepts> game_board_ =
      game_board_factory_.Create();
  DepthType default_search_depth_{3};
};

TEST_F(GameTest, InstantiateGame) {
  std::unordered_map<gameboard::PieceColor, std::unique_ptr<MoveEvaluatorBase>>
      evaluators;

  moveselection::MinimaxMoveEvaluatorFactory<uint64_t, 1> red_evaluator_factory{
      game_board_,
      default_search_depth_
  };
  moveselection::MinimaxMoveEvaluatorFactory<uint64_t, 1> black_evaluator_factory{
      game_board_,
      default_search_depth_
  };

  evaluators.emplace(
      gameboard::PieceColor::kRed,
      red_evaluator_factory.Create(gameboard::PieceColor::kRed)
  );
  evaluators.emplace(
      gameboard::PieceColor::kBlk,
      black_evaluator_factory.Create(gameboard::PieceColor::kBlk)
  );

  auto game = game::Game(game_board_, std::move(evaluators));

  game.Play();
}

//! Builds a Game using helper classes implemented in this test file
TEST_F(GameTest, BuildGameWithEvaluatorFactoryInfo) {
  std::unordered_map<gameboard::PieceColor, std::unique_ptr<MoveEvaluatorBase>>
      evaluators;

  DepthType red_search_depth{3};
  auto minimax_type_info_red =
      MinimaxTypeInfo{ZobristKeyType::k064, ZobristCalculatorCount::kTwo};
  auto evaluator_factory_info_red = EvaluatorFactoryInfo(
      EvaluatorType::kMinimax,
      minimax_type_info_red,
      red_search_depth
  );
  auto evaluator_factory_retriever_red =
      EvaluatorFactoryRetriever{evaluator_factory_info_red, game_board_};
  auto evalutor_factory_red = evaluator_factory_retriever_red.GetFactory();
  evaluators.emplace(
      gameboard::PieceColor::kRed,
      evalutor_factory_red->Create(gameboard::PieceColor::kRed)
  );

  DepthType black_search_depth{3};
  auto minimax_type_info_black =
      MinimaxTypeInfo{ZobristKeyType::k064, ZobristCalculatorCount::kTwo};
  auto evaluator_factory_info_black = EvaluatorFactoryInfo(
      EvaluatorType::kMinimax,
      minimax_type_info_black,
      black_search_depth
  );
  auto evaluator_factory_retriever_black =
      EvaluatorFactoryRetriever{evaluator_factory_info_black, game_board_};
  auto evalutor_factory_black = evaluator_factory_retriever_black.GetFactory();
  evaluators.emplace(
      gameboard::PieceColor::kBlk,
      evalutor_factory_red->Create(gameboard::PieceColor::kBlk)
  );

  auto game = game::Game(game_board_, std::move(evaluators));
  game.Play();
}

TEST_F(GameTest, BuildGameWithGameFactory) {
  DepthType red_search_depth{3};
  DepthType black_search_depth{2};

  auto minimax_type_info_red = game::MinimaxTypeInfo{
      game::ZobristKeyType::k064,
      game::ZobristCalculatorCount::kTwo
  };
  auto evaluator_factory_info_red = game::EvaluatorFactoryInfo(
      game::EvaluatorType::kMinimax,
      minimax_type_info_red,
      red_search_depth
  );

  auto minimax_type_info_black = game::MinimaxTypeInfo{
      game::ZobristKeyType::k064,
      game::ZobristCalculatorCount::kTwo
  };
  auto evaluator_factory_info_black = game::EvaluatorFactoryInfo(
      game::EvaluatorType::kMinimax,
      minimax_type_info_black,
      black_search_depth
  );

  auto game_factory =
      game::GameFactory(evaluator_factory_info_red, evaluator_factory_info_black);
  auto game = game_factory.Create();
  game->Play();
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}