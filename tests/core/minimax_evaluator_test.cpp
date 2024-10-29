#include <game_board.hpp>
#include <gtest/gtest.h>
#include <hash_calculator.hpp>
#include <move_evaluators.hpp>
#include <piece_position_points.hpp>
#include <utility_functs.hpp>

using namespace boardstate;
using namespace piecepoints;

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

  GameBoard starting_board;
  RandomMoveEvaluator<GameBoard> red_evaluator{PieceColor::kRed, starting_board};

  std::set<Move, bool (*)(const Move &, const Move &)> move_set(moveComparator);

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

  BPOPointsSKeys external_pts_spec =
      BPOPointsSKeys(points_spec_path);
  
  PiecePositionPoints imported_piece_points = PiecePositionPoints(external_pts_spec);
  
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
  GameBoard late_game_board_{kLateGameBoardMap};

  const int standard_search_depth = 4;
};

TEST_F(MinimaxEvaluatorTest, TestConstructorsWithDefaultPiecePositionPoints) {
  GameBoard starting_board;
  MinimaxMoveEvaluator<GameBoard, SingleZobristTracker<uint64_t>, PiecePositionPoints>
      red_evaluator{PieceColor::kRed, standard_search_depth, starting_board};
  MinimaxMoveEvaluator<GameBoard, SingleZobristTracker<uint64_t>, PiecePositionPoints>
      black_evaluator{PieceColor::kBlk, standard_search_depth, starting_board};
}

TEST_F(MinimaxEvaluatorTest, ConstructorWithZKeySeedSpecified) {
  GameBoard starting_board;
  MinimaxMoveEvaluator<GameBoard, SingleZobristTracker<uint64_t>, PiecePositionPoints>
      red_evaluator{PieceColor::kRed, standard_search_depth, starting_board, 12345};
  EXPECT_EQ(red_evaluator.zkeys_seed(), 12345);
}

TEST_F(MinimaxEvaluatorTest, TestGetStateHexString) {
  GameBoard starting_board;
  MinimaxMoveEvaluator<GameBoard, SingleZobristTracker<uint32_t>, PiecePositionPoints>
      red_evaluator_032{PieceColor::kRed, standard_search_depth, starting_board};
  std::cout << red_evaluator_032.board_state_hex_str() << std::endl;


  MinimaxMoveEvaluator<GameBoard, SingleZobristTracker<uint64_t>, PiecePositionPoints>
      red_evaluator_064{PieceColor::kRed, standard_search_depth, starting_board};
  std::cout << red_evaluator_064.board_state_hex_str() << std::endl;

  MinimaxMoveEvaluator<GameBoard, SingleZobristTracker<__uint128_t>, PiecePositionPoints>
      red_evaluator_128{PieceColor::kRed, standard_search_depth, starting_board};
  std::cout << red_evaluator_128.board_state_hex_str() << std::endl;
}

TEST_F(MinimaxEvaluatorTest, TestConstructorsWithImportedPiecePositionPoints) {
  GameBoard starting_board;
  MinimaxMoveEvaluator<GameBoard, SingleZobristTracker<uint64_t>, PiecePositionPoints>
      red_evaluator{
          PieceColor::kRed,
          standard_search_depth,
          starting_board,
          imported_piece_points
      };
  MinimaxMoveEvaluator<GameBoard, SingleZobristTracker<uint64_t>, PiecePositionPoints>
      black_evaluator{
          PieceColor::kBlk,
          standard_search_depth,
          starting_board,
          imported_piece_points
      };
}

TEST_F(MinimaxEvaluatorTest, RedStartingMoveSelection) {
  GameBoard starting_board;
  MinimaxMoveEvaluator<GameBoard, SingleZobristTracker<uint64_t>, PiecePositionPoints>
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

TEST_F(MinimaxEvaluatorTest, RedStartingMoveSelection128) {
  GameBoard starting_board;
  MinimaxMoveEvaluator<GameBoard, SingleZobristTracker<__uint128_t>, PiecePositionPoints>
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

TEST_F(MinimaxEvaluatorTest, GetSearchSummaries) {
  int shallow_search_depth{2};
  GameBoard starting_board;
  MinimaxMoveEvaluator<GameBoard, SingleZobristTracker<uint64_t>, PiecePositionPoints>
      red_evaluator{
          PieceColor::kRed,
          shallow_search_depth,
          starting_board,
          imported_piece_points
      };
  auto red_selected_move = red_evaluator.SelectMove();
  auto search_summaries = red_evaluator.search_summaries();
  auto num_nodes_visited = search_summaries.first_searches[0].num_nodes();
  EXPECT_TRUE(num_nodes_visited > 0);
}

TEST_F(MinimaxEvaluatorTest, EndOfGameSelectorTest) {

  MinimaxMoveEvaluator<GameBoard, SingleZobristTracker<uint64_t>, PiecePositionPoints>
      black_evaluator{
          PieceColor::kBlk,
          standard_search_depth,
          late_game_board_,
          imported_piece_points
      };

  auto black_selected_move = black_evaluator.SelectMove();
  auto black_executed_move = late_game_board_.ExecuteMove(black_selected_move);
  auto red_possible_moves = late_game_board_.CalcFinalMovesOf(PieceColor::kRed);
  auto red_num_possible_moves = red_possible_moves.Size();
  EXPECT_TRUE(red_num_possible_moves == 0);
}

TEST_F(MinimaxEvaluatorTest, PlayGame) {
  GameBoard game_board;

  int red_search_depth{2};
  MinimaxMoveEvaluator<GameBoard, SingleZobristTracker<uint64_t>, PiecePositionPoints>
      red_evaluator{
          PieceColor::kRed,
          red_search_depth,
          game_board,
          imported_piece_points
      };

  int black_search_depth{3};
  MinimaxMoveEvaluator<GameBoard, SingleZobristTracker<uint64_t>, PiecePositionPoints>
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
