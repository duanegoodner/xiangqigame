
#include <chrono>
#include <gtest/gtest.h>
#include <iostream>
#include <move_selector.hpp>
#include <piece_points.hpp>

using namespace piece_points;

class MoveSelectorTest : public ::testing::Test {

    protected:
    GameBoard game_board_;
    PiecePointsEvaluator piece_points_evaluator_;

    MoveSelectorTest()
        : game_board_{GameBoard()}
        , piece_points_evaluator_{PiecePointsEvaluator(DEFAULT_GAME_POINTS)}
        {}; 
};

// TEST_F(MoveSelectorTest, RandomMoveSelectorInitialMove) {
//     RandomMoveSelector move_selector;
//     auto cur_moves = game_board_.CalcFinalMovesOf(PieceColor::kRed);
//     auto selected_move = move_selector.SelectMove(game_board_, PieceColor::kRed, cur_moves);
//     EXPECT_EQ(cur_moves.Contains(selected_move), true);
// };

TEST_F(MoveSelectorTest, MinimaxMoveSelectorSelectInitialMove) {
    
    auto move_selector = PiecePointsMoveSelector(piece_points_evaluator_, 5);
    // auto start_time = std::chrono::high_resolution_clock::now();
    // auto selected_move = move_selector.SelectMove(game_board_, PieceColor::kRed);
    // auto end_time = std::chrono::high_resolution_clock::now();
    move_selector.SelectMove(game_board_, PieceColor::kRed);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}