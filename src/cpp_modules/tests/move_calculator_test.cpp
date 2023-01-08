#include "common.hpp"
#include <board_components.hpp>
#include <board_utilities.hpp>
#include <game_board.hpp>
#include <gtest/gtest.h>
#include <move_calculator.hpp>

using namespace board_utilities;

const BoardMapInt_t kFlyingBoard{{
    {5, 4, 3, 2, 1, 2, 3, 4, 5},
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 6, 0, 0, 0, 0, 0, 6, 0},
    {7, 0, 7, 0, 0, 0, 7, 0, 7},
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {-7, 0, -7, 0, 0, 0, -7, 0, -7},
    {0, -6, 0, 0, 0, 0, 0, -6, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {-5, -4, -3, -2, -1, -2, -3, -4, -5},
}};

class PieceMovesTest : public ::testing::Test {
protected:
  PieceMoves piece_moves_;
  BoardMap_t starting_board = int_board_to_game_pieces(kStartingBoard);
  BoardMap_t flying_board = int_board_to_game_pieces(kFlyingBoard);
};

TEST_F(PieceMovesTest, RedSoldier) {
  MoveCollection soldier_moves;
  piece_moves_.SoldierMoves(
      starting_board,
      PieceColor::kRed,
      BoardSpace{6, 0},
      soldier_moves
  );
  EXPECT_EQ(soldier_moves.moves.size(), 1);
}

TEST_F(PieceMovesTest, BlackCannon) {
  MoveCollection cannon_moves;
  piece_moves_.CannonMoves(
      starting_board,
      PieceColor::kBlk,
      BoardSpace{2, 1},
      cannon_moves
  );
  EXPECT_EQ(cannon_moves.moves.size(), 12);
}

TEST_F(PieceMovesTest, BlackChariot) {
  MoveCollection chariot_moves;
  piece_moves_.ChariotMoves(
      starting_board,
      PieceColor::kBlk,
      BoardSpace{0, 8},
      chariot_moves
  );
  EXPECT_EQ(chariot_moves.moves.size(), 2);
}

TEST_F(PieceMovesTest, RedHorse) {
  MoveCollection horse_moves;
  piece_moves_.HorseMoves(
      starting_board,
      PieceColor::kRed,
      BoardSpace{9, 1},
      horse_moves
  );
  EXPECT_EQ(horse_moves.moves.size(), 2);
}

TEST_F(PieceMovesTest, BlackElephant) {
  MoveCollection elephant_moves;
  piece_moves_.ElephantMoves(
      starting_board,
      PieceColor::kBlk,
      BoardSpace{0, 2},
      elephant_moves
  );
  EXPECT_EQ(elephant_moves.moves.size(), 2);
}

TEST_F(PieceMovesTest, RedAdvisor) {
  MoveCollection advisor_moves;
  piece_moves_.AdvisorMoves(
      starting_board,
      PieceColor::kRed,
      BoardSpace{9, 3},
      advisor_moves
  );
  EXPECT_EQ(advisor_moves.moves.size(), 1);
}

TEST_F(PieceMovesTest, BlackStartingGeneral) {
  MoveCollection general_moves;
  piece_moves_.GeneralMoves(
      starting_board,
      PieceColor::kBlk,
      BoardSpace{0, 4},
      general_moves
  );
  EXPECT_EQ(general_moves.moves.size(), 1);
}

TEST_F(PieceMovesTest, RedFlyingGeneral) {
  MoveCollection general_moves_with_flying;
  piece_moves_.GeneralMoves(
      flying_board,
      PieceColor::kRed,
      BoardSpace{9, 4},
      general_moves_with_flying
  );
  EXPECT_EQ(general_moves_with_flying.moves.size(), 2);
}

class MoveCalculatorTest : public ::testing::Test {

protected:
  BoardMap_t starting_board = int_board_to_game_pieces(kStartingBoard);
  // BoardMap_t flying_board = int_board_to_game_pieces(kFlyingBoard);
  MoveCalculator mc_standard_;
  MoveCalculator mc_flying_;
  MoveCalculatorTest()
      : mc_standard_{MoveCalculator()}
      , mc_flying_{MoveCalculator()} {}
};

