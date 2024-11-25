#include <game_board.hpp>
#include <gtest/gtest.h>
#include <zobrist.hpp>

class ZobristComponentTest : public ::testing::Test {
protected:
  boardstate::ZobristCalculator<uint64_t> zobrist_calculator_064_a{};
  boardstate::ZobristCalculator<uint64_t> zobrist_calculator_064_b{};

  BoardMap_t board_map{int_board_to_game_pieces(kStandardInitialBoard)};

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

TEST_F(ZobristComponentTest, InitFromSingleSeed) {
  boardstate::ZobristComponent<uint64_t, 1> component_a{1234};
  boardstate::ZobristComponent<uint64_t, 1> component_b{1234};

  component_a.FullBoardStateCalc(board_map);
  component_b.FullBoardStateCalc(board_map);

  EXPECT_EQ(component_a.primary_board_state(), component_b.primary_board_state());
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

class ZobristComponentNewTest : public ::testing::Test {
protected:
  boardstate::ZobristCalculator<uint64_t> zobrist_calculator_064_a{};
  boardstate::ZobristCalculator<uint64_t> zobrist_calculator_064_b{};
  std::array<boardstate::ZobristCalculator<uint64_t>, 1> confirmation_calculators{
      zobrist_calculator_064_b
  };

  BoardMap_t board_map{int_board_to_game_pieces(kStandardInitialBoard)};

  template <typename KeyType, size_t NumCalculators>
  struct CalcFullBoardStateResult {
    std::array<KeyType, NumCalculators> initial_states;
    KeyType initial_primary_state;
    std::array<KeyType, NumCalculators> final_states;
    KeyType final_primary_state;
  };

  template <typename KeyType, size_t NumCalculators>
  CalcFullBoardStateResult<KeyType, NumCalculators> TestCalcFullBoardState() {
    boardstate::ZobristComponentNew<KeyType, NumCalculators> zobrist_component{
        zobrist_calculator_064_a,
        confirmation_calculators
    };
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

TEST_F(ZobristComponentNewTest, Init) {

  boardstate::ZobristComponentNew<uint64_t, 1> zobrist_component_new{
      zobrist_calculator_064_a,
      confirmation_calculators
  };
}

TEST_F(ZobristComponentNewTest, CalcFullBoardState) {
  auto result = TestCalcFullBoardState<uint64_t, 1>();
}

TEST_F(ZobristComponentNewTest, PrimaryStateHexString) {
  boardstate::ZobristComponentNew<uint64_t, 1> component{
      zobrist_calculator_064_a,
      confirmation_calculators
  };
  std::cout << component.primary_board_state_hex_str() << std::endl;
  component.FullBoardStateCalc(board_map);
  std::cout << component.primary_board_state_hex_str() << std::endl;
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}