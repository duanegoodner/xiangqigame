// #include <common.hpp>
#include <game_board.hpp>
#include <gtest/gtest.h>
#include <hash_calculator.hpp>
#include <nlohmann/json.hpp>
#include <string>

using namespace boardstate;
using namespace nlohmann::json_abi_v3_11_3;
using namespace std;

class ZobristCalculatorTest : public ::testing::Test {
protected:
  BoardMap_t board_map = int_board_to_game_pieces(kStartingBoard);
};

TEST_F(ZobristCalculatorTest, DefaultInit) {
  auto zobrist_keys_064 = ZobristCalculator<uint64_t>(123456);
  EXPECT_NE(0, zobrist_keys_064.turn_key());
  EXPECT_EQ(2, zobrist_keys_064.zarray().size());
  EXPECT_EQ(kNumPieceTypeVals, zobrist_keys_064.zarray()[0].size());
  EXPECT_EQ(kNumRanks, zobrist_keys_064.zarray()[0][0].size());
  EXPECT_EQ(kNumFiles, zobrist_keys_064.zarray()[0][0][0].size());
}

TEST_F(ZobristCalculatorTest, InitFromSeed) {
  auto zobrist_keys_064 = ZobristCalculator<uint64_t>(123456);
  EXPECT_NE(0, zobrist_keys_064.turn_key());
  EXPECT_EQ(2, zobrist_keys_064.zarray().size());
  EXPECT_EQ(kNumPieceTypeVals, zobrist_keys_064.zarray()[0].size());
  EXPECT_EQ(kNumRanks, zobrist_keys_064.zarray()[0][0].size());
  EXPECT_EQ(kNumFiles, zobrist_keys_064.zarray()[0][0][0].size());
}

TEST_F(ZobristCalculatorTest, GetHashValueAt) {
  auto zobrist_keys_064 = ZobristCalculator<uint64_t>(1234567);
  auto sample_key_value = zobrist_keys_064.GetHashValueAt(
      PieceColor::kBlk,
      PieceType::kAdv,
      BoardSpace{0, 3}
  );
}

class SingleZobristTrackerTest : public ::testing::Test {

protected:
  BoardMap_t board_map = int_board_to_game_pieces(kStartingBoard);
  ZobristCalculator<uint32_t> zobrist_keys_032{78910};
  ZobristCalculator<uint64_t> zobrist_keys_064{654321};
  ZobristCalculator<__uint128_t> zobrist_keys_128{2468};
};

TEST_F(SingleZobristTrackerTest, InitializeFromKeysAndMap) {
  auto my_hash_calculator_064 = SingleZobristTracker<uint64_t>(zobrist_keys_064);
  auto my_hash_calculator_128 = SingleZobristTracker<__uint128_t>(zobrist_keys_128);
}

TEST_F(SingleZobristTrackerTest, DefaultInit) {
  auto my_hash_calculator_064 = SingleZobristTracker<uint64_t>(3579);
  auto my_hash_calculator_128 = SingleZobristTracker<__uint128_t>(8675309);

  auto dual_hash_calculator_064 = DualZobristTracker<uint64_t>();
  auto dual_hash_calculator_128 = DualZobristTracker<__uint128_t>();
}

TEST_F(SingleZobristTrackerTest, InitializeBoardState) {
  auto my_hash_calculator_064 = SingleZobristTracker<uint64_t>(22443355);
  uint64_t board_state{0};
  EXPECT_EQ(my_hash_calculator_064.ImplementGetState(), 0);
  my_hash_calculator_064.ImplementFullBoardStateCalc(board_map);
  EXPECT_NE(my_hash_calculator_064.ImplementGetState(), 0);
}

TEST_F(SingleZobristTrackerTest, BoardStateHexString) {
  auto my_hash_calculator_032 = SingleZobristTracker<uint32_t>(22443355);
  my_hash_calculator_032.ImplementFullBoardStateCalc(board_map);
  auto hex_state_032 = my_hash_calculator_032.board_state_hex_str();
  std::cout << hex_state_032 << std::endl;

  auto my_hash_calculator_064 = SingleZobristTracker<uint64_t>(22443355);
  my_hash_calculator_064.ImplementFullBoardStateCalc(board_map);
  auto hex_state_064 = my_hash_calculator_064.board_state_hex_str();
  std::cout << hex_state_064 << std::endl;

  auto my_hash_calculator_128 = SingleZobristTracker<__uint128_t>(22443355);
  my_hash_calculator_128.ImplementFullBoardStateCalc(board_map);
  auto hex_state_128 = my_hash_calculator_128.board_state_hex_str();
  std::cout << hex_state_128 << std::endl;
}

TEST_F(SingleZobristTrackerTest, ExecuteAndUndoMove64) {

  auto my_hash_calculator = SingleZobristTracker<uint64_t>(zobrist_keys_064);

  auto start = BoardSpace{6, 0};
  auto end = BoardSpace{5, 0};
  auto move = Move{start, end};
  auto moving_piece = GamePiece{PieceType::kSol, PieceColor::kRed};
  auto destination_piece = GamePiece{PieceType::kNnn, PieceColor::kNul};
  auto executed_move = ExecutedMove{move, moving_piece, destination_piece};

  my_hash_calculator.ImplementFullBoardStateCalc(board_map);
  auto initial_state = my_hash_calculator.ImplementGetState();
  my_hash_calculator.ImplementUpdateBoardState(executed_move);
  auto post_move_state = my_hash_calculator.ImplementGetState();
  my_hash_calculator.ImplementUpdateBoardState(executed_move);
  auto final_state = my_hash_calculator.ImplementGetState();

  EXPECT_NE(initial_state, post_move_state);
  EXPECT_EQ(initial_state, final_state);
}

