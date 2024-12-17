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