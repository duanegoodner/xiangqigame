#include <gtest/gtest.h>

#include "board_components.hpp"
#include "board_utilities_free.hpp"
#include "move_calculator.hpp"
#include "game_board.hpp"

using namespace board_utilities_free;

const BoardMap_t kFlyingBoard{{
    {5, 4, 3, 2, 1, 2, 3, 4, 5},
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 6, 0, 0, 0, 0, 0, 6, 0},
    {7, 0, 7, 0, 0, 0, 7, 0, 7},
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {-7, 0, -7, 0, 0, 0, -7, 0, -7},
    {0, -6, 0, 0, 0, 0, 0, -6, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {-5, -4, -3, -2, -1, -2, -3, -4, -5},
}};

class MoveCalculatorTest : public ::testing::Test {
    

   protected:
    BoardMap_t starting_board = kStartingBoard;
    BoardMap_t flying_board = kFlyingBoard;
    MoveCalculator mc_standard_;
    MoveCalculator mc_flying_;
    MoveCalculatorTest()
        : mc_standard_{MoveCalculator(starting_board)},
          mc_flying_{MoveCalculator{flying_board}} {}
};

TEST_F(MoveCalculatorTest, RedSoldier) {
    auto soldier_moves = mc_standard_.CalcMovesFrom(BoardSpace{6, 0});
    EXPECT_EQ(soldier_moves.moves.size(), 1);
    
}

TEST_F(MoveCalculatorTest, BlackCannon) {
    auto cannon_moves = mc_standard_.CalcMovesFrom(BoardSpace{2, 1});
    EXPECT_EQ(cannon_moves.moves.size(), 12);
}

TEST_F(MoveCalculatorTest, BlackChariot) {
    auto chariot_moves = mc_standard_.CalcMovesFrom(BoardSpace{0, 8});
    EXPECT_EQ(chariot_moves.moves.size(), 2);
}

TEST_F(MoveCalculatorTest, RedHorse) {
    auto horse_moves = mc_standard_.CalcMovesFrom(BoardSpace{9, 1});
    EXPECT_EQ(horse_moves.moves.size(), 2);
}

TEST_F(MoveCalculatorTest, BlackElephant) {
    auto elephant_moves = mc_standard_.CalcMovesFrom({0, 2});
    EXPECT_EQ(elephant_moves.moves.size(), 2);
}

TEST_F(MoveCalculatorTest, RedAdvisor) {
    auto advisor_moves = mc_standard_.CalcMovesFrom({9, 3});
    EXPECT_EQ(advisor_moves.moves.size(), 1);
}

TEST_F(MoveCalculatorTest, BlackStartingGeneral) {
    auto general_moves = mc_standard_.CalcMovesFrom(BoardSpace{0, 4});
    EXPECT_EQ(general_moves.moves.size(), 1);
}

TEST_F(MoveCalculatorTest, RedFlyingGeneral) {
    auto general_moves_with_flying = mc_flying_.CalcMovesFrom(BoardSpace{9, 4});
    EXPECT_EQ(general_moves_with_flying.moves.size(), 2);
}

TEST_F(MoveCalculatorTest, AllMovesNoCheckTest) {
    auto red_moves_without_check_test = mc_standard_.CalcAllMovesNoCheckTest(PieceColor::kRed);
    EXPECT_EQ(red_moves_without_check_test.moves.size(), 44);
}

class BoardUtilitiesTest : public ::testing::Test {
    

   protected:
    BoardMap_t starting_board = kStartingBoard;
    MoveCalculator mc_standard_;
    BoardUtilitiesTest()
        : mc_standard_{MoveCalculator(starting_board)} {}
};

TEST_F(BoardUtilitiesTest, IsOccupiedTrue) {
    auto result = is_occupied(starting_board, BoardSpace{0, 0});
    EXPECT_EQ(result, true);
}

TEST_F(BoardUtilitiesTest, IsOccupiedFalse) {
    auto result = is_occupied(starting_board, BoardSpace{1, 1});
    EXPECT_EQ(result, false);
}

TEST_F(BoardUtilitiesTest, GetColorBlack) {
    auto result = get_color(starting_board, BoardSpace{0, 0});
    EXPECT_EQ(result, PieceColor::kBlk);
}

TEST_F(BoardUtilitiesTest, GetColorEmpty) {
    auto result = get_color(starting_board, BoardSpace{1, 0});
    EXPECT_EQ(result, PieceColor::kNul);
}

TEST_F(BoardUtilitiesTest, GetTypeElephant) {
    auto result = get_type(starting_board, BoardSpace{9, 2});
    EXPECT_EQ(result, PieceType::kEle);
}

TEST_F(BoardUtilitiesTest, GetTypeEmpty) {
    auto result = get_type(starting_board, BoardSpace{5, 1});
    EXPECT_EQ(result, PieceType::kNnn);
}

TEST_F(BoardUtilitiesTest, ExistsAndPassesColorTestTrue) {
    auto result = exists_and_passes_color_test(
        starting_board, BoardSpace{3, 2}, PieceColor::kRed);
    EXPECT_EQ(result, true);
}

TEST_F(BoardUtilitiesTest, ExistsAndPassesColorTestFalseOffBoard) {
    auto result = exists_and_passes_color_test(
        starting_board, BoardSpace{100, 1}, PieceColor::kRed);
    EXPECT_EQ(result, false);
}

TEST_F(BoardUtilitiesTest, ExistsAndPassesColorTestFalseSameColor) {
    auto result = exists_and_passes_color_test(
        starting_board, BoardSpace{6, 0}, PieceColor::kRed);
    EXPECT_EQ(result, false);
}

TEST_F(BoardUtilitiesTest, FwdDirectionRed) {
    auto result =fwd_direction(PieceColor::kRed);
    EXPECT_EQ(result.rank, -1);
    EXPECT_EQ(result.file, 0);
}

TEST_F(BoardUtilitiesTest, RevDirectionRed) {
    auto result = rev_direction(PieceColor::kRed);
    EXPECT_EQ(result.rank, 1);
    EXPECT_EQ(result.file, 0);
}

TEST_F(BoardUtilitiesTest, GetGeneralPositionBlack) {
    auto result = get_general_position(starting_board, PieceColor::kBlk);
    EXPECT_EQ(result.rank, 0);
    EXPECT_EQ(result.file, 4);
}

TEST_F(BoardUtilitiesTest, SearchSpacesRedChariot) {
    auto result = search_spaces(starting_board, BoardSpace{9, 8}, BoardDirection{-1, 0});
    EXPECT_EQ(result.empty_spaces.size(), 2);
    EXPECT_EQ(result.first_occupied_space[0].rank, 6);
    EXPECT_EQ(result.first_occupied_space[0].file, 8);
}

TEST_F(BoardUtilitiesTest, GetAllSpacesOccupiedByBlack) {
    auto result = get_all_spaces_occupied_by(starting_board, PieceColor::kBlk);
    EXPECT_EQ(result.size(), 16);
}

TEST_F(BoardUtilitiesTest, IsSpaceDestinationOfAnyMovesTrue) {
    auto some_moves = MoveCollection({Move{BoardSpace{2, 1}, BoardSpace{9, 1}}});
    auto some_space = BoardSpace{9, 1};
    auto result = is_space_any_destination_of_moves(some_space, some_moves);
    EXPECT_EQ(result, true);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

