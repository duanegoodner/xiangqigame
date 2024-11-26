#include <builders.hpp>
#include <game_board_for_concepts.hpp>
#include <gtest/gtest.h>
#include <memory>
// #include <shared_ptr_builder_tests.hpp>
#include <concept_space_info_provider.hpp>
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

template <BoardStateCalculatorConcept RC, BoardStateCalculatorConcept BC>
std::shared_ptr<gameboard::GameBoardForConcepts<RC, BC>> BuildGameBoard(
    const BoardMapInt_t starting_board = kStandardInitialBoard
) {
  gameboard::GameBoardBuilder<RC, BC> builder;

  auto zcalc_r1 = std::make_shared<RC>();
  auto zcalc_r2 = std::make_shared<RC>();
  auto zcalc_b1 = std::make_shared<BC>();
  auto zcalc_b2 = std::make_shared<BC>();

  std::vector<std::shared_ptr<RC>> red_z_calculators{zcalc_r1, zcalc_r2};
  std::vector<std::shared_ptr<BC>> black_z_calculators{zcalc_b1, zcalc_b2};

  return builder.build(red_z_calculators, black_z_calculators, starting_board);
}

//! Tests specific to a GameBoardBuilder (beyone those in the generic Builder suite)
class GameBoardBuilderTest : public ::testing::Test {
protected:
  gameboard::GameBoardBuilder<
      boardstate::ZobristCalculatorForConcepts<uint64_t>,
      boardstate::ZobristCalculatorForConcepts<uint64_t>>
      builder_;

  template <BoardStateCalculatorConcept RC, BoardStateCalculatorConcept BC>
  void TestGameBoardCreation() {
    auto game_board = BuildGameBoard<RC, BC>();
    const auto &actual_board_map = game_board->map();
    auto expected_initial_board =
        gameboard::int_board_to_game_pieces(gameboard::kStandardInitialBoard);
    EXPECT_EQ(actual_board_map, expected_initial_board)
        << "When no arguments passed to .build(), .map() should be standard starting "
           "board";

    auto move_log_size = game_board->move_log().size();
    EXPECT_EQ(move_log_size, 0) << "Initial move log should be empty";
  }

  template <BoardStateCalculatorConcept RC, BoardStateCalculatorConcept BC>
  void TestSharedPtrBehavior() {
    std::shared_ptr<gameboard::GameBoardForConcepts<RC, BC>> game_board_outer_scope_ptr;
    {
      auto game_board_inner_scope_ptr = BuildGameBoard<RC, BC>();
      auto move_log_obtained_from_inner_scope = game_board_inner_scope_ptr->move_log();
      game_board_outer_scope_ptr = game_board_inner_scope_ptr;
    }
    auto move_log_obtained_from_outer_cope = game_board_outer_scope_ptr->move_log();
  }
};

TEST_F(GameBoardBuilderTest, CreatesValidGameBoard) {
  TestGameBoardCreation<
      boardstate::ZobristCalculatorForConcepts<uint64_t>,
      boardstate::ZobristCalculatorForConcepts<uint64_t>>();
}

