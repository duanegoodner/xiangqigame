#include <game_board.hpp>
#include <gtest/gtest.h>
#include <move_evaluators.hpp>
#include <piece_position_points.hpp>
#include <utility_functs.hpp>
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

// class MinimaxWithZobristComponentNewTest : public ::testing::Test {
// protected:
//   PiecePositionPoints piece_position_points_{};
//   GameBoard starting_game_board_;

//   boardstate::ZobristCalculator<uint64_t> red_primary_calculator_{};
//   boardstate::ZobristCalculator<uint64_t> black_primary_calculator_{};

//   std::array<boardstate::ZobristCalculator<uint64_t>, 1> red_confirmation_calculators_{
//       boardstate::ZobristCalculator<uint64_t>()
//   };
//   std::array<boardstate::ZobristCalculator<uint64_t>, 1> black_confirmation_calculators_{
//       boardstate::ZobristCalculator<uint64_t>()
//   };

//   boardstate::ZobristComponentNew<uint64_t, 1> red_zobrist_component_{
//       red_primary_calculator_,
//       red_confirmation_calculators_
//   };
//   boardstate::ZobristComponentNew<uint64_t, 1> black_zobrist_component_{
//       black_primary_calculator_,
//       black_confirmation_calculators_
//   };

//   boardstate::TranspositionTable<uint64_t, 1> red_tr_table_{};
//   boardstate::TranspositionTable<uint64_t, 1> black_tr_table_{};

//   boardstate::TranspositionTableGuard red_tr_table_guard_{};
//   boardstate::TranspositionTableGuard black_tr_table_guard_{};

//   boardstate::TranspositionTablePruner<uint64_t, 1> red_tr_table_pruner_{
//       red_tr_table_,
//       red_tr_table_guard_
//   };
//   boardstate::TranspositionTablePruner<uint64_t, 1> black_tr_table_pruner_{
//       black_tr_table_,
//       black_tr_table_guard_
//   };

//   boardstate::ZobristCoordinatorNew<uint64_t, 1> red_zobrist_coordinator_{
//       red_zobrist_component_,
//       red_tr_table_,
//       red_tr_table_guard_,
//       red_tr_table_pruner_
//   };
//   boardstate::ZobristCoordinatorNew<uint64_t, 1> black_zobrist_coordinator_{
//       black_zobrist_component_,
//       black_tr_table_,
//       black_tr_table_guard_,
//       black_tr_table_pruner_
//   };


//   moveselection::PreSearchMoveSorter<GameBoard, PiecePositionPoints> red_move_sorter_{
//       starting_game_board_,
//       piece_position_points_
//   };
//   moveselection::PreSearchMoveSorter<GameBoard, PiecePositionPoints> black_move_sorter_{
//       starting_game_board_,
//       piece_position_points_
//   };


//   DepthType default_search_depth_{4};

//   const BoardMapInt_t kLateGameBoardMap{{
//       {0, 0, 0, 1, 0, 0, 0, 0, 0},
//       {0, 0, 0, 0, 0, 0, 0, 0, 0},
//       {0, 0, 0, 0, 0, 0, 0, 0, 0},
//       {0, 0, 0, 0, 0, 0, 0, 0, 0},
//       {0, 0, 0, 0, 0, 0, 0, 0, 0},
//       {0, 0, 0, 0, 0, 0, 0, 0, 0},
//       {0, 0, 0, 0, 0, 0, 0, 0, 0},
//       {0, 0, 0, 0, 0, 0, 0, 0, 5},
//       {5, 0, 0, 0, 0, 0, 0, 0, 0},
//       {0, 0, 0, 0, -1, 0, 0, 0, 0},
//   }};
//   GameBoard late_game_board_{kLateGameBoardMap};

//   const DepthType standard_search_depth = 4;

//   template <typename KeyType, size_t NumConfKeys>
//   void PlayGame(DepthType red_search_depth, DepthType black_search_depth) {
//     GameBoard game_board;
//     moveselection::MinimaxMoveEvaluatorNew<
//         GameBoard,
//         boardstate::ZobristCoordinatorNew<uint64_t, 1>,
//         PiecePositionPoints>
//         red_evaluator{
//             PieceColor::kRed,
//             red_search_depth,
//             game_board,
//             piece_position_points_,
//             red_zobrist_coordinator_,
//             red_move_sorter_
//         };

