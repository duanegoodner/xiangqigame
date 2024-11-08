#include <game_board.hpp>
#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <string>
#include <zobrist.hpp>

class ZobristTrackerTest : public ::testing::Test {
protected:
  BoardMap_t board_map = int_board_to_game_pieces(kStartingBoard);

  boardstate::ZobristComponent<uint64_t, 1> zobrist_component_064_1{};
  boardstate::ZobristComponent<uint64_t, 0> zobrist_component_064_0{};
  boardstate::ZobristComponent<__uint128_t, 1> zobrist_component_128_1{};
};

TEST_F(ZobristTrackerTest, DefaultInit) {
  boardstate::ZobristTracker<uint32_t, 1> zobrist_tracker_032_1{};
  boardstate::ZobristTracker<uint64_t, 1> zobrist_tracker_064_1{};
  boardstate::ZobristTracker<__uint128_t, 1> zobrist_tracker_128_1{};
}

TEST_F(ZobristTrackerTest, InitWithExistingComponent) {
  boardstate::ZobristTracker<uint64_t, 1> zobrist_tracker_064_1{zobrist_component_064_1};
  boardstate::ZobristTracker<uint64_t, 0> zobrist_tracker_064_0{zobrist_tracker_064_0};
  boardstate::ZobristTracker<__uint128_t, 1> zobrist_tracker_128_1{
      zobrist_component_128_1
  };
}

TEST_F(ZobristTrackerTest, InitFromSeed) {
  boardstate::ZobristTracker<uint64_t, 1> zobrist_tracker_064_1{20241031};
  boardstate::ZobristTracker<uint64_t, 0> zobrist_component_064_0{20241030};
  boardstate::ZobristTracker<__uint128_t, 1> zobrist_tracker_128_1{20241029};
}

TEST_F(ZobristTrackerTest, ExecuteAndUndoMove64) {

  boardstate::ZobristTracker<uint64_t, 1> zobrist_tracker_064_1{};

  auto start = BoardSpace{6, 0};
  auto end = BoardSpace{5, 0};
  auto move = Move{start, end};
  auto moving_piece = GamePiece{PieceType::kSol, PieceColor::kRed};
  auto destination_piece = GamePiece{PieceType::kNnn, PieceColor::kNul};
  auto executed_move = ExecutedMove{move, moving_piece, destination_piece};

  zobrist_tracker_064_1.ImplementFullBoardStateCalc(board_map);
  auto initial_state = zobrist_tracker_064_1.ImplementGetState();
  zobrist_tracker_064_1.ImplementUpdateBoardState(executed_move);
  auto post_move_state = zobrist_tracker_064_1.ImplementGetState();
  zobrist_tracker_064_1.ImplementUpdateBoardState(executed_move);
  auto final_state = zobrist_tracker_064_1.ImplementGetState();

  EXPECT_NE(initial_state, post_move_state);
  EXPECT_EQ(initial_state, final_state);
}

TEST_F(ZobristTrackerTest, RecordAndReadData) {

  boardstate::ZobristTracker<uint64_t, 1> zobrist_tracker_064_1{};

  auto start = BoardSpace{6, 0};
  auto end = BoardSpace{5, 0};
  auto move = Move{start, end};
  auto moving_piece = GamePiece{PieceType::kSol, PieceColor::kRed};
  auto destination_piece = GamePiece{PieceType::kNnn, PieceColor::kNul};
  auto executed_move = ExecutedMove{move, moving_piece, destination_piece};

  zobrist_tracker_064_1.ImplementFullBoardStateCalc(board_map);
  auto initial_state = zobrist_tracker_064_1.ImplementGetState();

  MoveCollection dummy_move_collection{};
  dummy_move_collection.Append(move);

  moveselection::EqualScoreMoves dummy_equal_score_moves{1, dummy_move_collection};

  zobrist_tracker_064_1
      .RecordTrData(1, moveselection::MinimaxResultType::kFullyEvaluatedNode, dummy_equal_score_moves);

  auto retrieved_data = zobrist_tracker_064_1.GetTrData(1);
}
