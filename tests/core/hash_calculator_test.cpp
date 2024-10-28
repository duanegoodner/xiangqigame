// #include <common.hpp>
#include <game_board.hpp>
#include <gtest/gtest.h>
#include <hash_calculator.hpp>
#include <nlohmann/json.hpp>
#include <string>

using namespace boardstate;
using namespace nlohmann::json_abi_v3_11_3;
using namespace std;

class ZobristKeysTest : public ::testing::Test {
protected:
  BoardMap_t board_map = int_board_to_game_pieces(kStartingBoard);
};

TEST_F(ZobristKeysTest, DefaultInit) {
  auto zobrist_keys = ZobristKeys<uint64_t>(123456);
  EXPECT_NE(0, zobrist_keys.turn_key());
  EXPECT_EQ(2, zobrist_keys.zarray().size());
  EXPECT_EQ(kNumPieceTypeVals, zobrist_keys.zarray()[0].size());
  EXPECT_EQ(kNumRanks, zobrist_keys.zarray()[0][0].size());
  EXPECT_EQ(kNumFiles, zobrist_keys.zarray()[0][0][0].size());
}

TEST_F(ZobristKeysTest, InitFromSeed) {
  auto zobrist_keys = ZobristKeys<uint64_t>(123456);
  EXPECT_NE(0, zobrist_keys.turn_key());
  EXPECT_EQ(2, zobrist_keys.zarray().size());
  EXPECT_EQ(kNumPieceTypeVals, zobrist_keys.zarray()[0].size());
  EXPECT_EQ(kNumRanks, zobrist_keys.zarray()[0][0].size());
  EXPECT_EQ(kNumFiles, zobrist_keys.zarray()[0][0][0].size());
}

TEST_F(ZobristKeysTest, GetHashValueAt) {
  auto zobrist_keys = ZobristKeys<uint64_t>(1234567);
  auto sample_key_value = zobrist_keys.GetHashValueAt(
      PieceColor::kBlk,
      PieceType::kAdv,
      BoardSpace{0, 3}
  );
}

class HashCalculatorTest : public ::testing::Test {

protected:
  BoardMap_t board_map = int_board_to_game_pieces(kStartingBoard);
  ZobristKeys<uint64_t> zobrist_keys{654321};
  ZobristKeys<__uint128_t> zobrist_keys_128{2468};
};

TEST_F(HashCalculatorTest, InitializeFromKeysAndMap) {
  auto my_hash_calculator = HashCalculator<uint64_t>(zobrist_keys);
  auto my_hash_calculator_128 = HashCalculator<__uint128_t>(zobrist_keys_128);
}

TEST_F(HashCalculatorTest, DefaultInit) {
  auto my_hash_calculator = HashCalculator<uint64_t>(3579);
  auto my_hash_calculator_128 = HashCalculator<__uint128_t>(8675309);
}

TEST_F(HashCalculatorTest, InitializeBoardState) {
  auto my_hash_calculator = HashCalculator<uint64_t>(22443355);
  uint64_t board_state{0};
  EXPECT_EQ(my_hash_calculator.ImplementGetState(), 0);
  my_hash_calculator.ImplementFullBoardStateCalc(board_map);
  EXPECT_NE(my_hash_calculator.ImplementGetState(), 0);
}

TEST_F(HashCalculatorTest, ExecuteAndUndoMove64) {

  auto my_hash_calculator = HashCalculator<uint64_t>(zobrist_keys);

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

TEST_F(HashCalculatorTest, ExecuteAndUndoMove128) {

  auto my_hash_calculator = HashCalculator<__uint128_t>(zobrist_keys_128);

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

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}