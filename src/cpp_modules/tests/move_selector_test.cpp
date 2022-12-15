#include "move_selector.hpp"
#include <chrono>
#include <gtest/gtest.h>
#include <iostream>


class MoveSelectorTest : public ::testing::Test {

    protected:
    GameBoard game_board_;
    PiecePointsEvaluator piece_points_evaluator_;

    MoveSelectorTest()
        : game_board_{GameBoard()}
        , piece_points_evaluator_{PiecePointsEvaluator(DEFAULT_GAME_POINTS)}
        {}; 
};

TEST_F(MoveSelectorTest, RandomMoveSelectorInitialMove) {
    RandomMoveSelector move_selector;
    auto cur_moves = game_board_.CalcFinalMovesOf(PieceColor::kRed);
    auto selected_move = move_selector.ImplementSelectMove(game_board_, PieceColor::kRed, cur_moves);
    EXPECT_EQ(cur_moves.Contains(selected_move), true);
};

TEST_F(MoveSelectorTest, MinimaxMoveSelectorSelectInitialMove) {
    auto move_selector = MinimaxMoveSelector(piece_points_evaluator_, 4);
    auto cur_moves = game_board_.CalcFinalMovesOf(PieceColor::kRed);
    auto start_time = std::chrono::high_resolution_clock::now();
    auto selected_move = move_selector.ImplementSelectMove(game_board_, PieceColor::kRed, cur_moves);
    
    auto end_time = std::chrono::high_resolution_clock::now();
    std::cout << "selected move start: "
        <<  selected_move.start.rank
        << ", "
        << selected_move.start.file
        << std::endl;
    std::cout << "selected move end: "
        << selected_move.end.rank
        << ", "
        << selected_move.end.file
        << std::endl;
    std::cout << "search time: "
        << std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count()
        << std::endl;
    
    EXPECT_EQ(cur_moves.Contains(selected_move), true);

}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}