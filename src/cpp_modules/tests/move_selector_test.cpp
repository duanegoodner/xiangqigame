#include "move_selector.hpp"
#include <gtest/gtest.h>


class MoveSelectorTest : public ::testing::Test {

    protected:
    GameBoard game_board_;
    // PiecePointsEvaluator piece_points_evaluator_;

    MoveSelectorTest()
        : game_board_{GameBoard()}
        // , piece_points_evaluator_{PiecePointsEvaluator(DEFAULT_GAME_POINTS)}
        {}; 
};

TEST_F(MoveSelectorTest, SelectInitialMove) {
    RandomMoveSelector move_selector;
    // auto move_selector = MinimaxMoveSelector(piece_points_evaluator_, 4);
    auto cur_moves = game_board_.CalcFinalMovesOf(PieceColor::kRed);
    move_selector.ImplementSelectMove(game_board_, PieceColor::kRed, cur_moves);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}