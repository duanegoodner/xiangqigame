#include <builders.hpp>
#include <game_board_for_concepts.hpp>
#include <gtest/gtest.h>
#include <memory>
// #include <shared_ptr_builder_tests.hpp>
#include <space_info_provider_new_concept.hpp>
#include <type_traits>
#include <zobrist_calculator_for_concepts.hpp>

//! @todo Create generic share_ptr test suite that can handle GameBoardForconcepts<RC,
//! BC>
// Run our standard test suite for a Builder that has .build() -> shared_ptr
// using GameBoardBuilderTestTypes = ::testing::Types<
//     BuilderObjectPair<gameboard::GameBoardBuilder, gameboard::GameBoardForConcepts>>;
// INSTANTIATE_TYPED_TEST_SUITE_P(
//     GameBoardBuilderTestInstance,
//     SharedPtrBuilderTest,
//     GameBoardBuilderTestTypes
// );

//! Tests specific to a GameBoardBuilder (beyone those in the generic Builder suite)
class GameBoardBuilderTest : public ::testing::Test {
protected:
  gameboard::GameBoardBuilder<
      boardstate::ZobristCalculatorForConcepts<uint64_t>,
      boardstate::ZobristCalculatorForConcepts<uint64_t>>
      builder_;

  std::shared_ptr<boardstate::ZobristCalculatorForConcepts<uint64_t>> zcalc_r1_ =
      std::make_shared<boardstate::ZobristCalculatorForConcepts<uint64_t>>();
  std::shared_ptr<boardstate::ZobristCalculatorForConcepts<uint64_t>> zcalc_r2_ =
      std::make_shared<boardstate::ZobristCalculatorForConcepts<uint64_t>>();
  std::shared_ptr<boardstate::ZobristCalculatorForConcepts<uint64_t>> zcalc_b1_ =
      std::make_shared<boardstate::ZobristCalculatorForConcepts<uint64_t>>();
  std::shared_ptr<boardstate::ZobristCalculatorForConcepts<uint64_t>> zcalc_b2_ =
      std::make_shared<boardstate::ZobristCalculatorForConcepts<uint64_t>>();

  std::vector<std::shared_ptr<boardstate::ZobristCalculatorForConcepts<uint64_t>>>
      red_z_calculators_{zcalc_r1_, zcalc_r2_};
  std::vector<std::shared_ptr<boardstate::ZobristCalculatorForConcepts<uint64_t>>>
      black_z_calculators_{zcalc_b1_, zcalc_b2_};
};

TEST_F(GameBoardBuilderTest, CreatesValidGameBoard) {

  auto game_board = builder_.build(red_z_calculators_, black_z_calculators_);

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
  std::shared_ptr<gameboard::GameBoardForConcepts<
      boardstate::ZobristCalculatorForConcepts<uint64_t>,
      boardstate::ZobristCalculatorForConcepts<uint64_t>>>
      game_board_outer_scope_ptr;

  {
    auto game_board_inner_scope_ptr =
        builder_.build(red_z_calculators_, black_z_calculators_);
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

  gameboard::GameBoardBuilder<
      boardstate::ZobristCalculatorForConcepts<uint64_t>,
      boardstate::ZobristCalculatorForConcepts<uint64_t>>
      builder_;

  std::shared_ptr<boardstate::ZobristCalculatorForConcepts<uint64_t>> zcalc_r1_ =
      std::make_shared<boardstate::ZobristCalculatorForConcepts<uint64_t>>();
  std::shared_ptr<boardstate::ZobristCalculatorForConcepts<uint64_t>> zcalc_r2_ =
      std::make_shared<boardstate::ZobristCalculatorForConcepts<uint64_t>>();
  std::shared_ptr<boardstate::ZobristCalculatorForConcepts<uint64_t>> zcalc_b1_ =
      std::make_shared<boardstate::ZobristCalculatorForConcepts<uint64_t>>();
  std::shared_ptr<boardstate::ZobristCalculatorForConcepts<uint64_t>> zcalc_b2_ =
      std::make_shared<boardstate::ZobristCalculatorForConcepts<uint64_t>>();

  std::vector<std::shared_ptr<boardstate::ZobristCalculatorForConcepts<uint64_t>>>
      red_z_calculators_{zcalc_r1_, zcalc_r2_};
  std::vector<std::shared_ptr<boardstate::ZobristCalculatorForConcepts<uint64_t>>>
      black_z_calculators_{zcalc_b1_, zcalc_b2_};

  std::shared_ptr<gameboard::GameBoardForConcepts<
      boardstate::ZobristCalculatorForConcepts<uint64_t>,
      boardstate::ZobristCalculatorForConcepts<uint64_t>>>
      starting_board_ = builder_.build(red_z_calculators_, black_z_calculators_);

  std::shared_ptr<gameboard::GameBoardForConcepts<
      boardstate::ZobristCalculatorForConcepts<uint64_t>,
      boardstate::ZobristCalculatorForConcepts<uint64_t>>>
      draw_test_board_ =
          builder_.build(red_z_calculators_, black_z_calculators_, kDrawTestBoard);

  std::shared_ptr<gameboard::GameBoardForConcepts<
      boardstate::ZobristCalculatorForConcepts<uint64_t>,
      boardstate::ZobristCalculatorForConcepts<uint64_t>>>
      repeat_move_test_board_ =
          builder_.build(red_z_calculators_, black_z_calculators_, kRepeatMoveTestBoard);
};

TEST_F(GameBoardForConceptsTest, SatisfiesSpaceInfoProviderConcept) {
  static_assert(
      SpaceInfoProviderConcept<gameboard::GameBoardForConcepts<
      boardstate::ZobristCalculatorForConcepts<uint64_t>,
      boardstate::ZobristCalculatorForConcepts<uint64_t>>>,
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
      EXPECT_TRUE(
          repeat_move_test_board_->CalcFinalMovesOf(PieceColor::kRed).Size() > 0
      );
      repeat_move_test_board_->ExecuteMove(move_z);
      EXPECT_TRUE(
          repeat_move_test_board_->CalcFinalMovesOf(PieceColor::kRed).Size() > 0
      );
    }
  }

  repeat_move_test_board_->ExecuteMove(move_y);
  EXPECT_TRUE(repeat_move_test_board_->CalcFinalMovesOf(PieceColor::kRed).Size() == 0);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
