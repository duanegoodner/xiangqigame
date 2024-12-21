#include <builders.hpp>
#include <concepts/board_state_calculator_registry.hpp>
#include <concepts/composite_concepts.hpp>
#include <concepts/space_info_provider.hpp>
#include <gameboard/game_board_for_concepts.hpp>
#include <gtest/gtest.h>
#include <memory>
#include <type_traits>
#include <boardstate/zobrist_calculator_for_concepts.hpp>

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

  gameboard::GameBoardFactory game_board_factory_;
  std::shared_ptr<gameboard::GameBoardForConcepts> starting_game_board_ =
      game_board_factory_.Create();

  template <typename RedKeyType, typename BlackKeyType>
  void BuildGameBoardWithAttachedZobristCalculators(
      size_t NumRedCalculators,
      size_t NumBlackCalculators
  ) {
    using RedCalculatorType = boardstate::ZobristCalculatorForConcepts<RedKeyType>;
    using BlackCalculatorType = boardstate::ZobristCalculatorForConcepts<BlackKeyType>;

    boardstate::ZobristCalculatorFactory<RedKeyType, gameboard::GameBoardForConcepts>
        red_zobrist_calculator_factory;
    boardstate::ZobristCalculatorFactory<BlackKeyType, gameboard::GameBoardForConcepts>
        black_zobrist_calculator_factory;

    auto red_zobrist_calculator =
        red_zobrist_calculator_factory.CreateRegisteredCalculator(starting_game_board_);
    auto black_zobrist_calculator =
        black_zobrist_calculator_factory.CreateRegisteredCalculator(starting_game_board_);
  }

  template <typename RedKeyType, typename BlackKeyType>
  void TestExecuteUndoMoveWithZobristCalculatorsAttached(
      size_t NumRedCalculatorsPerPlayer
  ) {

    boardstate::ZobristCalculatorFactory<RedKeyType, gameboard::GameBoardForConcepts>
        red_zobrist_calculator_factory;
    boardstate::ZobristCalculatorFactory<BlackKeyType, gameboard::GameBoardForConcepts>
        black_zobrist_calculator_factory;

    auto red_zobrist_calculator =
        red_zobrist_calculator_factory.CreateRegisteredCalculator(starting_game_board_);
    auto black_zobrist_calculator =
        black_zobrist_calculator_factory.CreateRegisteredCalculator(starting_game_board_);

    auto red_initial_state = red_zobrist_calculator->board_state();
    auto black_initial_state = black_zobrist_calculator->board_state();

    EXPECT_NE(red_initial_state, 0);
    EXPECT_NE(black_initial_state, 0);
    EXPECT_NE(red_initial_state, black_initial_state);

    auto actual_move = Move{BoardSpace{6, 2}, BoardSpace{5, 2}};
    auto actual_executed_move = starting_game_board_->ExecuteMove(actual_move);

    auto red_post_move_state = red_zobrist_calculator->board_state();
    auto black_post_move_state = black_zobrist_calculator->board_state();

    EXPECT_NE(red_initial_state, red_post_move_state);
    EXPECT_NE(black_initial_state, black_post_move_state);
  }
};

TEST_F(GameBoardForConceptsTest, SatisfiesSpaceInfoProviderConcept) {
  static_assert(
      SpaceInfoProviderConcept<gameboard::GameBoardForConcepts>,
      "GameBoardForConcepts must satisfy SpaceInfoProviderConcept"
  );
}

TEST_F(GameBoardForConceptsTest, SatisfiesBoardStateCalculatorRegistryConcept) {
  static_assert(
      BoardStateCalculatorRegistryConcept<gameboard::GameBoardForConcepts>,
      "GameBoardForConcepts must satisfy BoardStateCalculatorRegistryConcept"
  );
}

TEST_F(GameBoardForConceptsTest, SatisfiesProviderAndRegistryConcept) {
  static_assert(
      SpaceInfoProviderAndCalculatorRegistryConcept<gameboard::GameBoardForConcepts>,
      "GameBoardForConcepts must satisfy SpaceInfoProviderAndCalculatorRegistryConcept"
  );
}

TEST_F(GameBoardForConceptsTest, TestCreateGameBoard) {
  auto game_board = game_board_factory_.Create();
}

TEST_F(GameBoardForConceptsTest, TestGetsCorrectOccupants) {
  EXPECT_EQ(starting_game_board_->GetOccupantAt(BoardSpace{0, 0}), 5);
  EXPECT_EQ(starting_game_board_->GetOccupantAt(BoardSpace{1, 0}), 0);
  EXPECT_EQ(starting_game_board_->GetOccupantAt(BoardSpace{9, 7}), -4);
}

TEST_F(GameBoardForConceptsTest, TestDrawDetection) {
  auto draw_test_board = game_board_factory_.Create(kDrawTestBoard);
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
  auto actual_move = Move{BoardSpace{6, 2}, BoardSpace{5, 2}};
  auto actual_executed_move = starting_game_board_->ExecuteMove(actual_move);
  EXPECT_EQ(starting_game_board_->GetOccupantAt(BoardSpace{6, 2}), 0);
  EXPECT_EQ(starting_game_board_->GetOccupantAt(BoardSpace{5, 2}), -7);
  starting_game_board_->UndoMove(actual_executed_move);
  EXPECT_EQ(starting_game_board_->GetOccupantAt(BoardSpace{6, 2}), -7);
  EXPECT_EQ(starting_game_board_->GetOccupantAt(BoardSpace{5, 2}), 0);
}

TEST_F(GameBoardForConceptsTest, TestCorrectNumSpacesOccupied) {
  auto red_player_spaces =
      starting_game_board_->GetAllSpacesOccupiedBy(gameboard::PieceColor::kRed);
  auto black_player_spaces =
      starting_game_board_->GetAllSpacesOccupiedBy(gameboard::PieceColor::kBlk);

  EXPECT_EQ(red_player_spaces.size(), 16);
  EXPECT_EQ(black_player_spaces.size(), 16);
}

TEST_F(GameBoardForConceptsTest, TestCorrectNumberAvailableMoves) {
  auto black_moves = starting_game_board_->CalcFinalMovesOf(PieceColor::kBlk);
  auto red_moves = starting_game_board_->CalcFinalMovesOf(PieceColor::kRed);
}

TEST_F(GameBoardForConceptsTest, TestProhibitsTripleRepeatMovePeriod_02) {
  auto game_board = game_board_factory_.Create(kRepeatMoveTestBoard);
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
  auto game_board = game_board_factory_.Create(kRepeatMoveTestBoard);

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