TEST_F(MoveCalculatorTest, AllMovesNoCheckTest) {
  auto red_moves_without_check_test =
      mc_standard_.CalcAllMovesNoCheckTest(PieceColor::kRed, starting_board);
  EXPECT_EQ(red_moves_without_check_test.moves.size(), 44);
}

class BoardUtilitiesTest : public ::testing::Test {

protected:
  BoardMap_t starting_board = int_board_to_game_pieces(kStartingBoard);
  MoveCalculator mc_standard_;
  BoardUtilitiesTest()
      : mc_standard_{MoveCalculator()} {}
};

TEST_F(BoardUtilitiesTest, IsOccupiedTrue) {
  auto result = is_occupied(starting_board, BoardSpace{0, 0});
  EXPECT_EQ(result, true);
}

TEST_F(BoardUtilitiesTest, IsOccupiedFalse) {
  auto result = is_occupied(starting_board, BoardSpace{1, 1});
  EXPECT_EQ(result, false);
}

TEST_F(BoardUtilitiesTest, GetColorBlack) {
  auto result = get_color(starting_board, BoardSpace{0, 0});
  EXPECT_EQ(result, PieceColor::kBlk);
}

TEST_F(BoardUtilitiesTest, GetColorEmpty) {
  auto result = get_color(starting_board, BoardSpace{1, 0});
  EXPECT_EQ(result, PieceColor::kNul);
}

TEST_F(BoardUtilitiesTest, GetTypeElephant) {
  auto result = get_type(starting_board, BoardSpace{9, 2});
  EXPECT_EQ(result, PieceType::kEle);
}

TEST_F(BoardUtilitiesTest, GetTypeEmpty) {
  auto result = get_type(starting_board, BoardSpace{5, 1});
  EXPECT_EQ(result, PieceType::kNnn);
}

TEST_F(BoardUtilitiesTest, ExistsAndPassesColorTestTrue) {
  auto result = exists_and_passes_color_test(
      starting_board,
      BoardSpace{3, 2},
      PieceColor::kRed
  );
  EXPECT_EQ(result, true);
}

TEST_F(BoardUtilitiesTest, ExistsAndPassesColorTestFalseOffBoard) {
  auto result = exists_and_passes_color_test(
      starting_board,
      BoardSpace{100, 1},
      PieceColor::kRed
  );
  EXPECT_EQ(result, false);
}

TEST_F(BoardUtilitiesTest, ExistsAndPassesColorTestFalseSameColor) {
  auto result = exists_and_passes_color_test(
      starting_board,
      BoardSpace{6, 0},
      PieceColor::kRed
  );
  EXPECT_EQ(result, false);
}

TEST_F(BoardUtilitiesTest, FwdDirectionRed) {
  auto result = fwd_direction(PieceColor::kRed);
  EXPECT_EQ(result.rank, -1);
  EXPECT_EQ(result.file, 0);
}

TEST_F(BoardUtilitiesTest, RevDirectionRed) {
  auto result = rev_direction(PieceColor::kRed);
  EXPECT_EQ(result.rank, 1);
  EXPECT_EQ(result.file, 0);
}

TEST_F(BoardUtilitiesTest, GetGeneralPositionBlack) {
  auto result = get_general_position(starting_board, PieceColor::kBlk);
  EXPECT_EQ(result.rank, 0);
  EXPECT_EQ(result.file, 4);
}

TEST_F(BoardUtilitiesTest, GetAllSpacesOccupiedByBlack) {
  auto result = get_all_spaces_occupied_by(starting_board, PieceColor::kBlk);
  EXPECT_EQ(result.size(), 16);
}

TEST_F(BoardUtilitiesTest, IsSpaceDestinationOfAnyMovesTrue) {
  auto some_moves = MoveCollection({Move{BoardSpace{2, 1}, BoardSpace{9, 1}}});
  auto some_space = BoardSpace{9, 1};
  auto result = is_space_any_destination_of_moves(some_space, some_moves);
  EXPECT_EQ(result, true);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
