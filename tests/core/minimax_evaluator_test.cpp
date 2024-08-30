#include <game_board.hpp>
#include <gtest/gtest.h>
#include <hash_calculator.hpp>
#include <move_evaluators.hpp>
#include <piece_points.hpp>
#include <utility_functs.hpp>

using namespace piece_points;

class RandomEvaluatorTest : public ::testing::Test {
protected:
  static bool moveComparator(const Move &lhs, const Move &rhs) {
    if (lhs.start.rank != rhs.start.rank)
      return lhs.start.rank < rhs.start.rank;
    if (lhs.start.file != rhs.start.file)
      return lhs.start.file < rhs.start.file;
    if (lhs.end.rank != rhs.end.rank)
      return lhs.end.rank < rhs.end.rank;
    return lhs.end.file < rhs.end.file;
  }
};

// Red selects starting move 10 times. If choice is random, we can be 
// almost certain that the number of unique selected Moves will be > 5.
TEST_F(RandomEvaluatorTest, TestStartingMoveSelection) {
  int num_first_move_selections = 10;

  NewGameBoard<HashCalculator> starting_board;
  RandomMoveEvaluator<NewGameBoard<HashCalculator>> red_evaluator{
      PieceColor::kRed,
      starting_board
  };

  std::set<Move, bool (*)(const Move &, const Move &)> move_set(moveComparator
  );

  for (auto idx = 0; idx < num_first_move_selections; idx++) {
    auto red_selected_move = red_evaluator.SelectMove();
    move_set.insert(red_selected_move);
  }
  EXPECT_TRUE(move_set.size() > 5);
}

class MinimaxEvaluatorTest : public ::testing::Test {

protected:
  const string points_spec_path =
      utility_functs::get_data_file_abs_path("ICGA_2004_bpo.json");

  const PointsSpecBPOExternal external_pts_spec =
      PointsSpecBPOExternal(points_spec_path);
  const PiecePoints imported_piece_points{external_pts_spec};

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
  NewGameBoard<HashCalculator> late_game_board_{kLateGameBoardMap};

  const int standard_search_depth = 4;
};

TEST_F(MinimaxEvaluatorTest, TestConstructorsWithDefaultPiecePoints) {
  NewGameBoard<HashCalculator> starting_board;
  MinimaxMoveEvaluator<NewGameBoard<HashCalculator>, PiecePoints>
      red_evaluator{PieceColor::kRed, standard_search_depth, starting_board};
  MinimaxMoveEvaluator<NewGameBoard<HashCalculator>, PiecePoints>
      black_evaluator{PieceColor::kBlk, standard_search_depth, starting_board};
}

TEST_F(MinimaxEvaluatorTest, TestConstructorsWithImportedPiecePoints) {
  NewGameBoard<HashCalculator> starting_board;
  MinimaxMoveEvaluator<NewGameBoard<HashCalculator>, PiecePoints>
      red_evaluator{
          PieceColor::kRed,
          standard_search_depth,
          starting_board,
          imported_piece_points
      };
  MinimaxMoveEvaluator<NewGameBoard<HashCalculator>, PiecePoints>
      black_evaluator{
          PieceColor::kBlk,
          standard_search_depth,
          starting_board,
          imported_piece_points
      };
}

TEST_F(MinimaxEvaluatorTest, RedStartingMoveSelection) {
  NewGameBoard<HashCalculator> starting_board;
  MinimaxMoveEvaluator<NewGameBoard<HashCalculator>, PiecePoints>
      red_evaluator{
          PieceColor::kRed,
          standard_search_depth,
          starting_board,
          imported_piece_points
      };

  auto red_selected_move = red_evaluator.SelectMove();
  EXPECT_TRUE(
      (red_selected_move.start == BoardSpace{9, 1} &&
       red_selected_move.end == BoardSpace{7, 2}) ||
      (red_selected_move.start == BoardSpace{9, 7} &&
       red_selected_move.end == BoardSpace{7, 6})
  );
}

TEST_F(MinimaxEvaluatorTest, EndOfGameSelectorTest) {

  MinimaxMoveEvaluator<NewGameBoard<HashCalculator>, PiecePoints>
      black_evaluator{
          PieceColor::kBlk,
          standard_search_depth,
          late_game_board_,
          imported_piece_points
      };

  auto black_selected_move = black_evaluator.SelectMove();
  auto black_executed_move = late_game_board_.ExecuteMove(black_selected_move);
  auto red_possible_moves =
      late_game_board_.CalcFinalMovesOf(PieceColor::kRed);
  auto red_num_possible_moves = red_possible_moves.Size();
  EXPECT_TRUE(red_num_possible_moves == 0);
}

TEST_F(MinimaxEvaluatorTest, PlayGame) {
  NewGameBoard<HashCalculator> game_board;

  int red_search_depth{2};
  MinimaxMoveEvaluator<NewGameBoard<HashCalculator>, PiecePoints>
      red_evaluator{
          PieceColor::kRed,
          red_search_depth,
          game_board,
          imported_piece_points
      };

  int black_search_depth{3};
  MinimaxMoveEvaluator<NewGameBoard<HashCalculator>, PiecePoints>
      black_evaluator{
          PieceColor::kBlk,
          black_search_depth,
          game_board,
          imported_piece_points
      };

  PieceColor losing_player{};

  while (true) {
    auto red_moves = game_board.CalcFinalMovesOf(PieceColor::kRed);
    if (red_moves.Size() == 0) {
      std::cout << "Red has no available moves" << std::endl;
      losing_player = PieceColor::kRed;
      break;
    }
    auto red_selected_move = red_evaluator.SelectMove();
    auto red_executed_move = game_board.ExecuteMove(red_selected_move);

    auto black_moves = game_board.CalcFinalMovesOf(PieceColor::kBlk);
    if (black_moves.Size() == 0) {
      std::cout << "Black has no available moves" << std::endl;
      losing_player = PieceColor::kBlk;
      break;
    }

    auto black_selected_move = black_evaluator.SelectMove();
    auto black_executed_move = game_board.ExecuteMove(black_selected_move);
  }

  EXPECT_TRUE(losing_player == PieceColor::kRed);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}