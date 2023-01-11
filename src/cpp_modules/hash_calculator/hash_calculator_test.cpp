#include "common.hpp"
#include <bits/stdc++.h>
#include <filesystem>
#include <fstream>
#include <game_board.hpp>
#include <gtest/gtest.h>
#include <hash_calculator.hpp>
#include <json.hpp>
#include <source_location>
#include <string>

using namespace board_components;
using namespace nlohmann::json_abi_v3_11_2;
using namespace std;

class ZobristKeysTest : public ::testing::Test {
    protected:
    string keyfile = "zkeys_v01.json";
    string key_file_path = get_sibling_path(keyfile);
    BoardMap_t board_map = int_board_to_game_pieces(kStartingBoard);
};

TEST_F(ZobristKeysTest, ImportJson) {
    auto zkeys_json = import_json(key_file_path);
    auto turn_key = zkeys_json.at("turn_key");
    auto zarray = zkeys_json.at("zarray");
    EXPECT_NE(0, turn_key);
    EXPECT_EQ(2, zarray.size());
    EXPECT_EQ(kNumPieceTypeVals, zarray[0].size());
    EXPECT_EQ(kNumRanks, zarray[0][0].size());
    EXPECT_EQ(kNumFiles, zarray[0][0][0].size());
    // auto Zobrist_keys = ZobristKeys(zkeys_json);
}

TEST_F(ZobristKeysTest, InitFromJsonObject) {
    auto zkeys_json = import_json(key_file_path);
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

TEST_F(ZobristKeysTest, CalcInitialBoardState) {
    auto zobrist_keys = ZobristKeys(key_file_path);
    auto initial_board_state = zobrist_keys.CalcInitialBoardState(board_map);
    EXPECT_NE(initial_board_state, 0);
}

TEST_F(ZobristKeysTest, GetHashValue) {
    auto zkeys_json = import_json(key_file_path);
    auto zobrist_keys = ZobristKeys(zkeys_json);
    EXPECT_EQ(
        zkeys_json.at("zarray")[get_zcolor_index(PieceColor::kBlk)][PieceType::kAdv][0][3],
        zobrist_keys.GetHashValue(PieceColor::kBlk, PieceType::kAdv, BoardSpace{0, 3})
        );
}

class HashCalculatorTest : public ::testing::Test {

  protected:
    string keyfile = "zkeys_v01.json";
    BoardMap_t board_map = int_board_to_game_pieces(kStartingBoard);
    string key_file_path = get_sibling_path(keyfile);
    ZobristKeys zobrist_keys = ZobristKeys(key_file_path);
};

TEST_F(HashCalculatorTest, InitializeFromKeysAndMap) {   
    auto my_zobrist = HashCalculator(zobrist_keys, board_map);
}

TEST_F(HashCalculatorTest, InitializeFromMap) {   
    auto my_zobrist = HashCalculator(board_map);
}

// TEST_F(HashCalculatorTest, InitializeBoardState) {
//     // ifstream input(key_file_path);
//     // json zsjon = json::parse(input);
//     auto my_zobrist = HashCalculator(zobrist_keys, board_map);

//     zkey_t board_state{0};
//     EXPECT_EQ(board_state, 0);
//     board_state = my_zobrist.CalcInitialBoardState(board_map);
//     EXPECT_NE(board_state, 0);
// }

TEST_F(HashCalculatorTest, ExecuteAndUndoMove) {

    auto my_zobrist = HashCalculator(zobrist_keys, board_map);

    auto start = BoardSpace{6, 0};
    auto end = BoardSpace{5, 0};
    auto move = Move{start, end};
    auto moving_piece = GamePiece{PieceType::kSol, PieceColor::kRed};
    auto destination_piece = GamePiece{PieceType::kNnn, PieceColor::kNul};
    auto executed_move = ExecutedMove{move, moving_piece, destination_piece};

    // auto initial_state = my_zobrist.CalcInitialBoardState(board_map);
    // auto post_move_state =
    //     my_zobrist.CalcNewBoardState(executed_move, initial_state);
    // auto final_state =
    //     my_zobrist.CalcNewBoardState(executed_move, post_move_state);

    // EXPECT_NE(initial_state, post_move_state);
    // EXPECT_EQ(initial_state, final_state);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}