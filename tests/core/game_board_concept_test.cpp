#include <builders.hpp>
#include <concept_space_info_provider.hpp>
#include <game_board_for_concepts.hpp>
#include <gtest/gtest.h>
#include <memory>
#include <type_traits>
#include <zobrist_calculator_for_concepts.hpp>

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

  template <BoardStateCalculatorConcept C>
  std::vector<std::shared_ptr<C>> BuildVectorOfCalculators(size_t NumCalculators) {
    std::vector<std::shared_ptr<C>> calculators;
    for (auto idx = 0; idx < NumCalculators; ++idx) {
      calculators.emplace_back(C::Create());
    }
    return calculators;
  }

  template <BoardStateCalculatorConcept RC, BoardStateCalculatorConcept BC>
  std::shared_ptr<gameboard::GameBoardForConcepts<RC, BC>>
  BuildGameBoardBasedOnCalcConcepts(
      size_t NumCalculatorsRed = 2,
      size_t NumCalculatorsBlack = 2,
      const BoardMapInt_t starting_board = kStandardInitialBoard
  ) {
    auto red_z_calculators = BuildVectorOfCalculators<RC>(NumCalculatorsRed);
    auto black_z_calculators = BuildVectorOfCalculators<BC>(NumCalculatorsBlack);

    return gameboard::GameBoardForConcepts<RC, BC>::Create(
        red_z_calculators,
        black_z_calculators,
        starting_board
    );
  }

  template <typename KeyTypeRed, typename KeyTypeBlack>
  std::shared_ptr<gameboard::GameBoardForConcepts<
      boardstate::ZobristCalculatorForConcepts<KeyTypeRed>,
      boardstate::ZobristCalculatorForConcepts<KeyTypeBlack>>>
  BuildGameBoardWithZobristHashCalculators(
      size_t NumCalculatorsRed = 2,
      size_t NumCalculatorsBlack = 2,
      const BoardMapInt_t starting_board = kStandardInitialBoard

  ) {
    return BuildGameBoardBasedOnCalcConcepts<
        boardstate::ZobristCalculatorForConcepts<KeyTypeRed>,
        boardstate::ZobristCalculatorForConcepts<KeyTypeBlack>>(
        NumCalculatorsRed,
        NumCalculatorsBlack,
        starting_board
    );
  }

  template <typename KeyTypeRed, typename KeyTypeBlack>
  void TestDrawDetection() {
    auto draw_test_board =
        BuildGameBoardWithZobristHashCalculators<KeyTypeRed, KeyTypeBlack>(
            2,
            2,
            kDrawTestBoard
        );
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
    EXPECT_TRUE(is_draw);
    EXPECT_EQ(red_available_moves.Size(), 0);
  }

  template <typename KeyTypeRed, typename KeyTypeBlack>
  void TestGetsCorrectOccupants() {
    auto starting_board =
        BuildGameBoardWithZobristHashCalculators<KeyTypeRed, KeyTypeBlack>();
    EXPECT_EQ(starting_board->GetOccupantAt(BoardSpace{0, 0}), 5);
    EXPECT_EQ(starting_board->GetOccupantAt(BoardSpace{1, 0}), 0);
    EXPECT_EQ(starting_board->GetOccupantAt(BoardSpace{9, 7}), -4);
  }

  template <typename KeyTypeRed, typename KeyTypeBlack>
  void TestExecuteAndUndoActualMove() {
    auto starting_board =
        BuildGameBoardWithZobristHashCalculators<KeyTypeRed, KeyTypeBlack>();
    auto actual_move = Move{BoardSpace{6, 2}, BoardSpace{5, 2}};
    auto actual_executed_move = starting_board->ExecuteMove(actual_move);
    EXPECT_EQ(starting_board->GetOccupantAt(BoardSpace{6, 2}), 0);
    EXPECT_EQ(starting_board->GetOccupantAt(BoardSpace{5, 2}), -7);
    starting_board->UndoMove(actual_executed_move);
    EXPECT_EQ(starting_board->GetOccupantAt(BoardSpace{6, 2}), -7);
    EXPECT_EQ(starting_board->GetOccupantAt(BoardSpace{5, 2}), 0);
  }

  template <typename KeyTypeRed, typename KeyTypeBlack>
  void TestCorrectNumSpacesOccupied() {
    auto starting_board =
        BuildGameBoardWithZobristHashCalculators<KeyTypeRed, KeyTypeBlack>();
    auto red_player_spaces =
        starting_board->GetAllSpacesOccupiedBy(gameboard::PieceColor::kRed);
    auto black_player_spaces =
        starting_board->GetAllSpacesOccupiedBy(gameboard::PieceColor::kBlk);

    EXPECT_EQ(red_player_spaces.size(), 16);
    EXPECT_EQ(black_player_spaces.size(), 16);
  }

  template <typename KeyTypeRed, typename KeyTypeBlack>
  void TestCorrectNumberAvailableMoves() {
    auto starting_board =
        BuildGameBoardWithZobristHashCalculators<KeyTypeRed, KeyTypeBlack>();
    auto black_moves = starting_board->CalcFinalMovesOf(PieceColor::kBlk);
    auto red_moves = starting_board->CalcFinalMovesOf(PieceColor::kRed);
  }

  template <typename KeyTypeRed, typename KeyTypeBlack>
  void TestProhibitsTripleRepeatMovePeriod_02() {
    auto repeat_move_test_board =
        BuildGameBoardWithZobristHashCalculators<KeyTypeRed, KeyTypeBlack>(
            2,
            2,
            kRepeatMoveTestBoard
        );

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

  template <typename KeyTypeRed, typename KeyTypeBlack>
  void TestProhibitsTripleRepeatMovePeriod_03() {
    auto repeat_move_test_board =
        BuildGameBoardWithZobristHashCalculators<KeyTypeRed, KeyTypeBlack>(
            2,
            2,
            kRepeatMoveTestBoard
        );
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
  }

  std::shared_ptr<gameboard::GameBoardForConcepts<
      NullBoardStateCalculator,
      NullBoardStateCalculator>>
  BuildGameBoardWithNullBoardStateCalculators() {
    std::vector<std::shared_ptr<NullBoardStateCalculator>> red_z_calculators;
    std::vector<std::shared_ptr<NullBoardStateCalculator>> black_z_calculators;

    red_z_calculators.emplace_back(NullBoardStateCalculator::Create());
    black_z_calculators.emplace_back(NullBoardStateCalculator::Create());

    return gameboard::GameBoardForConcepts<
        NullBoardStateCalculator,
        NullBoardStateCalculator>::Create(red_z_calculators, black_z_calculators);
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

TEST_F(GameBoardForConceptsTest, TestBuildVectorOfCalculators) {
  auto calculators =
      BuildVectorOfCalculators<boardstate::ZobristCalculatorForConcepts<uint64_t>>(2);
  EXPECT_EQ(calculators.size(), 2);
}

TEST_F(GameBoardForConceptsTest, TestBuildGameBoard) {
  auto game_board = BuildGameBoardBasedOnCalcConcepts<
      boardstate::ZobristCalculatorForConcepts<uint64_t>,
      boardstate::ZobristCalculatorForConcepts<uint64_t>>(2, 2);
}

TEST_F(GameBoardForConceptsTest, TestBuildBasedOnKeyType) {
  auto game_board = BuildGameBoardWithZobristHashCalculators<uint64_t, uint64_t>(2, 2);
}

TEST_F(GameBoardForConceptsTest, TestGetsCorrectOccupants) {
  TestGetsCorrectOccupants<uint64_t, uint64_t>();
}

TEST_F(GameBoardForConceptsTest, TestDrawDetection) {
  TestDrawDetection<uint64_t, uint64_t>();
}

TEST_F(GameBoardForConceptsTest, TestExecuteAndUndoMove) {
  TestExecuteAndUndoActualMove<uint64_t, uint64_t>();
}

TEST_F(GameBoardForConceptsTest, TestCorrectNumSpacesOccupied) {
  TestCorrectNumSpacesOccupied<uint64_t, uint64_t>();
}

TEST_F(GameBoardForConceptsTest, TestCorrectNumberAvailableMoves) {
  TestCorrectNumberAvailableMoves<uint64_t, uint64_t>();
}

TEST_F(GameBoardForConceptsTest, TestProhibitsTripleRepeatMovePeriod_02) {
  TestProhibitsTripleRepeatMovePeriod_02<uint64_t, uint64_t>();
}

TEST_F(GameBoardForConceptsTest, TestProhibitsTripleRepeatMovePeriod_03) {
  TestProhibitsTripleRepeatMovePeriod_03<uint64_t, uint64_t>();
}

TEST_F(GameBoardForConceptsTest, TestWithNullBoardStateCalculators) {

  auto game_board = BuildGameBoardWithNullBoardStateCalculators();
}

TEST_F(GameBoardForConceptsTest, ExecuteMoveWithNullBoardStateCalculators) {
  auto starting_board = BuildGameBoardWithNullBoardStateCalculators();
  auto actual_move = Move{BoardSpace{6, 2}, BoardSpace{5, 2}};
  auto actual_executed_move = starting_board->ExecuteMove(actual_move);
  EXPECT_EQ(starting_board->GetOccupantAt(BoardSpace{6, 2}), 0);
  EXPECT_EQ(starting_board->GetOccupantAt(BoardSpace{5, 2}), -7);
  starting_board->UndoMove(actual_executed_move);
  EXPECT_EQ(starting_board->GetOccupantAt(BoardSpace{6, 2}), -7);
  EXPECT_EQ(starting_board->GetOccupantAt(BoardSpace{5, 2}), 0);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