//     moveselection::MinimaxMoveEvaluatorNew<
//         GameBoard,
//         boardstate::ZobristCoordinatorNew<uint64_t, 1>,
//         PiecePositionPoints>
//         black_evaluator{
//             PieceColor::kBlk,
//             black_search_depth,
//             game_board,
//             piece_position_points_,
//             black_zobrist_coordinator_,
//             black_move_sorter_
//         };

//     PieceColor losing_player{};

//     while (true) {
//       auto red_moves = game_board.CalcFinalMovesOf(PieceColor::kRed);
//       if (red_moves.Size() == 0) {
//         std::cout << "Red has no available moves" << std::endl;
//         losing_player = PieceColor::kRed;
//         break;
//       }
//       auto red_selected_move = red_evaluator.SelectMove(red_moves);
//       auto red_executed_move = game_board.ExecuteMove(red_selected_move);

//       auto black_moves = game_board.CalcFinalMovesOf(PieceColor::kBlk);
//       if (black_moves.Size() == 0) {
//         std::cout << "Black has no available moves" << std::endl;
//         losing_player = PieceColor::kBlk;
//         break;
//       }

//       auto black_selected_move = black_evaluator.SelectMove(black_moves);
//       auto black_executed_move = game_board.ExecuteMove(black_selected_move);
//     }

//     if (red_search_depth < black_search_depth) {
//       EXPECT_TRUE(losing_player == PieceColor::kRed);
//     }
//     if (black_search_depth < red_search_depth) {
//       EXPECT_TRUE(losing_player == PieceColor::kBlk);
//     }
//   }
// };

// TEST_F(MinimaxWithZobristComponentNewTest, Init) {
//   moveselection::MinimaxMoveEvaluatorNew<
//       GameBoard,
//       boardstate::ZobristCoordinatorNew<uint64_t, 1>,
//       PiecePositionPoints>
//       red_evaluator{
//           PieceColor::kRed,
//           default_search_depth_,
//           starting_game_board_,
//           piece_position_points_,
//           red_zobrist_coordinator_,
//           red_move_sorter_
//       };
// }

// TEST_F(MinimaxWithZobristComponentNewTest, BoardStateHexStr) {
//   moveselection::MinimaxMoveEvaluatorNew<
//       GameBoard,
//       boardstate::ZobristCoordinatorNew<uint64_t, 1>,
//       PiecePositionPoints>
//       red_evaluator{
//           PieceColor::kRed,
//           default_search_depth_,
//           starting_game_board_,
//           piece_position_points_,
//           black_zobrist_coordinator_,
//           black_move_sorter_
//       };
//   std::cout << red_evaluator.board_state_hex_str() << std::endl;
// }

// TEST_F(MinimaxWithZobristComponentNewTest, RedStartingMoveSelection) {
//   moveselection::MinimaxMoveEvaluatorNew<
//       GameBoard,
//       boardstate::ZobristCoordinatorNew<uint64_t, 1>,
//       PiecePositionPoints>
//       red_evaluator{
//           PieceColor::kRed,
//           default_search_depth_,
//           starting_game_board_,
//           piece_position_points_,
//           red_zobrist_coordinator_,
//           red_move_sorter_
//       };

//   auto allowed_moves = starting_game_board_.CalcFinalMovesOf(PieceColor::kRed);
//   auto red_selected_move = red_evaluator.SelectMove(allowed_moves);

//   EXPECT_TRUE(
//       (red_selected_move.start == BoardSpace{9, 1} &&
//        red_selected_move.end == BoardSpace{7, 2}) ||
//       (red_selected_move.start == BoardSpace{9, 7} &&
//        red_selected_move.end == BoardSpace{7, 6})
//   );
// }

// TEST_F(MinimaxWithZobristComponentNewTest, PlayGameSingleConfKey) {
//   PlayGame<uint64_t, 1>(2, 3);
// }

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