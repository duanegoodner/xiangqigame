#include <game_board.hpp>
#include <gtest/gtest.h>
#include <zobrist.hpp>

class ZobristCalculatorTest : public ::testing::Test {
protected:
  gameboard::BoardMap_t board_map = int_board_to_game_pieces(gameboard::kStartingBoard);

  template <typename KeyType>
  void ValidateZobristCalculator(
      boardstate::ZobristCalculator<KeyType> zobrist_calculator
  ) {
    EXPECT_NE(0, zobrist_calculator.turn_key());
  }

  template <typename KeyType>
  void GetBlackAdvisor_0_3_HashValue(
      boardstate::ZobristCalculator<KeyType> zobrist_calculator
  ) {
    auto sample_key_value = zobrist_calculator.GetHashValueAt(
        PieceColor::kBlk,
        PieceType::kAdv,
        BoardSpace{0, 3}
    );
  }

  template <typename KeyType>
  void CalcFullBoardState(boardstate::ZobristCalculator<KeyType> zobrist_calculator) {
    auto board_state_initial = zobrist_calculator.board_state();
    EXPECT_EQ(board_state_initial, 0);
    zobrist_calculator.FullBoardStateCalc(board_map);
    auto board_state_final = zobrist_calculator.board_state();
    EXPECT_NE(board_state_initial, board_state_final);
  }

  gameboard::BoardSpace test_move_start{6, 0};
  gameboard::BoardSpace test_move_end{5, 0};
  gameboard::Move test_move{test_move_start, test_move_end};
  gameboard::GamePiece moving_piece{
      gameboard::PieceType::kSol,
      gameboard::PieceColor::kRed
  };
  gameboard::GamePiece destination_piece{
      gameboard::PieceType::kNnn,
      gameboard::PieceColor::kNul
  };
  gameboard::ExecutedMove executed_move{test_move, moving_piece, destination_piece};

  template <typename KeyType>
  void CalcFullBoardStateAndUpdate(
      boardstate::ZobristCalculator<KeyType> zobrist_calculator
  ) {
    CalcFullBoardState(zobrist_calculator);
    auto initial_state = zobrist_calculator.board_state();
    zobrist_calculator.UpdateBoardState(executed_move);
    auto post_move_state = zobrist_calculator.board_state();
    zobrist_calculator.UpdateBoardState(executed_move);
    auto final_state = zobrist_calculator.board_state();

    EXPECT_NE(initial_state, post_move_state);
    EXPECT_EQ(initial_state, final_state);
  }
};

TEST_F(ZobristCalculatorTest, DefaultInit) {
  auto zobrist_keys_032 = boardstate::ZobristCalculator<uint32_t>{};
  ValidateZobristCalculator<uint32_t>(zobrist_keys_032);

  auto zobrist_keys_064 = boardstate::ZobristCalculator<uint64_t>{};
  ValidateZobristCalculator<uint64_t>(zobrist_keys_064);

  auto zobrist_keys_128 = boardstate::ZobristCalculator<__uint128_t>{};
  ValidateZobristCalculator<__uint128_t>(zobrist_keys_128);
}

TEST_F(ZobristCalculatorTest, InitFromSeed) {
  auto zobrist_keys_032 = boardstate::ZobristCalculator<uint32_t>{123456};
  ValidateZobristCalculator<uint32_t>(zobrist_keys_032);

  auto zobrist_keys_064 = boardstate::ZobristCalculator<uint64_t>{123456};
  ValidateZobristCalculator<uint64_t>(zobrist_keys_064);

  auto zobrist_keys_128 = boardstate::ZobristCalculator<__uint128_t>{123456};
  ValidateZobristCalculator<__uint128_t>(zobrist_keys_128);
}

TEST_F(ZobristCalculatorTest, GetBlackAdvisor_0_3_HashValue) {
  auto zobrist_keys_032 = boardstate::ZobristCalculator<uint32_t>{};
  GetBlackAdvisor_0_3_HashValue(zobrist_keys_032);

  auto zobrist_keys_064 = boardstate::ZobristCalculator<uint64_t>{};
  GetBlackAdvisor_0_3_HashValue(zobrist_keys_064);

  auto zobrist_keys_128 = boardstate::ZobristCalculator<__uint128_t>{};
  GetBlackAdvisor_0_3_HashValue(zobrist_keys_128);
}

TEST_F(ZobristCalculatorTest, FullBoardStateCalc) {
  auto zobrist_keys_032 = boardstate::ZobristCalculator<uint32_t>{};
  CalcFullBoardState(zobrist_keys_032);

  auto zobrist_keys_064 = boardstate::ZobristCalculator<uint64_t>{};
  CalcFullBoardState(zobrist_keys_064);

  auto zobrist_keys_128 = boardstate::ZobristCalculator<__uint128_t>{};
  CalcFullBoardState(zobrist_keys_128);
}

TEST_F(ZobristCalculatorTest, UpdateBoardState) {
  auto zobrist_keys_032 = boardstate::ZobristCalculator<uint32_t>{};
  CalcFullBoardStateAndUpdate(zobrist_keys_032);

  auto zobrist_keys_064 = boardstate::ZobristCalculator<uint64_t>{};
  CalcFullBoardStateAndUpdate(zobrist_keys_064);

  auto zobrist_keys_128 = boardstate::ZobristCalculator<__uint128_t>{};
  CalcFullBoardStateAndUpdate(zobrist_keys_128);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}