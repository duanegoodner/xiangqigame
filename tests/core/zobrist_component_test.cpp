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
    auto confirmation_states_initial = zobrist_component.confirmation_board_states();
    auto primary_board_state_initial = zobrist_component.primary_board_state();
    zobrist_component.FullBoardStateCalc(board_map);
    auto confirmation_states_final = zobrist_component.confirmation_board_states();
    auto primary_board_state_final = zobrist_component.primary_board_state();

    EXPECT_EQ(confirmation_states_initial.size(), NumCalculators);
    EXPECT_EQ(primary_board_state_initial, 0);
    EXPECT_EQ(confirmation_states_final.size(), NumCalculators);
    EXPECT_NE(primary_board_state_final, 0);

    return CalcFullBoardStateResult<KeyType, NumCalculators>{
        confirmation_states_initial,
        primary_board_state_initial,
        confirmation_states_final,
        primary_board_state_final
    };
  }
};

TEST_F(ZobristComponentTest, DefaultInit) {
  boardstate::ZobristComponent<uint64_t, 1> component{};
}

TEST_F(ZobristComponentTest, InitFromCalculators) {
  boardstate::ZobristComponent<uint64_t, 1> component{
      zobrist_calculator_064_a,
      std::array<boardstate::ZobristCalculator<uint64_t>, 1>{zobrist_calculator_064_b}
  };
}

TEST_F(ZobristComponentTest, InitFromASeeds) {
  uint32_t primary_seed{1234};
  std::array<uint32_t, 1> seeds{5678};
  boardstate::ZobristComponent<uint64_t, 1> component{primary_seed, seeds};
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