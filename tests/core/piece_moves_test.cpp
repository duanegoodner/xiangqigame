#include <board_utilities.hpp>
#include <piece_moves.hpp>

#include <gtest/gtest.h>

class PieceMovesTest : public ::testing::Test {};

TEST_F(PieceMovesTest, SoldierMoves) {
  const BoardMapInt_t kSoldierMoveTestBoard{{
      {0, 0, 0, 1, 0, 0, -7, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, -7, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 5},
      {5, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, -1, 0, 0, 0, 0},
  }};

  auto board_map = int_board_to_game_pieces(kSoldierMoveTestBoard);
  moves::MoveCollection red_soldier_move_collection{};
  moves::PieceMoves piece_moves{};
  BoardSpace test_player_space_a{2, 5};
  BoardSpace test_player_space_b{0, 6};

  piece_moves.SoldierMoves(
      board_map,
      PieceColor::kRed,
      test_player_space_a,
      red_soldier_move_collection
  );

  piece_moves.SoldierMoves(
      board_map,
      PieceColor::kRed,
      test_player_space_b,
      red_soldier_move_collection
  );

  EXPECT_EQ(red_soldier_move_collection.Size(), 5);
}