TEST_F(GameBoardBuilderTest, SharedPtrBehavior) {
  TestSharedPtrBehavior<
      boardstate::ZobristCalculatorForConcepts<uint64_t>,
      boardstate::ZobristCalculatorForConcepts<uint64_t>>();
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

  template <BoardStateCalculatorConcept RC, BoardStateCalculatorConcept BC>
  void TestDrawDetection() {
    auto draw_test_board = BuildGameBoard<RC, BC>(kDrawTestBoard);

    for (auto round_trip = 0; round_trip < 10; ++round_trip) {
      for (auto idx = 0; idx < 6; ++idx) {
        BoardSpace start{3, idx};
        BoardSpace end{3, idx + 1};
        Move move{start, end};
        draw_test_board->ExecuteMove(move);
      }

      for (auto idx = 6; idx > 0; idx--) {
        BoardSpace start{3, idx};
        BoardSpace end{3, idx - 1};
        Move move{start, end};
        draw_test_board->ExecuteMove(move);
      }
    }
    auto is_draw = draw_test_board->IsDraw();
    auto red_available_moves = draw_test_board->CalcFinalMovesOf(PieceColor::kRed);
    std::cout << draw_test_board->IsDraw() << std::endl;
  }

  template <BoardStateCalculatorConcept RC, BoardStateCalculatorConcept BC>
  void TestGetsCorrectOccupants() {
    auto starting_board = BuildGameBoard<RC, BC>();
    EXPECT_EQ(starting_board->GetOccupantAt(BoardSpace{0, 0}), 5);
    EXPECT_EQ(starting_board->GetOccupantAt(BoardSpace{1, 0}), 0);
    EXPECT_EQ(starting_board->GetOccupantAt(BoardSpace{9, 7}), -4);
  }

  template <BoardStateCalculatorConcept RC, BoardStateCalculatorConcept BC>
  void TestExecuteAndUndoActualMove() {
    auto starting_board = BuildGameBoard<RC, BC>();
    auto actual_move = Move{BoardSpace{6, 2}, BoardSpace{5, 2}};
    auto actual_executed_move = starting_board->ExecuteMove(actual_move);
    EXPECT_EQ(starting_board->GetOccupantAt(BoardSpace{6, 2}), 0);
    EXPECT_EQ(starting_board->GetOccupantAt(BoardSpace{5, 2}), -7);
    starting_board->UndoMove(actual_executed_move);
    EXPECT_EQ(starting_board->GetOccupantAt(BoardSpace{6, 2}), -7);
    EXPECT_EQ(starting_board->GetOccupantAt(BoardSpace{5, 2}), 0);
  }

  template <BoardStateCalculatorConcept RC, BoardStateCalculatorConcept BC>
  void TestCorrecNumSpacesOccupiedBy(PieceColor color) {
    auto starting_board = BuildGameBoard<RC, BC>();
    auto player_spaces = starting_board->GetAllSpacesOccupiedBy(color);
    EXPECT_EQ(player_spaces.size(), 16);
  }

  template <BoardStateCalculatorConcept RC, BoardStateCalculatorConcept BC>
  void TestCorrectNumberAvailableMoves() {
    auto starting_board = BuildGameBoard<RC, BC>();
    auto black_moves = starting_board->CalcFinalMovesOf(PieceColor::kBlk);
    auto red_moves = starting_board->CalcFinalMovesOf(PieceColor::kRed);
  }

  template <BoardStateCalculatorConcept RC, BoardStateCalculatorConcept BC>
  void TestProhibitsTripleRepeatMovePeriod_02() {
    auto repeat_move_test_board = BuildGameBoard<RC, BC>(kRepeatMoveTestBoard);

    auto red_king_position_a = BoardSpace{9, 4};
    auto red_king_position_b = BoardSpace{9, 3};

    Move move_x = Move{red_king_position_a, red_king_position_b};
    Move move_y = Move{red_king_position_b, red_king_position_a};

    for (int round_trips = 0; round_trips < 2; round_trips++) {
      repeat_move_test_board->ExecuteMove(move_x);
      auto avail_moves_a = repeat_move_test_board->CalcFinalMovesOf(PieceColor::kRed);
      EXPECT_TRUE(avail_moves_a.Size() > 0);

      repeat_move_test_board->ExecuteMove(move_y);
      auto avail_moves_b = repeat_move_test_board->CalcFinalMovesOf(PieceColor::kRed);
      EXPECT_TRUE(avail_moves_b.Size() > 0);
    }
    repeat_move_test_board->ExecuteMove(move_x);
    auto avail_moves_c = repeat_move_test_board->CalcFinalMovesOf(PieceColor::kRed);
    EXPECT_TRUE(avail_moves_c.Size() == 0);
  }

  template <BoardStateCalculatorConcept RC, BoardStateCalculatorConcept BC>
  void TestProhibitsTripleRepeatMovePeriod_03() {
    auto repeat_move_test_board = BuildGameBoard<RC, BC>(kRepeatMoveTestBoard);

    auto red_king_position_a = BoardSpace{9, 4};
    auto red_king_position_b = BoardSpace{9, 3};
    auto red_king_position_c = BoardSpace{9, 5};

    Move move_w = Move{red_king_position_a, red_king_position_b};
    Move move_x = Move{red_king_position_b, red_king_position_a};
    Move move_y = Move{red_king_position_a, red_king_position_c};
    Move move_z = Move{red_king_position_c, red_king_position_a};

    for (int cycles = 0; cycles < 3; cycles++) {
      repeat_move_test_board->ExecuteMove(move_w);
      EXPECT_TRUE(repeat_move_test_board->CalcFinalMovesOf(PieceColor::kRed).Size() > 0);
      repeat_move_test_board->ExecuteMove(move_x);
      EXPECT_TRUE(repeat_move_test_board->CalcFinalMovesOf(PieceColor::kRed).Size() > 0);

      if (cycles < 2) {
        repeat_move_test_board->ExecuteMove(move_y);
        EXPECT_TRUE(
            repeat_move_test_board->CalcFinalMovesOf(PieceColor::kRed).Size() > 0
        );
        repeat_move_test_board->ExecuteMove(move_z);
        EXPECT_TRUE(
            repeat_move_test_board->CalcFinalMovesOf(PieceColor::kRed).Size() > 0
        );
      }
    }

    repeat_move_test_board->ExecuteMove(move_y);
    EXPECT_TRUE(repeat_move_test_board->CalcFinalMovesOf(PieceColor::kRed).Size() == 0);
  }
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
  TestDrawDetection<
      boardstate::ZobristCalculatorForConcepts<uint64_t>,
      boardstate::ZobristCalculatorForConcepts<uint64_t>>();
}

