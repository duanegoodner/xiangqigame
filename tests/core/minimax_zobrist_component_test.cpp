#include <game_board.hpp>
#include <gtest/gtest.h>
#include <move_data_structs.hpp>
#include <move_evaluators.hpp>
#include <piecepoints/piece_position_points.hpp>
#include <zobrist.hpp>

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
  moveselection::RandomMoveEvaluator<GameBoard> red_evaluator{
      PieceColor::kRed,
      starting_board
  };

  std::set<Move, bool (*)(const Move &, const Move &)> move_set(moveComparator);

  auto allowed_moves = starting_board.CalcFinalMovesOf(gameboard::PieceColor::kRed);

  for (auto idx = 0; idx < num_first_move_selections; idx++) {
    auto red_selected_move = red_evaluator.SelectMove(allowed_moves);
    move_set.insert(red_selected_move);
  }
  EXPECT_TRUE(move_set.size() > 5);
}


class MinimaxWithZobristComponentTest : public ::testing::Test {
protected:
  const string points_spec_path =
      utility_functs::get_data_file_abs_path("ICGA_2004_bpo.json");

  BPOPointsSKeys external_pts_spec = BPOPointsSKeys(points_spec_path);

  PiecePositionPoints imported_piece_points = PiecePositionPoints(external_pts_spec);
  GameBoard starting_game_board;
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

  const DepthType standard_search_depth = 4;

  template <typename KeyType, size_t NumConfKeys>
  void PlayGame(DepthType red_search_depth, DepthType black_search_depth) {
    GameBoard game_board;
    moveselection::MinimaxMoveEvaluator<
        GameBoard,
        boardstate::ZobristCoordinator<KeyType, NumConfKeys>,
        PiecePositionPoints>
        red_evaluator{PieceColor::kRed, red_search_depth, game_board};

    moveselection::MinimaxMoveEvaluator<
        GameBoard,
        boardstate::ZobristCoordinator<KeyType, NumConfKeys>,
        PiecePositionPoints>
        black_evaluator{PieceColor::kBlk, black_search_depth, game_board};

    PieceColor losing_player{};

    while (true) {
      auto red_moves = game_board.CalcFinalMovesOf(PieceColor::kRed);
      if (red_moves.Size() == 0) {
        std::cout << "Red has no available moves" << std::endl;
        losing_player = PieceColor::kRed;
        break;
      }
      auto red_selected_move = red_evaluator.SelectMove(red_moves);
      auto red_executed_move = game_board.ExecuteMove(red_selected_move);

      auto black_moves = game_board.CalcFinalMovesOf(PieceColor::kBlk);
      if (black_moves.Size() == 0) {
        std::cout << "Black has no available moves" << std::endl;
        losing_player = PieceColor::kBlk;
        break;
      }

      auto black_selected_move = black_evaluator.SelectMove(black_moves);
      auto black_executed_move = game_board.ExecuteMove(black_selected_move);
    }

    if (red_search_depth < black_search_depth) {
      EXPECT_TRUE(losing_player == PieceColor::kRed);
    }
    if (black_search_depth < red_search_depth) {
      EXPECT_TRUE(losing_player == PieceColor::kBlk);
    }
  }
};

TEST_F(MinimaxWithZobristComponentTest, InitWithPlayerDefaultPiecePositionPoints) {
  moveselection::MinimaxMoveEvaluator<
      GameBoard,
      boardstate::ZobristCoordinator<uint64_t, 1>,
      PiecePositionPoints>
      red_evaluator{PieceColor::kRed, standard_search_depth, starting_game_board};
}

TEST_F(MinimaxWithZobristComponentTest, InitWithImportedPiecePositionPoints) {
  moveselection::MinimaxMoveEvaluator<
      GameBoard,
      boardstate::ZobristCoordinator<uint64_t, 1>,
      PiecePositionPoints>
      red_evaluator{
          PieceColor::kRed,
          standard_search_depth,
          starting_game_board,
          12345,
          imported_piece_points
      };
}

TEST_F(MinimaxWithZobristComponentTest, BoardStateHexStr) {
  moveselection::MinimaxMoveEvaluator<
      GameBoard,
      boardstate::ZobristCoordinator<uint64_t, 1>,
      PiecePositionPoints>
      red_evaluator{PieceColor::kRed, standard_search_depth, starting_game_board};

  std::cout << red_evaluator.board_state_hex_str() << std::endl;
}

TEST_F(MinimaxWithZobristComponentTest, ReadZKeysSeed) {
  moveselection::MinimaxMoveEvaluator<
      GameBoard,
      boardstate::ZobristCoordinator<uint64_t, 1>,
      PiecePositionPoints>
      red_evaluator{PieceColor::kRed, standard_search_depth, starting_game_board};

  std::cout << "zkeys_seed() = " << red_evaluator.zkeys_seed() << std::endl;
}

TEST_F(MinimaxWithZobristComponentTest, InitWithZKeysSeed) {
  moveselection::MinimaxMoveEvaluator<
      GameBoard,
      boardstate::ZobristCoordinator<uint64_t, 1>,
      PiecePositionPoints>
      red_evaluator{PieceColor::kRed, standard_search_depth, starting_game_board, 1234};

  EXPECT_EQ(red_evaluator.zkeys_seed(), 1234);
}

TEST_F(MinimaxWithZobristComponentTest, RedStartingMoveSelection) {
  moveselection::MinimaxMoveEvaluator<
      GameBoard,
      boardstate::ZobristCoordinator<uint64_t, 1>,
      PiecePositionPoints>
      red_evaluator{PieceColor::kRed, standard_search_depth, starting_game_board};

  auto allowed_moves = starting_game_board.CalcFinalMovesOf(PieceColor::kRed);
  auto red_selected_move = red_evaluator.SelectMove(allowed_moves);

  EXPECT_TRUE(
      (red_selected_move.start == BoardSpace{9, 1} &&
       red_selected_move.end == BoardSpace{7, 2}) ||
      (red_selected_move.start == BoardSpace{9, 7} &&
       red_selected_move.end == BoardSpace{7, 6})
  );
}

TEST_F(MinimaxWithZobristComponentTest, PlayGameSingleConfKey) {
  PlayGame<uint64_t, 1>(2, 3);
}

TEST_F(MinimaxWithZobristComponentTest, PlayGameNoConfKey) {
  PlayGame<uint64_t, 0>(2, 3);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}