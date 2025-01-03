#include <gameboard/game_board.hpp>
#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <string>
#include <boardstate/zobrist.hpp>

class ZobristCoordinatorTest : public ::testing::Test {
protected:
  BoardMap_t board_map = int_board_to_game_pieces(kStandardInitialBoard);

  boardstate::ZobristComponent<uint64_t, 1> zobrist_component_064_1{};
  boardstate::ZobristComponent<uint64_t, 0> zobrist_component_064_0{};
  boardstate::ZobristComponent<__uint128_t, 1> zobrist_component_128_1{};
};

TEST_F(ZobristCoordinatorTest, DefaultInit) {
  boardstate::ZobristCoordinator<uint32_t, 1> zobrist_coordinator_032_1{};
  boardstate::ZobristCoordinator<uint64_t, 1> zobrist_coordinator_064_1{};
  boardstate::ZobristCoordinator<__uint128_t, 1> zobrist_coordinator_128_1{};
}

TEST_F(ZobristCoordinatorTest, InitWithExistingComponent) {
  boardstate::ZobristCoordinator<uint64_t, 1> zobrist_coordinator_064_1{
      zobrist_component_064_1
  };
  boardstate::ZobristCoordinator<uint64_t, 0> zobrist_coordinator_064_0{
      zobrist_component_064_0
  };
  boardstate::ZobristCoordinator<__uint128_t, 1> zobrist_coordinator_128_1{
      zobrist_component_128_1
  };
}

TEST_F(ZobristCoordinatorTest, InitFromSeed) {
  boardstate::ZobristCoordinator<uint64_t, 1> zobrist_coordinator_064_1{20241031};
  boardstate::ZobristCoordinator<uint64_t, 0> zobrist_component_064_0{20241030};
  boardstate::ZobristCoordinator<__uint128_t, 1> zobrist_coordinator_128_1{20241029};
}

TEST_F(ZobristCoordinatorTest, ExecuteAndUndoMove64) {

  boardstate::ZobristCoordinator<uint64_t, 1> zobrist_coordinator_064_1{};

  auto start = BoardSpace{6, 0};
  auto end = BoardSpace{5, 0};
  auto move = Move{start, end};
  auto moving_piece = GamePiece{PieceType::kSol, PieceColor::kRed};
  auto destination_piece = GamePiece{PieceType::kNnn, PieceColor::kNul};
  auto executed_move = ExecutedMove{move, moving_piece, destination_piece};

  zobrist_coordinator_064_1.ImplementFullBoardStateCalc(board_map);
  auto initial_state = zobrist_coordinator_064_1.ImplementGetState();
  zobrist_coordinator_064_1.ImplementUpdateBoardState(executed_move);
  auto post_move_state = zobrist_coordinator_064_1.ImplementGetState();
  zobrist_coordinator_064_1.ImplementUpdateBoardState(executed_move);
  auto final_state = zobrist_coordinator_064_1.ImplementGetState();

  EXPECT_NE(initial_state, post_move_state);
  EXPECT_EQ(initial_state, final_state);
}

TEST_F(ZobristCoordinatorTest, RecordAndReadData) {

  boardstate::ZobristCoordinator<uint64_t, 1> zobrist_coordinator_064_1{};

  auto start = BoardSpace{6, 0};
  auto end = BoardSpace{5, 0};
  auto move = Move{start, end};
  auto moving_piece = GamePiece{PieceType::kSol, PieceColor::kRed};
  auto destination_piece = GamePiece{PieceType::kNnn, PieceColor::kNul};
  auto executed_move = ExecutedMove{move, moving_piece, destination_piece};

  zobrist_coordinator_064_1.ImplementFullBoardStateCalc(board_map);
  auto initial_state = zobrist_coordinator_064_1.ImplementGetState();

  MoveCollection dummy_move_collection{};
  dummy_move_collection.Append(move);

  moveselection::EqualScoreMoves dummy_equal_score_moves{1, dummy_move_collection};
  int dummy_search_depth = 1;
  int dummy_access_index_at_write = 0;

  zobrist_coordinator_064_1.RecordTrData(
      dummy_search_depth,
      moveselection::MinimaxResultType::kFullyEvaluatedNode,
      dummy_equal_score_moves,
      dummy_access_index_at_write
  );

  int dummy_access_index_at_read = 1;
  auto retrieved_data = zobrist_coordinator_064_1.GetTrData(
      dummy_search_depth,
      dummy_access_index_at_read
  );

  EXPECT_TRUE(retrieved_data.found());
}