TEST_F(GameBoardForConceptsTest, GetsCorrectOccupants) {
  TestGetsCorrectOccupants<
      boardstate::ZobristCalculatorForConcepts<uint64_t>,
      boardstate::ZobristCalculatorForConcepts<uint64_t>>();
}

TEST_F(GameBoardForConceptsTest, ExecuteAndUndoActualMove) {
  TestExecuteAndUndoActualMove<
      boardstate::ZobristCalculatorForConcepts<uint64_t>,
      boardstate::ZobristCalculatorForConcepts<uint64_t>>();
}

TEST_F(GameBoardForConceptsTest, CorrecNumSpacesOccupiedByBlack) {
  TestCorrecNumSpacesOccupiedBy<
      boardstate::ZobristCalculatorForConcepts<uint64_t>,
      boardstate::ZobristCalculatorForConcepts<uint64_t>>(PieceColor::kBlk);
}

TEST_F(GameBoardForConceptsTest, CorrecNumSpacesOccupiedByRed) {
  TestCorrecNumSpacesOccupiedBy<
      boardstate::ZobristCalculatorForConcepts<uint64_t>,
      boardstate::ZobristCalculatorForConcepts<uint64_t>>(PieceColor::kRed);
}

TEST_F(GameBoardForConceptsTest, CorrectNumberAvailableMoves) {
  TestCorrectNumberAvailableMoves<
      boardstate::ZobristCalculatorForConcepts<uint64_t>,
      boardstate::ZobristCalculatorForConcepts<uint64_t>>();
}

TEST_F(GameBoardForConceptsTest, ProhibitsTripleRepeatMovePeriod_02) {
  TestProhibitsTripleRepeatMovePeriod_02<
      boardstate::ZobristCalculatorForConcepts<uint64_t>,
      boardstate::ZobristCalculatorForConcepts<uint64_t>>();
}

TEST_F(GameBoardForConceptsTest, ProhibitsTripleRepeatMovePeriod_03) {
  TestProhibitsTripleRepeatMovePeriod_03<
      boardstate::ZobristCalculatorForConcepts<uint64_t>,
      boardstate::ZobristCalculatorForConcepts<uint64_t>>();
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
