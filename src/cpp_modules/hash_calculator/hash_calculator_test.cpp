#include <bits/stdc++.h>
#include <filesystem>
#include <fstream>
#include <gtest/gtest.h>
#include <source_location>
#include <string>
#include <game_board.hpp>
#include <json.hpp>
#include <hash_calculator.hpp>

using namespace board_components;
using namespace nlohmann::json_abi_v3_11_2;
using namespace std;

class HashCalculatorTest : public ::testing::Test {
    protected:
    string keyfile = "zkeys_v01.json";
    BoardMap_t board_map = int_board_to_game_pieces(kStartingBoard);
    string key_file_path = get_sibling_path(keyfile);

};

TEST_F(HashCalculatorTest, InitializeHashCalculator) {
    ifstream input(key_file_path);
    json zsjon = json::parse(input);
    auto my_zorbist = HashCalculator(zsjon);
}

TEST_F(HashCalculatorTest, InitializeBoardState) {
    ifstream input(key_file_path);
    json zsjon = json::parse(input);
    auto my_zorbist = HashCalculator(zsjon);

    unsigned long long board_state{0};
    EXPECT_EQ(board_state, 0);
    board_state = my_zorbist.CalcInitialBoardState(board_map);
    EXPECT_NE(board_state, 0);

    // EXPECT_EQ(sizeof(my_zorbist.board_state()), 8);
}

TEST_F(HashCalculatorTest, ExecuteAndUndoMove) {


    ifstream input(key_file_path);
    json zsjon = json::parse(input);
    auto my_zorbist = HashCalculator(zsjon);

    auto start = BoardSpace{6, 0};
    auto end = BoardSpace{5, 0};
    auto move = Move{start, end};
    auto moving_piece = GamePiece{PieceType::kSol, PieceColor::kRed};
    auto destination_piece = GamePiece{PieceType::kNnn, PieceColor::kNul};
    auto executed_move = ExecutedMove{move, moving_piece, destination_piece};

    auto initial_state = my_zorbist.CalcInitialBoardState(board_map);   
    auto post_move_state = my_zorbist.CalcNewBoardState(executed_move, initial_state);
    auto final_state = my_zorbist.CalcNewBoardState(executed_move, post_move_state);

     EXPECT_NE(initial_state, post_move_state);
     EXPECT_EQ(initial_state, final_state);    
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}