#include <board_data_structs.hpp>
#include <board_map_fixture.hpp>
#include <concept_board_state_calculator.hpp>
#include <concepts>
#include <gtest/gtest.h>
#include <zobrist_calculator_for_concepts.hpp>

class ZobristCalculatorConceptTest : public ::testing::Test {
protected:
  fixtures::BoardMapFixture board_map_fixture_;

  template <typename KeyType>
  void CheckComplianceWithConcept() {
    static_assert(
        BoardStateCalculatorConcept<boardstate::ZobristCalculatorForConcepts<KeyType>>,
        "ZobristCalculator must comply with BoardStateCalculatorConcept"
    );
  }

  template <typename KeyType>
  void TestCreateMethod() {
    auto zobrist_calculator =
        boardstate::ZobristCalculatorForConcepts<KeyType>::Create();
  }

  template <typename KeyType>
  void TestGetters() {
    auto zobrist_calculator =
        boardstate::ZobristCalculatorForConcepts<KeyType>::Create();
    auto initial_board_state = zobrist_calculator->board_state();
    auto seed = zobrist_calculator->seed();

    EXPECT_EQ(initial_board_state, 0);
    EXPECT_NE(seed, 0);
  }

  template <typename KeyType>
  void TestFullBoardStateCalc() {
    auto zobrist_calculator =
        boardstate::ZobristCalculatorForConcepts<KeyType>::Create();
    auto starting_boardmap = board_map_fixture_.starting_boardmap();
    zobrist_calculator->FullBoardStateCalc(starting_boardmap);

    EXPECT_NE(zobrist_calculator->board_state(), 0);
  }

  template <typename KeyType>
  void TestUpdateBoardState() {
    auto zobrist_calculator =
        boardstate::ZobristCalculatorForConcepts<KeyType>::Create();
    zobrist_calculator->FullBoardStateCalc(board_map_fixture_.starting_boardmap());

    gameboard::BoardSpace move_start{6, 0};
    gameboard::BoardSpace move_end{5, 0};

    auto executed_move = board_map_fixture_.GenerateExecutedMove(move_start, move_end);

    auto initial_state = zobrist_calculator->board_state();
    zobrist_calculator->UpdateBoardState(executed_move);
    auto post_move_state = zobrist_calculator->board_state();
    zobrist_calculator->UpdateBoardState(executed_move);
    auto final_state = zobrist_calculator->board_state();

    EXPECT_NE(initial_state, post_move_state);
    EXPECT_EQ(initial_state, final_state);
  }
};

TEST_F(ZobristCalculatorConceptTest, NullCalculatorCompliesWithConcept) {
  static_assert(
      BoardStateCalculatorConcept<NullBoardStateCalculator>,
      "NullBoardStateCalculator must comply with BoardStateCalculatorConcept"
  );
}

TEST_F(ZobristCalculatorConceptTest, CreateNullCalculator) {
  auto null_calculator = NullBoardStateCalculator::Create();
}

TEST_F(ZobristCalculatorConceptTest, ZobristCalculatorCompliesWithConcept) {
  CheckComplianceWithConcept<uint32_t>();
  CheckComplianceWithConcept<uint64_t>();
  CheckComplianceWithConcept<__uint128_t>();
}

TEST_F(ZobristCalculatorConceptTest, CreateCalculator) {
  TestCreateMethod<uint32_t>();
  TestCreateMethod<uint64_t>();
  TestCreateMethod<__uint128_t>();
}

TEST_F(ZobristCalculatorConceptTest, TestGetters) {
  TestGetters<uint32_t>();
  TestGetters<uint64_t>();
  TestGetters<__uint128_t>();
}

TEST_F(ZobristCalculatorConceptTest, FullBoardStateCalc) {
  TestFullBoardStateCalc<uint32_t>();
  TestFullBoardStateCalc<uint64_t>();
  TestFullBoardStateCalc<__uint128_t>();
}

TEST_F(ZobristCalculatorConceptTest, UpdateBoardState) {
  TestUpdateBoardState<uint32_t>();
  TestUpdateBoardState<uint64_t>();
  TestUpdateBoardState<__uint128_t>();
}
