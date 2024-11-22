#include <game_board_for_concepts.hpp>
#include <gtest/gtest.h>
#include <space_info_provider_new_concept.hpp>
#include <type_traits>

class GameBoardBuilderTest : public ::testing::Test {
protected:
  gameboard::GameBoardBuilder builder_;
};

TEST_F(GameBoardBuilderTest, BuilderReturnsValidSharedPtr) {
  auto game_board = builder_.build();
  ASSERT_NE(game_board, nullptr) << "Builder should not return nullptr";

  // Initial reference count should be 1
  ASSERT_EQ(game_board.use_count(), 1) << "Initial use count should be 1";
}

TEST_F(GameBoardBuilderTest, SharedPtrCopyIncreasesRefCount) {
  auto game_board = builder_.build();
  {
    auto game_board_copy = game_board; // Copy the shared_ptr
    EXPECT_EQ(game_board.use_count(), 2) << "Use count should be 2 after copy";
  }
  // game_board_copy goes out of scope
  EXPECT_EQ(game_board.use_count(), 1)
      << "Use count should return to 1 after copy is destroyed";
}

TEST_F(GameBoardBuilderTest, SharedPtrMoveDoesNotIncreaseRefCount) {
  auto game_board = builder_.build();
  {
    auto game_board_moved = std::move(game_board); // Move the shared_ptr
    EXPECT_EQ(game_board_moved.use_count(), 1) << "Use count should remain 1 after move";
    EXPECT_EQ(game_board, nullptr) << "Original shared_ptr should be null after move";
  }
  EXPECT_EQ(game_board, nullptr)
      << "game_board should still be in scope (with value = null)";
  // game_board_moved goes out of scope, should trigger destruction if no other owners
  // exist
}

TEST_F(GameBoardBuilderTest, CreatesValidGameBoard) {
  auto game_board = builder_.build();

  const auto &actual_board_map = game_board->map();
  auto expected_initial_board =
      gameboard::int_board_to_game_pieces(gameboard::kStandardInitialBoard);
  EXPECT_EQ(
      actual_board_map,
      expected_initial_board
  ) << "When no arguments passed to .build(), .map() should be standard starting board";

  auto move_log_size = game_board->move_log().size();
  EXPECT_EQ(move_log_size, 0) << "Initial move log should be empty";
}

TEST_F(GameBoardBuilderTest, SharedPtrBehavior) {
  std::shared_ptr<GameBoardForConcepts> game_board_outer_scope_ptr;

  {
    auto game_board_inner_scope_ptr = builder_.build();
    auto move_log_obtained_from_inner_scope = game_board_inner_scope_ptr->move_log();

    game_board_outer_scope_ptr = game_board_inner_scope_ptr;
  }
  auto move_log_obtained_from_outer_cope = game_board_outer_scope_ptr->move_log();
}

class GameBoardForConceptsTest : public ::testing::Test {
protected:
  const gameboard::BoardMapInt_t kRepeatMoveTestBoard{{
      {0, 0, 0, 1, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 7, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 5},
      {5, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, -1, 0, 0, 0, 0},
  }};

  const gameboard::BoardMapInt_t kDrawTestBoard{{
      {0, 0, 0, 1, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0},
      {-7, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0},
      {7, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, -1, 0, 0, 0, 0},
  }};

  gameboard::GameBoardBuilder builder_;

  std::shared_ptr<GameBoardForConcepts> starting_board_ = builder_.build();
  std::shared_ptr<GameBoardForConcepts> repeat_move_test_board_ =
      builder_.build(kRepeatMoveTestBoard);
  std::shared_ptr<GameBoardForConcepts> draw_test_board_ = builder_.build(kDrawTestBoard);

};

TEST_F(GameBoardForConceptsTest, SatisfiesSpaceInfoProviderConcept) {
  static_assert(
      SpaceInfoProviderConcept<gameboard::GameBoardForConcepts>,
      "GameBoardForConcepts must satisfy SpaceInfoProviderConcept"
  );
}

TEST_F(GameBoardForConceptsTest, DetectsDraw) {
  // GameBoardForConcepts draw_game_board{kDrawTestBoard};

  for (auto round_trip = 0; round_trip < 10; ++round_trip) {
    for (auto idx = 0; idx < 6; ++idx) {
      BoardSpace start{3, idx};
      BoardSpace end{3, idx + 1};
      Move move{start, end};
      draw_test_board_->ExecuteMove(move);
    }

    for (auto idx = 6; idx > 0; idx--) {
      BoardSpace start{3, idx};
      BoardSpace end{3, idx - 1};
      Move move{start, end};
      draw_test_board_->ExecuteMove(move);
    }
  }
  auto is_draw = draw_test_board_->IsDraw();
  auto red_available_moves = draw_test_board_->CalcFinalMovesOf(PieceColor::kRed);
  std::cout << draw_test_board_->IsDraw() << std::endl;
}

