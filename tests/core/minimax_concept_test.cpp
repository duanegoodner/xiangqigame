#include <builders.hpp>
#include <concept_move_evaluator.hpp>
#include <game_board_for_concepts.hpp>
#include <gtest/gtest.h>
#include <move_evaluators_for_concepts.hpp>
#include <piece_position_points_for_concepts.hpp>
#include <type_traits>
#include <utility_functs.hpp>
#include <zobrist_for_concepts.hpp>



class MinimaxEvaluatorConceptTest : public ::testing::Test {
protected:
  const BoardMapInt_t kLateGameBoardMap{{
      {0, 0, 0, 1, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 5},
      {5, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, -1, 0, 0, 0, 0},
  }};

  using ExampleKeyTypeRed = uint64_t;
  using ExampleKeyTypeBlack = uint64_t;
  static constexpr size_t example_num_conf_keys_red_ = 1;
  static constexpr size_t example_num_conf_keys_black_ = 1;

  using ExampleCalculatorTypeRed =
      boardstate::ZobristCalculatorForConcepts<ExampleKeyTypeRed>;
  using ExamplCalculatorTypeBlack =
      boardstate::ZobristCalculatorForConcepts<ExampleKeyTypeBlack>;
  using ExampleGameBoardType = gameboard::GameBoardForConcepts;

  std::shared_ptr<ExampleGameBoardType> example_game_board_ =
      ExampleGameBoardType::Create();

  moveselection::MinimaxMoveEvaluatorFactory<
      ExampleKeyTypeRed,
      example_num_conf_keys_red_,
      ExampleGameBoardType>
      example_red_evaluator_factory_;

  moveselection::MinimaxMoveEvaluatorFactory<
      ExampleKeyTypeBlack,
      example_num_conf_keys_black_,
      ExampleGameBoardType>
      example_black_evaluator_factory_;

  DepthType default_search_depth_{4};

  template <
      typename KeyTypeRed,
      typename KeyTypeBlack,
      size_t NumConfKeysRed,
      size_t NumConfKeysBlack>
  void PlayGame(DepthType red_search_depth, DepthType black_search_depth) {
    using GameBoardType = gameboard::GameBoardForConcepts;
    // gameboard::GameBoardFactory<KeyTypeRed, KeyTypeBlack> game_board_factory;

    auto minimax_evaluator_red = example_red_evaluator_factory_.Create(
        example_game_board_,
        gameboard::PieceColor::kRed,
        red_search_depth
    );
    auto minimax_evaluator_black = example_black_evaluator_factory_.Create(
        example_game_board_,
        gameboard::PieceColor::kBlk,
        black_search_depth
    );

    PieceColor losing_player{};

    while (true) {
      auto red_moves = example_game_board_->CalcFinalMovesOf(PieceColor::kRed);
      if (red_moves.Size() == 0) {
        std::cout << "Red has no available moves" << std::endl;
        losing_player = PieceColor::kRed;
        break;
      }
      auto red_selected_move = minimax_evaluator_red->SelectMove(red_moves);
      auto red_executed_move = example_game_board_->ExecuteMove(red_selected_move);

      auto black_moves = example_game_board_->CalcFinalMovesOf(PieceColor::kBlk);
      if (black_moves.Size() == 0) {
        std::cout << "Black has no available moves" << std::endl;
        losing_player = PieceColor::kBlk;
        break;
      }

      auto black_selected_move = minimax_evaluator_black->SelectMove(black_moves);
      auto black_executed_move = example_game_board_->ExecuteMove(black_selected_move);
    }

    if (red_search_depth < black_search_depth) {
      EXPECT_TRUE(losing_player == PieceColor::kRed);
    }
    if (black_search_depth < red_search_depth) {
      EXPECT_TRUE(losing_player == PieceColor::kBlk);
    }
  }
};

TEST_F(MinimaxEvaluatorConceptTest, TestBuildEvaluator) {
  auto example_red_evaluator = example_red_evaluator_factory_.Create(
      example_game_board_,
      gameboard::PieceColor::kRed,
      default_search_depth_
  );
}

TEST_F(MinimaxEvaluatorConceptTest, TestBuildRedAndBlackEvaluators) {
  auto example_red_evaluator = example_red_evaluator_factory_.Create(
      example_game_board_,
      gameboard::PieceColor::kRed,
      default_search_depth_
  );

  auto example_black_evaluator = example_black_evaluator_factory_.Create(
      example_game_board_,
      gameboard::PieceColor::kRed,
      default_search_depth_
  );
}

TEST_F(MinimaxEvaluatorConceptTest, BoardStateHexStr) {
  auto example_red_evaluator = example_red_evaluator_factory_.Create(
      example_game_board_,
      gameboard::PieceColor::kRed,
      default_search_depth_
  );

  std::cout << example_red_evaluator->board_state_hex_str() << std::endl;
}

TEST_F(MinimaxEvaluatorConceptTest, RedStartingMoveSelection) {
  auto red_evaluator = example_red_evaluator_factory_.Create(
      example_game_board_,
      gameboard::PieceColor::kRed,
      default_search_depth_
  );

  auto allowed_moves = example_game_board_->CalcFinalMovesOf(PieceColor::kRed);
  auto red_selected_move = red_evaluator->SelectMove(allowed_moves);

  EXPECT_TRUE(
      (red_selected_move.start == BoardSpace{9, 1} &&
       red_selected_move.end == BoardSpace{7, 2}) ||
      (red_selected_move.start == BoardSpace{9, 7} &&
       red_selected_move.end == BoardSpace{7, 6})
  );
}

TEST_F(MinimaxEvaluatorConceptTest, TestPlayGame) {
  PlayGame<uint64_t, uint64_t, 1, 1>(2, 3);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}