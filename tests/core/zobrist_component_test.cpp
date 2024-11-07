#include <game_board.hpp>
#include <gtest/gtest.h>
#include <zobrist.hpp>

class ZobristComponentTest : public ::testing::Test {
protected:
  boardstate::ZobristCalculator<uint64_t> zobrist_calculator_064_a{};
  boardstate::ZobristCalculator<uint64_t> zobrist_calculator_064_b{};

  BoardMap_t board_map{int_board_to_game_pieces(kStartingBoard)};

  template <typename KeyType, size_t NumCalculators>
  struct CalcFullBoardStateResult {
    std::array<KeyType, NumCalculators> initial_states;
    KeyType initial_primary_state;
    std::array<KeyType, NumCalculators> final_states;
    KeyType final_primary_state;
  };

  template <typename KeyType, size_t NumCalculators>
  CalcFullBoardStateResult<KeyType, NumCalculators> TestCalcFullBoardState() {
    boardstate::ZobristComponent<KeyType, NumCalculators> zobrist_component{};
    auto board_states_array_initial = zobrist_component.GetAllBoardStates();
    auto primary_board_state_initial = zobrist_component.GetPrimaryBoardState();
    zobrist_component.FullBoardStateCalc(board_map);
    auto board_states_array_final = zobrist_component.GetAllBoardStates();
    auto primary_board_state_final = zobrist_component.GetPrimaryBoardState();

    EXPECT_EQ(board_states_array_initial.size(), NumCalculators);
    EXPECT_EQ(primary_board_state_initial, 0);
    EXPECT_EQ(board_states_array_final.size(), NumCalculators);
    EXPECT_NE(primary_board_state_final, 0);

    return CalcFullBoardStateResult<KeyType, NumCalculators>{
        board_states_array_initial,
        primary_board_state_initial,
        board_states_array_final,
        primary_board_state_final
    };
  }
};

TEST_F(ZobristComponentTest, DefaultInit) {
  boardstate::ZobristComponent<uint64_t, 2> component{};
}

TEST_F(ZobristComponentTest, InitFromArrayOfCalculators) {
  std::array<boardstate::ZobristCalculator<uint64_t>, 2> calculators{
      zobrist_calculator_064_a,
      zobrist_calculator_064_b
  };
  boardstate::ZobristComponent<uint64_t, 2> component{calculators};
}

TEST_F(ZobristComponentTest, InitFromArrayOfSeeds) {
  std::array<uint32_t, 2> seeds{1234, 5678};
  boardstate::ZobristComponent<uint64_t, 2> component{seeds};
}

TEST_F(ZobristComponentTest, CalcFullBoardState) {
  auto result_064_02 = TestCalcFullBoardState<uint64_t, 2>();
}

TEST_F(ZobristComponentTest, PrimaryStateHexString) {
  boardstate::ZobristComponent<uint64_t, 2> component{};
  std::cout << component.primary_board_state_hex_str() << std::endl;
  component.FullBoardStateCalc(board_map);
  std::cout << component.primary_board_state_hex_str() << std::endl;
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}