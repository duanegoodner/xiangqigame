#include <gameboard/game_board.hpp>
#include <boardstate/zobrist.hpp>
#include <gameboard/move_calculator.hpp>

#include <gtest/gtest.h>

class GameBoardTest : public ::testing::Test {
protected:
  GameBoard gb_;
  const BoardMapInt_t kRepeatMoveTestBoard{{
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
};

const BoardMapInt_t kDrawTestBoard{{
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

// TEST_F(GameBoardTest, InitializesBoardState) {
//   EXPECT_NE(gb_.board_state(), 0);
// }

TEST_F(GameBoardTest, DetectsDraw) {
  GameBoard draw_game_board{kDrawTestBoard};

  for (auto round_trip = 0; round_trip < 10; ++round_trip) {
    for (auto idx = 0; idx < 6; ++idx) {
      BoardSpace start{3, idx};
      BoardSpace end{3, idx + 1};
      Move move{start, end};
      draw_game_board.ExecuteMove(move);
    }

    for (auto idx = 6; idx > 0; idx--) {
      BoardSpace start{3, idx};
      BoardSpace end{3, idx - 1};
      Move move{start, end};
      draw_game_board.ExecuteMove(move);
    }
  }
  auto is_draw = draw_game_board.IsDraw();
  auto red_available_moves = draw_game_board.CalcFinalMovesOf(PieceColor::kRed);
  std::cout << draw_game_board.IsDraw() << std::endl;
}

TEST_F(GameBoardTest, GetsCorrectOccupants) {
  EXPECT_EQ(gb_.GetOccupantAt(BoardSpace{0, 0}), 5);
  EXPECT_EQ(gb_.GetOccupantAt(BoardSpace{1, 0}), 0);
  EXPECT_EQ(gb_.GetOccupantAt(BoardSpace{9, 7}), -4);
}

// TEST_F(GameBoardTest, SimpleMove) {
//   auto start_state = gb_.board_state();
//   auto simple_move = Move{BoardSpace{0, 0}, BoardSpace{1, 0}};
//   auto executed_move = gb_.ExecuteMove(simple_move);
//   auto end_state = gb_.board_state();

//   EXPECT_NE(start_state, end_state);
//   EXPECT_EQ(gb_.GetOccupantAt(BoardSpace{0, 0}), 0);
//   EXPECT_EQ(gb_.GetOccupantAt(BoardSpace{1, 0}), 5);
// }

// After adding move_log_ to GameBoard, cannot undo a move
// that is not present in move_log_
// TEST_F(GameBoardTest, UndoPretendMove) {
//   auto pretend_executed_move =
//       ExecutedMove{Move{BoardSpace{8, 0}, BoardSpace{9, 0}}, -5, 0};
//   gb_.UndoMove(pretend_executed_move);
//   EXPECT_EQ(gb_.GetOccupantAt(BoardSpace{8, 0}), -5);
//   EXPECT_EQ(gb_.GetOccupantAt(BoardSpace{9, 0}), 0);
// }

TEST_F(GameBoardTest, ExecuteAndUndoActualMove) {
  // auto start_state = gb_.board_state();

  auto actual_move = Move{BoardSpace{6, 2}, BoardSpace{5, 2}};
  auto actual_executed_move = gb_.ExecuteMove(actual_move);
  // auto post_move_state = gb_.board_state();
  // EXPECT_NE(start_state, post_move_state);
  EXPECT_EQ(gb_.GetOccupantAt(BoardSpace{6, 2}), 0);
  EXPECT_EQ(gb_.GetOccupantAt(BoardSpace{5, 2}), -7);
  gb_.UndoMove(actual_executed_move);
  // auto end_state = gb_.board_state();
  // EXPECT_EQ(start_state, end_state);
  EXPECT_EQ(gb_.GetOccupantAt(BoardSpace{6, 2}), -7);
  EXPECT_EQ(gb_.GetOccupantAt(BoardSpace{5, 2}), 0);
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

// TEST_F(GameBoardTest, AttachSingleZobristCoordinators) {
//   auto red_hash_calculator = SingleZobristCoordinator();
//   auto black_hash_calculator = SingleZobristCoordinator();
//   gb_.ImplementAttachSingleZobristCoordinator(&red_hash_calculator, 0);
//   gb_.ImplementAttachSingleZobristCoordinator(&black_hash_calculator, 1);
// }

TEST_F(GameBoardTest, ExecuteMoveWithAttachedSingleZobristCoordinators) {
  auto red_hash_calculator = boardstate::ZobristCoordinator<uint64_t, 0>(1234);
  auto black_hash_calculator = boardstate::ZobristCoordinator<uint64_t, 0>(5678);
  // gb_.ImplementAttachSingleZobristCoordinator(&red_hash_calculator, 0);
  // gb_.ImplementAttachSingleZobristCoordinator(&black_hash_calculator, 1);
  auto actual_move = Move{BoardSpace{6, 2}, BoardSpace{5, 2}};
  auto actual_executed_move = gb_.ExecuteMove(actual_move);
  gb_.UndoMove(actual_executed_move);
}

TEST_F(GameBoardTest, ProhibitsTripleRepeatMovePeriod_02) {
  GameBoard late_game_board(kRepeatMoveTestBoard);
  auto red_king_position_a = BoardSpace{9, 4};
  auto red_king_position_b = BoardSpace{9, 3};

  Move move_x = Move{red_king_position_a, red_king_position_b};
  Move move_y = Move{red_king_position_b, red_king_position_a};

  for (int round_trips = 0; round_trips < 2; round_trips++) {
    late_game_board.ExecuteMove(move_x);
    auto avail_moves_a = late_game_board.CalcFinalMovesOf(PieceColor::kRed);
    EXPECT_TRUE(avail_moves_a.Size() > 0);

    late_game_board.ExecuteMove(move_y);
    auto avail_moves_b = late_game_board.CalcFinalMovesOf(PieceColor::kRed);
    EXPECT_TRUE(avail_moves_b.Size() > 0);
  }
  late_game_board.ExecuteMove(move_x);
  auto avail_moves_c = late_game_board.CalcFinalMovesOf(PieceColor::kRed);
  EXPECT_TRUE(avail_moves_c.Size() == 0);
}

TEST_F(GameBoardTest, ProhibitsTripleRepeatMovePeriod_03) {
  GameBoard late_game_board(kRepeatMoveTestBoard);
  auto red_king_position_a = BoardSpace{9, 4};
  auto red_king_position_b = BoardSpace{9, 3};
  auto red_king_position_c = BoardSpace{9, 5};

  Move move_w = Move{red_king_position_a, red_king_position_b};
  Move move_x = Move{red_king_position_b, red_king_position_a};
  Move move_y = Move{red_king_position_a, red_king_position_c};
  Move move_z = Move{red_king_position_c, red_king_position_a};

  for (int cycles = 0; cycles < 3; cycles++) {
    late_game_board.ExecuteMove(move_w);
    EXPECT_TRUE(late_game_board.CalcFinalMovesOf(PieceColor::kRed).Size() > 0);
    late_game_board.ExecuteMove(move_x);
    EXPECT_TRUE(late_game_board.CalcFinalMovesOf(PieceColor::kRed).Size() > 0);

    if (cycles < 2) {
      late_game_board.ExecuteMove(move_y);
      EXPECT_TRUE(late_game_board.CalcFinalMovesOf(PieceColor::kRed).Size() > 0);
      late_game_board.ExecuteMove(move_z);
      EXPECT_TRUE(late_game_board.CalcFinalMovesOf(PieceColor::kRed).Size() > 0);
    }
  }

  late_game_board.ExecuteMove(move_y);
  EXPECT_TRUE(late_game_board.CalcFinalMovesOf(PieceColor::kRed).Size() == 0);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}