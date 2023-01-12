#include <game_board.hpp>
#include <move_calculator.hpp>
#include <hash_calculator.hpp>

#include <gtest/gtest.h>

class GameBoardTest : public ::testing::Test {
protected:
  GameBoard<HashCalculator> gb_;
};

// TEST_F(GameBoardTest, InitializesBoardState) {
//   EXPECT_NE(gb_.board_state(), 0);
// }

TEST_F(GameBoardTest, GetsCorrectOccupants) {
  EXPECT_EQ(gb_.GetOccupant(BoardSpace{0, 0}), 5);
  EXPECT_EQ(gb_.GetOccupant(BoardSpace{1, 0}), 0);
  EXPECT_EQ(gb_.GetOccupant(BoardSpace{9, 7}), -4);
}

// TEST_F(GameBoardTest, SimpleMove) {
//   auto start_state = gb_.board_state();
//   auto simple_move = Move{BoardSpace{0, 0}, BoardSpace{1, 0}};
//   auto executed_move = gb_.ExecuteMove(simple_move);
//   auto end_state = gb_.board_state();

//   EXPECT_NE(start_state, end_state);
//   EXPECT_EQ(gb_.GetOccupant(BoardSpace{0, 0}), 0);
//   EXPECT_EQ(gb_.GetOccupant(BoardSpace{1, 0}), 5);
// }

TEST_F(GameBoardTest, UndoPretendMove) {
  auto pretend_executed_move =
      ExecutedMove{Move{BoardSpace{8, 0}, BoardSpace{9, 0}}, -5, 0};
  gb_.UndoMove(pretend_executed_move);
  EXPECT_EQ(gb_.GetOccupant(BoardSpace{8, 0}), -5);
  EXPECT_EQ(gb_.GetOccupant(BoardSpace{9, 0}), 0);
}

TEST_F(GameBoardTest, ExecuteAndUndoActualMove) {
  // auto start_state = gb_.board_state();

  auto actual_move = Move{BoardSpace{6, 2}, BoardSpace{5, 2}};
  auto actual_executed_move = gb_.ExecuteMove(actual_move);
  // auto post_move_state = gb_.board_state();
  // EXPECT_NE(start_state, post_move_state);
  EXPECT_EQ(gb_.GetOccupant(BoardSpace{6, 2}), 0);
  EXPECT_EQ(gb_.GetOccupant(BoardSpace{5, 2}), -7);
  gb_.UndoMove(actual_executed_move);
  // auto end_state = gb_.board_state();
  // EXPECT_EQ(start_state, end_state);
  EXPECT_EQ(gb_.GetOccupant(BoardSpace{6, 2}), -7);
  EXPECT_EQ(gb_.GetOccupant(BoardSpace{5, 2}), 0);
}

TEST_F(GameBoardTest, CorrecNumSpacesOccupiedByBlack) {
  auto black_spaces = gb_.GetAllSpacesOccupiedBy(PieceColor::kBlk);
  EXPECT_EQ(black_spaces.size(), 16);
}

TEST_F(GameBoardTest, CorrecNumSpacesOccupiedByRed) {
  auto red_spaces = gb_.GetAllSpacesOccupiedBy(PieceColor::kRed);
  EXPECT_EQ(red_spaces.size(), 16);
}

TEST_F(GameBoardTest, CorrectNumberAvailableMoves) {
  auto black_moves = gb_.CalcFinalMovesOf(PieceColor::kBlk);
  auto red_moves = gb_.CalcFinalMovesOf(PieceColor::kRed);
}

TEST_F(GameBoardTest, AttachHashCalculators) {
  auto red_hash_calculator = HashCalculator();
  auto black_hash_calculator = HashCalculator();
  gb_.ImplementAttachHashCalculator(&red_hash_calculator, 0);
  gb_.ImplementAttachHashCalculator(&black_hash_calculator, 1);
}

TEST_F(GameBoardTest, ExecuteMoveWithAttachedHashCalculators) {
  auto red_hash_calculator = HashCalculator();
  auto black_hash_calculator = HashCalculator();
  gb_.ImplementAttachHashCalculator(&red_hash_calculator, 0);
  gb_.ImplementAttachHashCalculator(&black_hash_calculator, 1);
  auto actual_move = Move{BoardSpace{6, 2}, BoardSpace{5, 2}};
  auto actual_executed_move = gb_.ExecuteMove(actual_move);
  gb_.UndoMove(actual_executed_move);
}



int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}