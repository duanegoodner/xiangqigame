
#include "hash_calculator.hpp"
#include <chrono>
#include <functional>
#include <gtest/gtest.h>
#include <iostream>
#include <move_selector.hpp>
#include <piece_points.hpp>
#include <minimax_evaluator.hpp>
#include <game_board.hpp>
#include <hash_calculator.hpp>

using namespace piece_points;
using namespace std;

class MoveSelectorTest : public ::testing::Test {

protected:
  GameBoard<HashCalculator> game_board_;
  // PiecePointsEvaluator<GameBoard> piece_points_evaluator_(DEFAULT_GAME_POINTS);
  PiecePointsEvaluator<GameBoard<HashCalculator>> piece_points_evaluator_;

  // MoveSelectorTest()
  //     : game_board_{GameBoard()}
  //     , piece_points_evaluator_{PiecePointsEvaluator(DEFAULT_GAME_POINTS)} {};
};

TEST_F(MoveSelectorTest, RandomMoveSelectorInitialMove) {
  RandomMoveSelector<GameBoard<HashCalculator>> move_selector;
  auto cur_moves = game_board_.CalcFinalMovesOf(PieceColor::kRed);
  auto selected_move = move_selector.SelectMove(game_board_, PieceColor::kRed);
  EXPECT_EQ(cur_moves.Contains(selected_move), true);
};

TEST_F(MoveSelectorTest, InitializeConcreteMinimaxSelector) {
  int test_search_depth{4};
  MinimaxMoveSelector<PiecePointsEvaluator<GameBoard<HashCalculator>>> move_selector(
      piece_points_evaluator_,
      test_search_depth
  );
  auto start_time = chrono::high_resolution_clock::now();
  auto selected_move = move_selector.SelectMove(game_board_, PieceColor::kRed);
  auto end_time = std::chrono::high_resolution_clock::now();
  auto search_duration =
      std::chrono::duration_cast<chrono::milliseconds>(end_time - start_time);
  cout << "Selected Move from: "
    << selected_move.start.rank << ", " << selected_move.start.file << endl;
  cout << "To: " << selected_move.end.rank << ", " << selected_move.end.file << endl;
  cout << "Search time: " << search_duration.count() << "ms" << endl;
  cout << "Search depth: " << test_search_depth << endl;
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}