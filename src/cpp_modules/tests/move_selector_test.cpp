
#include "board_components.hpp"
#include "hash_calculator.hpp"
#include <chrono>
#include <functional>
#include <game_board.hpp>
#include <gtest/gtest.h>
#include <hash_calculator.hpp>
#include <iostream>
#include <minimax_evaluator.hpp>
#include <move_selector.hpp>
#include <piece_points.hpp>

using namespace piece_points;
using namespace std;

class MoveSelectorTest : public ::testing::Test {

protected:
  GameBoard<HashCalculator> game_board_;
  PiecePointsEvaluator<GameBoard<HashCalculator>, PiecePoints>
      piece_points_evaluator_;
  const BoardMapInt_t kLateGameTestBoard{{
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
};

TEST_F(MoveSelectorTest, RandomMoveSelectorInitialMove) {
  RandomMoveSelector<GameBoard<HashCalculator>> move_selector;
  auto cur_moves = game_board_.CalcFinalMovesOf(PieceColor::kRed);
  auto selected_move = move_selector.SelectMove(game_board_, PieceColor::kRed);
  EXPECT_EQ(cur_moves.ContainsMove(selected_move), true);
};

TEST_F(MoveSelectorTest, InitializeMinimaxSelector) {
  int test_search_depth{4};
  MinimaxMoveSelector<
      PiecePointsEvaluator<GameBoard<HashCalculator>, PiecePoints>>
      move_selector(piece_points_evaluator_, test_search_depth);
}

TEST_F(MoveSelectorTest, AttachHashCalculators) {
  int test_search_depth{4};
  MinimaxMoveSelector<
      PiecePointsEvaluator<GameBoard<HashCalculator>, PiecePoints>>
      move_selector(piece_points_evaluator_, test_search_depth);
  auto red_hash_calculator = HashCalculator();
  auto black_hash_calculator = HashCalculator();
  game_board_.AttachHashCalculator(&red_hash_calculator, 0);
  game_board_.AttachHashCalculator(&red_hash_calculator, 1);
}

TEST_F(MoveSelectorTest, GetBoardState) {
  int test_search_depth{4};
  MinimaxMoveSelector<
      PiecePointsEvaluator<GameBoard<HashCalculator>, PiecePoints>>
      move_selector(piece_points_evaluator_, test_search_depth);
  auto red_hash_calculator = HashCalculator();
  auto black_hash_calculator = HashCalculator();
  EXPECT_EQ(red_hash_calculator.GetBoardState(), 0);
  EXPECT_EQ(black_hash_calculator.GetBoardState(), 0);
  game_board_.AttachHashCalculator(&red_hash_calculator, 0);
  game_board_.AttachHashCalculator(&black_hash_calculator, 1);
  auto red_board_state = red_hash_calculator.GetBoardState();
  auto black_board_state = black_hash_calculator.GetBoardState();
  EXPECT_NE(red_board_state, 0);
  EXPECT_EQ(red_board_state, black_board_state);
}

TEST_F(MoveSelectorTest, EndOfGameSelectorTest) {
  int test_search_depth{2};
  GameBoard<HashCalculator> end_of_game_board(kLateGameTestBoard);
  MinimaxMoveSelector<
      PiecePointsEvaluator<GameBoard<HashCalculator>, PiecePoints>>
      move_selector(piece_points_evaluator_, test_search_depth);
  auto selected_move =
      move_selector.SelectMove(end_of_game_board, PieceColor::kBlk);

  auto executed_move = end_of_game_board.ExecuteMove(selected_move);
  auto red_possible_moves =
      end_of_game_board.CalcFinalMovesOf(PieceColor::kRed);
  auto red_num_possible_moves = red_possible_moves.Size();
  EXPECT_TRUE(red_num_possible_moves == 0);
}

TEST_F(MoveSelectorTest, MinimaxSelectorPerformance) {
  int test_search_depth{4};
  MinimaxMoveSelector<
      PiecePointsEvaluator<GameBoard<HashCalculator>, PiecePoints>>
      move_selector(piece_points_evaluator_, test_search_depth);
  auto start_time = chrono::high_resolution_clock::now();
  auto selected_move = move_selector.SelectMove(game_board_, PieceColor::kRed);

  // Found move depends on search depth. If test_search_depth, need to modify
  // both assertions. (having this pair reduces likelihood of confusion if/when
  // test_test_search depth gets changed).
  EXPECT_TRUE(test_search_depth == 4);
  EXPECT_TRUE(
      (selected_move.start == BoardSpace{9, 1} &&
       selected_move.end == BoardSpace{7, 2}) ||
      (selected_move.start == BoardSpace{9, 7} &&
       selected_move.end == BoardSpace{7, 6})
  );

  auto end_time = std::chrono::high_resolution_clock::now();
  auto search_duration =
      std::chrono::duration_cast<chrono::milliseconds>(end_time - start_time);
  cout << "Selected Move from: " << selected_move.start.rank << ", "
       << selected_move.start.file << endl;
  cout << "To: " << selected_move.end.rank << ", " << selected_move.end.file
       << endl;
  cout << "Search time: " << search_duration.count() << "ms" << endl;
  cout << "Search depth: " << test_search_depth << endl;
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}