TEST_F(GameBoardForConceptsTest, GetsCorrectOccupants) {
  EXPECT_EQ(starting_board_->GetOccupantAt(BoardSpace{0, 0}), 5);
  EXPECT_EQ(starting_board_->GetOccupantAt(BoardSpace{1, 0}), 0);
  EXPECT_EQ(starting_board_->GetOccupantAt(BoardSpace{9, 7}), -4);
}

TEST_F(GameBoardForConceptsTest, ExecuteAndUndoActualMove) {
  auto actual_move = Move{BoardSpace{6, 2}, BoardSpace{5, 2}};
  auto actual_executed_move = starting_board_->ExecuteMove(actual_move);
  EXPECT_EQ(starting_board_->GetOccupantAt(BoardSpace{6, 2}), 0);
  EXPECT_EQ(starting_board_->GetOccupantAt(BoardSpace{5, 2}), -7);
  starting_board_->UndoMove(actual_executed_move);
  EXPECT_EQ(starting_board_->GetOccupantAt(BoardSpace{6, 2}), -7);
  EXPECT_EQ(starting_board_->GetOccupantAt(BoardSpace{5, 2}), 0);
}

TEST_F(GameBoardForConceptsTest, CorrecNumSpacesOccupiedByBlack) {
  auto black_spaces = starting_board_->GetAllSpacesOccupiedBy(PieceColor::kBlk);
  EXPECT_EQ(black_spaces.size(), 16);
}

TEST_F(GameBoardForConceptsTest, CorrecNumSpacesOccupiedByRed) {
  auto red_spaces = starting_board_->GetAllSpacesOccupiedBy(PieceColor::kRed);
  EXPECT_EQ(red_spaces.size(), 16);
}

TEST_F(GameBoardForConceptsTest, CorrectNumberAvailableMoves) {
  auto black_moves = starting_board_->CalcFinalMovesOf(PieceColor::kBlk);
  auto red_moves = starting_board_->CalcFinalMovesOf(PieceColor::kRed);
}

TEST_F(GameBoardForConceptsTest, ProhibitsTripleRepeatMovePeriod_02) {
  // GameBoardForConcepts late_game_board(kRepeatMoveTestBoard);
  auto red_king_position_a = BoardSpace{9, 4};
  auto red_king_position_b = BoardSpace{9, 3};

  Move move_x = Move{red_king_position_a, red_king_position_b};
  Move move_y = Move{red_king_position_b, red_king_position_a};

  for (int round_trips = 0; round_trips < 2; round_trips++) {
    repeat_move_test_board_->ExecuteMove(move_x);
    auto avail_moves_a = repeat_move_test_board_->CalcFinalMovesOf(PieceColor::kRed);
    EXPECT_TRUE(avail_moves_a.Size() > 0);

    repeat_move_test_board_->ExecuteMove(move_y);
    auto avail_moves_b = repeat_move_test_board_->CalcFinalMovesOf(PieceColor::kRed);
    EXPECT_TRUE(avail_moves_b.Size() > 0);
  }
  repeat_move_test_board_->ExecuteMove(move_x);
  auto avail_moves_c = repeat_move_test_board_->CalcFinalMovesOf(PieceColor::kRed);
  EXPECT_TRUE(avail_moves_c.Size() == 0);
}

TEST_F(GameBoardForConceptsTest, ProhibitsTripleRepeatMovePeriod_03) {
  // GameBoardForConcepts late_game_board(kRepeatMoveTestBoard);
  auto red_king_position_a = BoardSpace{9, 4};
  auto red_king_position_b = BoardSpace{9, 3};
  auto red_king_position_c = BoardSpace{9, 5};

  Move move_w = Move{red_king_position_a, red_king_position_b};
  Move move_x = Move{red_king_position_b, red_king_position_a};
  Move move_y = Move{red_king_position_a, red_king_position_c};
  Move move_z = Move{red_king_position_c, red_king_position_a};

  for (int cycles = 0; cycles < 3; cycles++) {
    repeat_move_test_board_->ExecuteMove(move_w);
    EXPECT_TRUE(repeat_move_test_board_->CalcFinalMovesOf(PieceColor::kRed).Size() > 0);
    repeat_move_test_board_->ExecuteMove(move_x);
    EXPECT_TRUE(repeat_move_test_board_->CalcFinalMovesOf(PieceColor::kRed).Size() > 0);

    if (cycles < 2) {
      repeat_move_test_board_->ExecuteMove(move_y);
      EXPECT_TRUE(repeat_move_test_board_->CalcFinalMovesOf(PieceColor::kRed).Size() > 0);
      repeat_move_test_board_->ExecuteMove(move_z);
      EXPECT_TRUE(repeat_move_test_board_->CalcFinalMovesOf(PieceColor::kRed).Size() > 0);
    }
  }

  repeat_move_test_board_->ExecuteMove(move_y);
  EXPECT_TRUE(repeat_move_test_board_->CalcFinalMovesOf(PieceColor::kRed).Size() == 0);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
