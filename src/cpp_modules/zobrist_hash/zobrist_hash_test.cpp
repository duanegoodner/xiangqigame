#include <bits/stdc++.h>
#include <filesystem>
#include <fstream>
#include <gtest/gtest.h>
#include <source_location>
#include <string>
#include <game_board.hpp>
#include <json.hpp>
#include <zobrist_hash.hpp>

using namespace board_components;
using namespace nlohmann::json_abi_v3_11_2;
using namespace std;

class ZobristHashTest : public ::testing::Test {
    protected:
    string keyfile = "zkeys_v01.json";
    BoardMap_t board_map = int_board_to_game_pieces(kStartingBoard);
    string key_file_path = get_sibling_path(keyfile);

};

TEST_F(ZobristHashTest, Init) {
    ifstream input(key_file_path);
    json zsjon = json::parse(input);
    auto my_zorbist = ZobristHash(zsjon, board_map);
    EXPECT_EQ(sizeof(my_zorbist.board_state()), 8);
}

TEST_F(ZobristHashTest, ExecuteAndUndoMove) {
    auto game_board = GameBoard();

    ifstream input(key_file_path);
    json zsjon = json::parse(input);
    auto my_zorbist = ZobristHash(zsjon, board_map);
    auto starting_hash_val = my_zorbist.board_state();
    
    auto red_moves = game_board.CalcFinalMovesOf(PieceColor::kRed);
    auto executed_move = game_board.ExecuteMove(red_moves.moves[0]);
    my_zorbist.UpdateBoardState(executed_move);
    auto post_move_hash_val = my_zorbist.board_state();

    game_board.UndoMove(executed_move);
    my_zorbist.UpdateBoardState(executed_move);
    auto post_undo_move_hash_val = my_zorbist.board_state();

    EXPECT_EQ(starting_hash_val, post_undo_move_hash_val);    
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}