#include <builders.hpp>
#include <concept_board_state_calculator_registry.hpp>
#include <concept_composite_concepts.hpp>
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

  std::shared_ptr<gameboard::GameBoardForConcepts<
      NullBoardStateCalculator,
      NullBoardStateCalculator>>
      starting_board_null_zcalcs_ = gameboard::GameBoardForConcepts<
          NullBoardStateCalculator,
          NullBoardStateCalculator>::Create();

  template <BoardStateCalculatorConcept RC, BoardStateCalculatorConcept BC>
  std::shared_ptr<gameboard::GameBoardForConcepts<RC, BC>> BuildGameBoard(
      const BoardMapInt_t &starting_board = gameboard::kStandardInitialBoard
  ) {
    return gameboard::GameBoardForConcepts<RC, BC>::Create(starting_board);
  }

  template <typename RedKeyType, typename BlackKeyType>
  void BuilldGameBoardForZobristCalculators(
      const BoardMapInt_t &starting_board = gameboard::kStandardInitialBoard
  ) {
    using RedCalculatorType = boardstate::ZobristCalculatorForConcepts<RedKeyType>;
    using BlackCalculatorType = boardstate::ZobristCalculatorForConcepts<BlackKeyType>;
    using GameBoardFactoryType =
        gameboard::GameBoardFactory<RedCalculatorType, BlackCalculatorType>;

    GameBoardFactoryType game_board_factory;
    auto game_board = game_board_factory.Create(starting_board);
  }

  template <typename RedKeyType, typename BlackKeyType>
  void BuildGameBoardWithAttachedZobristCalculators(
      size_t NumRedCalculators,
      size_t NumBlackCalculators
  ) {
    using RedCalculatorType = boardstate::ZobristCalculatorForConcepts<RedKeyType>;
    using BlackCalculatorType = boardstate::ZobristCalculatorForConcepts<BlackKeyType>;
    using GameBoardFactoryType =
        gameboard::GameBoardFactory<RedCalculatorType, BlackCalculatorType>;

    using GameBoardType = GameBoardFactoryType::GameBoardType;

    GameBoardFactoryType game_board_factory;
    auto game_board = game_board_factory.Create();

    boardstate::ZobristCalculatorFactory<RedKeyType, GameBoardType>
        red_zobrist_calculator_factory;
    boardstate::ZobristCalculatorFactory<BlackKeyType, GameBoardType>
        black_zobrist_calculator_factory;

    auto red_zobrist_calculator =
        red_zobrist_calculator_factory.Create(game_board, gameboard::PieceColor::kRed);
    auto black_zobrist_calculator =
        black_zobrist_calculator_factory.Create(game_board, gameboard::PieceColor::kBlk);
  }

  template <typename RedKeyType, typename BlackKeyType>
  void TestExecuteUndoMoveWithZobristCalculatorsAttached(
      size_t NumRedCalculatorsPerPlayer
  ) {
    using GameBoardFactoryType = gameboard::GameBoardFactory<
        boardstate::ZobristCalculatorForConcepts<RedKeyType>,
        boardstate::ZobristCalculatorForConcepts<BlackKeyType>>;

    using GameBoardType = GameBoardFactoryType::GameBoardType;

    GameBoardFactoryType game_board_factory;
    auto game_board = game_board_factory.Create();

    boardstate::ZobristCalculatorFactory<RedKeyType, GameBoardType>
        red_zobrist_calculator_factory;
    boardstate::ZobristCalculatorFactory<BlackKeyType, GameBoardType>
        black_zobrist_calculator_factory;

    auto red_zobrist_calculator =
        red_zobrist_calculator_factory.Create(game_board, gameboard::PieceColor::kRed);
    auto black_zobrist_calculator =
        black_zobrist_calculator_factory.Create(game_board, gameboard::PieceColor::kBlk);

    auto red_initial_state = red_zobrist_calculator->board_state();
    auto black_initial_state = black_zobrist_calculator->board_state();

    EXPECT_NE(red_initial_state, 0);
    EXPECT_NE(black_initial_state, 0);
    EXPECT_NE(red_initial_state, black_initial_state);

    auto actual_move = Move{BoardSpace{6, 2}, BoardSpace{5, 2}};
    auto actual_executed_move = game_board->ExecuteMove(actual_move);

    auto red_post_move_state = red_zobrist_calculator->board_state();
    auto black_post_move_state = black_zobrist_calculator->board_state();

    EXPECT_NE(red_initial_state, red_post_move_state);
    EXPECT_NE(black_initial_state, black_post_move_state);
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

TEST_F(GameBoardForConceptsTest, SatisfiesBoardStateCalculatorRegistryConcept) {
  static_assert(
      BoardStateCalculatorRegistryConcept<gameboard::GameBoardForConcepts<
          boardstate::ZobristCalculatorForConcepts<uint64_t>,
          boardstate::ZobristCalculatorForConcepts<uint64_t>>>,
      "GameBoardForConcepts must satisfy SpaceInfoProviderConcept"
  );
}

TEST_F(GameBoardForConceptsTest, SatisfiesProviderAndRegistryConcept) {
  static_assert(
      SpaceInfoProviderAndCalculatorRegistryConcept<gameboard::GameBoardForConcepts<
          boardstate::ZobristCalculatorForConcepts<uint64_t>,
          boardstate::ZobristCalculatorForConcepts<uint64_t>>>,
      "GameBoardForConcepts must satisfy SpaceInfoProviderAndCalculatorRegistryConcept"
  );
}

TEST_F(GameBoardForConceptsTest, TestCreateGameBoard) {
  auto game_board = gameboard::
      GameBoardForConcepts<NullBoardStateCalculator, NullBoardStateCalculator>::Create();
}

TEST_F(GameBoardForConceptsTest, TestBuildBasedOnKeyType) {
  BuilldGameBoardForZobristCalculators<uint64_t, uint64_t>();
}

TEST_F(GameBoardForConceptsTest, TestGetsCorrectOccupants) {
  EXPECT_EQ(starting_board_null_zcalcs_->GetOccupantAt(BoardSpace{0, 0}), 5);
  EXPECT_EQ(starting_board_null_zcalcs_->GetOccupantAt(BoardSpace{1, 0}), 0);
  EXPECT_EQ(starting_board_null_zcalcs_->GetOccupantAt(BoardSpace{9, 7}), -4);
}

TEST_F(GameBoardForConceptsTest, TestDrawDetection) {
  auto draw_test_board = gameboard::GameBoardForConcepts<
      NullBoardStateCalculator,
      NullBoardStateCalculator>::Create(kDrawTestBoard);
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

TEST_F(GameBoardForConceptsTest, TestExecuteAndUndoMove) {
  // auto game_board = gameboard::
  //     GameBoardForConcepts<NullBoardStateCalculator,
  //     NullBoardStateCalculator>::Create();
  auto actual_move = Move{BoardSpace{6, 2}, BoardSpace{5, 2}};
  auto actual_executed_move = starting_board_null_zcalcs_->ExecuteMove(actual_move);
  EXPECT_EQ(starting_board_null_zcalcs_->GetOccupantAt(BoardSpace{6, 2}), 0);
  EXPECT_EQ(starting_board_null_zcalcs_->GetOccupantAt(BoardSpace{5, 2}), -7);
  starting_board_null_zcalcs_->UndoMove(actual_executed_move);
  EXPECT_EQ(starting_board_null_zcalcs_->GetOccupantAt(BoardSpace{6, 2}), -7);
  EXPECT_EQ(starting_board_null_zcalcs_->GetOccupantAt(BoardSpace{5, 2}), 0);
}

TEST_F(GameBoardForConceptsTest, TestCorrectNumSpacesOccupied) {
  auto game_board = gameboard::
      GameBoardForConcepts<NullBoardStateCalculator, NullBoardStateCalculator>::Create();
  auto red_player_spaces =
      game_board->GetAllSpacesOccupiedBy(gameboard::PieceColor::kRed);
  auto black_player_spaces =
      game_board->GetAllSpacesOccupiedBy(gameboard::PieceColor::kBlk);

  EXPECT_EQ(red_player_spaces.size(), 16);
  EXPECT_EQ(black_player_spaces.size(), 16);
}

TEST_F(GameBoardForConceptsTest, TestCorrectNumberAvailableMoves) {
  auto game_board = gameboard::
      GameBoardForConcepts<NullBoardStateCalculator, NullBoardStateCalculator>::Create();
  auto black_moves = game_board->CalcFinalMovesOf(PieceColor::kBlk);
  auto red_moves = game_board->CalcFinalMovesOf(PieceColor::kRed);
}

TEST_F(GameBoardForConceptsTest, TestProhibitsTripleRepeatMovePeriod_02) {
  auto game_board = gameboard::GameBoardForConcepts<
      NullBoardStateCalculator,
      NullBoardStateCalculator>::Create(kRepeatMoveTestBoard);
  auto red_king_position_a = BoardSpace{9, 4};
  auto red_king_position_b = BoardSpace{9, 3};

  Move move_x = Move{red_king_position_a, red_king_position_b};
  Move move_y = Move{red_king_position_b, red_king_position_a};

  for (int round_trips = 0; round_trips < 2; round_trips++) {
    game_board->ExecuteMove(move_x);
    auto avail_moves_a = game_board->CalcFinalMovesOf(PieceColor::kRed);
    EXPECT_TRUE(avail_moves_a.Size() > 0);

    game_board->ExecuteMove(move_y);
    auto avail_moves_b = game_board->CalcFinalMovesOf(PieceColor::kRed);
    EXPECT_TRUE(avail_moves_b.Size() > 0);
  }
  game_board->ExecuteMove(move_x);
  auto avail_moves_c = game_board->CalcFinalMovesOf(PieceColor::kRed);
  EXPECT_TRUE(avail_moves_c.Size() == 0);
}

TEST_F(GameBoardForConceptsTest, TestProhibitsTripleRepeatMovePeriod_03) {
  auto game_board = gameboard::GameBoardForConcepts<
      NullBoardStateCalculator,
      NullBoardStateCalculator>::Create(kRepeatMoveTestBoard);

  auto red_king_position_a = BoardSpace{9, 4};
  auto red_king_position_b = BoardSpace{9, 3};
  auto red_king_position_c = BoardSpace{9, 5};

  Move move_w = Move{red_king_position_a, red_king_position_b};
  Move move_x = Move{red_king_position_b, red_king_position_a};
  Move move_y = Move{red_king_position_a, red_king_position_c};
  Move move_z = Move{red_king_position_c, red_king_position_a};

  for (int cycles = 0; cycles < 3; cycles++) {
    game_board->ExecuteMove(move_w);
    EXPECT_TRUE(game_board->CalcFinalMovesOf(PieceColor::kRed).Size() > 0);
    game_board->ExecuteMove(move_x);
    EXPECT_TRUE(game_board->CalcFinalMovesOf(PieceColor::kRed).Size() > 0);

    if (cycles < 2) {
      game_board->ExecuteMove(move_y);
      EXPECT_TRUE(game_board->CalcFinalMovesOf(PieceColor::kRed).Size() > 0);
      game_board->ExecuteMove(move_z);
      EXPECT_TRUE(game_board->CalcFinalMovesOf(PieceColor::kRed).Size() > 0);
    }
  }
}

TEST_F(GameBoardForConceptsTest, TestAttachZobristCalculators) {

  BuildGameBoardWithAttachedZobristCalculators<uint64_t, uint64_t>(2, 2);
}

TEST_F(GameBoardForConceptsTest, ExecuteAndUndoMoveWithZobristCalculatorsAttached) {
  TestExecuteUndoMoveWithZobristCalculatorsAttached<uint64_t, uint64_t>(2);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
