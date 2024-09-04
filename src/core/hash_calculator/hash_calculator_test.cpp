#include <common.hpp>
#include <game_board.hpp>
#include <gtest/gtest.h>
#include <hash_calculator.hpp>
#include <hash_calculator_details.hpp>
// #include <key_generator.hpp>
#include <nlohmann/json.hpp>
// #include <random>
#include <string>

// using namespace board_components;
using namespace nlohmann::json_abi_v3_11_3;
using namespace std;

// class KeyGeneratorTest : public ::testing::Test {};

// TEST_F(KeyGeneratorTest, GenerateKey) {
//   std::random_device rd;
//   std::mt19937_64 gen_64(rd());
//   zkey_t result = KeyGenerator::generate_key<zkey_t>(gen_64);
// }

class ZobristKeysTest : public ::testing::Test {
protected:
  string key_file_path = DEFAULT_ZKEYS_FILEPATH;
  BoardMap_t board_map = int_board_to_game_pieces(kStartingBoard);
};

TEST_F(ZobristKeysTest, DefaultInit) {
  auto zobrist_keys = ZobristKeys();
  EXPECT_NE(0, zobrist_keys.turn_key);
  EXPECT_EQ(2, zobrist_keys.zarray.size());
  EXPECT_EQ(kNumPieceTypeVals, zobrist_keys.zarray[0].size());
  EXPECT_EQ(kNumRanks, zobrist_keys.zarray[0][0].size());
  EXPECT_EQ(kNumFiles, zobrist_keys.zarray[0][0][0].size());
}

TEST_F(ZobristKeysTest, InitFromSeed) {
  auto zobrist_keys = ZobristKeys(123456);
  EXPECT_NE(0, zobrist_keys.turn_key);
  EXPECT_EQ(2, zobrist_keys.zarray.size());
  EXPECT_EQ(kNumPieceTypeVals, zobrist_keys.zarray[0].size());
  EXPECT_EQ(kNumRanks, zobrist_keys.zarray[0][0].size());
  EXPECT_EQ(kNumFiles, zobrist_keys.zarray[0][0][0].size());
}

TEST_F(ZobristKeysTest, ImportJson) {
  auto zkeys_json = utility_functs::import_json(key_file_path);
  auto turn_key = zkeys_json.at("turn_key");
  auto zarray = zkeys_json.at("zarray");
  EXPECT_NE(0, turn_key);
  EXPECT_EQ(2, zarray.size());
  EXPECT_EQ(kNumPieceTypeVals, zarray[0].size());
  EXPECT_EQ(kNumRanks, zarray[0][0].size());
  EXPECT_EQ(kNumFiles, zarray[0][0][0].size());
}

TEST_F(ZobristKeysTest, InitFromJsonObject) {
  auto zkeys_json = utility_functs::import_json(key_file_path);
  auto zobrist_keys = ZobristKeys(zkeys_json);
  EXPECT_NE(0, zobrist_keys.turn_key);
  EXPECT_EQ(2, zobrist_keys.zarray.size());
  EXPECT_EQ(kNumPieceTypeVals, zobrist_keys.zarray[0].size());
  EXPECT_EQ(kNumRanks, zobrist_keys.zarray[0][0].size());
  EXPECT_EQ(kNumFiles, zobrist_keys.zarray[0][0][0].size());
}

TEST_F(ZobristKeysTest, InitFromJsonFile) {
  auto zobrist_keys = ZobristKeys(key_file_path);
  EXPECT_NE(0, zobrist_keys.turn_key);
  EXPECT_EQ(2, zobrist_keys.zarray.size());
  EXPECT_EQ(kNumPieceTypeVals, zobrist_keys.zarray[0].size());
  EXPECT_EQ(kNumRanks, zobrist_keys.zarray[0][0].size());
  EXPECT_EQ(kNumFiles, zobrist_keys.zarray[0][0][0].size());
}

TEST_F(ZobristKeysTest, GetHashValue) {
  auto zkeys_json = utility_functs::import_json(key_file_path);
  auto zobrist_keys = ZobristKeys(zkeys_json);
  EXPECT_EQ(
      zkeys_json.at("zarray"
      )[get_zcolor_index(PieceColor::kBlk)][PieceType::kAdv][0][3],
      zobrist_keys
          .GetHashValue(PieceColor::kBlk, PieceType::kAdv, BoardSpace{0, 3})
  );
}

class HashCalculatorTest : public ::testing::Test {

protected:
  BoardMap_t board_map = int_board_to_game_pieces(kStartingBoard);
  string key_file_path = DEFAULT_ZKEYS_FILEPATH;
  ZobristKeys zobrist_keys = ZobristKeys(key_file_path);
};

TEST_F(HashCalculatorTest, InitializeFromKeysAndMap) {
  auto my_zobrist = HashCalculator(zobrist_keys);
}

TEST_F(HashCalculatorTest, DefaultInit) { auto my_zobrist = HashCalculator(); }

TEST_F(HashCalculatorTest, InitializeBoardState) {
  auto my_zobrist = HashCalculator();
  zkey_t board_state{0};
  EXPECT_EQ(my_zobrist.ImplementGetState(), 0);
  my_zobrist.ImplementCalcInitialBoardState(board_map);
  EXPECT_NE(my_zobrist.ImplementGetState(), 0);
}

TEST_F(HashCalculatorTest, ExecuteAndUndoMove) {

  auto my_zobrist = HashCalculator(zobrist_keys);

  auto start = BoardSpace{6, 0};
  auto end = BoardSpace{5, 0};
  auto move = Move{start, end};
  auto moving_piece = GamePiece{PieceType::kSol, PieceColor::kRed};
  auto destination_piece = GamePiece{PieceType::kNnn, PieceColor::kNul};
  auto executed_move = ExecutedMove{move, moving_piece, destination_piece};

  my_zobrist.ImplementCalcInitialBoardState(board_map);
  auto initial_state = my_zobrist.ImplementGetState();
  my_zobrist.ImplementCalcNewBoardState(executed_move);
  auto post_move_state = my_zobrist.ImplementGetState();
  my_zobrist.ImplementCalcNewBoardState(executed_move);
  auto final_state = my_zobrist.ImplementGetState();

  EXPECT_NE(initial_state, post_move_state);
  EXPECT_EQ(initial_state, final_state);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}