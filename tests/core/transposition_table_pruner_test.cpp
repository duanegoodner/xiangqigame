#include <gameboard/game_board.hpp>
#include <gtest/gtest.h>
#include <moveselection/move_evaluators.hpp>
#include <piecepoints/piece_position_points.hpp>
#include <utilities/utility_functs.hpp>
#include <boardstate/zobrist.hpp>

class TranspositionTablePrunerTest : public ::testing::Test {
protected:
  boardstate::TranspositionTable<uint64_t, 1> tr_table_;
  boardstate::TranspositionTableGuard tr_table_guard_;
};

TEST_F(TranspositionTablePrunerTest, Init) {
  boardstate::TranspositionTablePruner<uint64_t, 1> tr_table_pruner{
      tr_table_,
      tr_table_guard_
  };
}