TEST_F(SingleZobristTrackerTest, ExecuteAndUndoMove128) {

  auto my_hash_calculator = SingleZobristTracker<__uint128_t>(zobrist_keys_128);

  auto start = BoardSpace{6, 0};
  auto end = BoardSpace{5, 0};
  auto move = Move{start, end};
  auto moving_piece = GamePiece{PieceType::kSol, PieceColor::kRed};
  auto destination_piece = GamePiece{PieceType::kNnn, PieceColor::kNul};
  auto executed_move = ExecutedMove{move, moving_piece, destination_piece};

  my_hash_calculator.ImplementFullBoardStateCalc(board_map);
  auto initial_state = my_hash_calculator.ImplementGetState();
  my_hash_calculator.ImplementUpdateBoardState(executed_move);
  auto post_move_state = my_hash_calculator.ImplementGetState();
  my_hash_calculator.ImplementUpdateBoardState(executed_move);
  auto final_state = my_hash_calculator.ImplementGetState();

  EXPECT_NE(initial_state, post_move_state);
  EXPECT_EQ(initial_state, final_state);
}

class DualZobristTrackerTest : public ::testing::Test {
protected:
  BoardMap_t board_map = int_board_to_game_pieces(kStartingBoard);
  ZobristCalculator<uint32_t> zobrist_calculator_032_a{78910};
  ZobristCalculator<uint32_t> zobrist_calculator_032_b{910789};

  ZobristCalculator<uint64_t> zobrist_calculator_064_a{654321};
  ZobristCalculator<uint64_t> zobrist_calculator_064_b{321654};

  ZobristCalculator<__uint128_t> zobrist_calculator_128_a{2468};
  ZobristCalculator<__uint128_t> zobrist_calculator_128_b{6824};
};

TEST_F(DualZobristTrackerTest, DefaultInit) {
  auto dual_tracker_032 = DualZobristTracker<uint32_t>();
  auto dual_tracker_064 = DualZobristTracker<uint64_t>();
  auto dual_tracker_128 = DualZobristTracker<__uint128_t>();
}

TEST_F(DualZobristTrackerTest, InitWithExistingCalculators) {
  auto dual_tracker_032 =
      DualZobristTracker<uint32_t>(zobrist_calculator_032_a, zobrist_calculator_032_b);
  auto dual_tracker_064 =
      DualZobristTracker<uint64_t>(zobrist_calculator_064_a, zobrist_calculator_064_b);
  auto dual_tracker_128 =
      DualZobristTracker<__uint128_t>(zobrist_calculator_128_a, zobrist_calculator_128_b);
}

TEST_F(DualZobristTrackerTest, InitFromSeed) {
  DualZobristTracker<uint32_t> dual_tracker_032{20241031};
  DualZobristTracker<uint64_t> dual_tracker_064{20241030};
  DualZobristTracker<__uint128_t> dual_tracker_128{20241029};
}

TEST_F(DualZobristTrackerTest, ExecuteAndUndoMove64) {

  DualZobristTracker<uint64_t> dual_tracker_064{};

  auto start = BoardSpace{6, 0};
  auto end = BoardSpace{5, 0};
  auto move = Move{start, end};
  auto moving_piece = GamePiece{PieceType::kSol, PieceColor::kRed};
  auto destination_piece = GamePiece{PieceType::kNnn, PieceColor::kNul};
  auto executed_move = ExecutedMove{move, moving_piece, destination_piece};

  dual_tracker_064.ImplementFullBoardStateCalc(board_map);
  auto initial_state = dual_tracker_064.ImplementGetState();
  dual_tracker_064.ImplementUpdateBoardState(executed_move);
  auto post_move_state = dual_tracker_064.ImplementGetState();
  dual_tracker_064.ImplementUpdateBoardState(executed_move);
  auto final_state = dual_tracker_064.ImplementGetState();

  EXPECT_NE(initial_state, post_move_state);
  EXPECT_EQ(initial_state, final_state);
}

TEST_F(DualZobristTrackerTest, RecordData) {
  
  DualZobristTracker<uint64_t> dual_tracker_064{};
  
  auto start = BoardSpace{6, 0};
  auto end = BoardSpace{5, 0};
  auto move = Move{start, end};
  auto moving_piece = GamePiece{PieceType::kSol, PieceColor::kRed};
  auto destination_piece = GamePiece{PieceType::kNnn, PieceColor::kNul};
  auto executed_move = ExecutedMove{move, moving_piece, destination_piece};

  dual_tracker_064.ImplementFullBoardStateCalc(board_map);
  auto initial_state = dual_tracker_064.ImplementGetState();

  MoveCollection dummy_move_collection{};
  dummy_move_collection.Append(move);

  EqualScoreMoves dummy_equal_score_moves{1, dummy_move_collection};
  
  dual_tracker_064.RecordTrData(1, MinimaxResultType::kFullyEvaluatedNode, dummy_equal_score_moves);


}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}