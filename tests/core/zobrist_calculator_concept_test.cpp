#include <board_data_structs.hpp>
#include <board_map_fixture.hpp>
#include <builders.hpp>
#include <concept_board_state_calculator.hpp>
#include <concept_composite_concepts.hpp>
#include <concept_single_board_state_provider.hpp>
#include <concepts>
#include <gtest/gtest.h>
#include <zobrist_calculator_for_concepts.hpp>

class ZobristCalculatorConceptTest : public ::testing::Test {
protected:
  fixtures::BoardMapFixture board_map_fixture_;
  gameboard::BoardMap_t starting_board_map_ = board_map_fixture_.starting_boardmap();

  template <typename CalculatorType>
  void CheckComplianceWithBoardStateCalculatorConcept() {
    static_assert(
        BoardStateCalculatorConcept<CalculatorType>,
        "ZobristCalculator must comply with BoardStateCalculatorConcept"
    );
  }

  template <typename CalculatorType>
  void CheckComplianceWithSingleBoardStateProviderConcept() {
    static_assert(
        SingleBoardStateProviderConcept<CalculatorType>,
        "ZobristCalculator must comply with SingleBoardStateProviderConcept"
    );
  }

  template <typename CalculatorType>
  void CheckComplianceWithCompositeConcept() {
    static_assert(
        SingleBoardStateProviderAndBoardStateCalculatorConcept<CalculatorType>,
        "ZobristCalculator must comply with SingleBoardStateProviderConcept"
    );
  }

  template <SingleBoardStateProviderConcept C>
  void TestGetters() {
    auto zobrist_calculator = C::Create();
    auto initial_board_state = zobrist_calculator->board_state();
    auto seed = zobrist_calculator->seed();

    EXPECT_EQ(initial_board_state, 0);
    EXPECT_NE(seed, 0);
  }

  template <BoardStateCalculatorConcept C>
  void TestFullBoardStateCalc() {
    auto zobrist_calculator = C::Create();
    zobrist_calculator->FullBoardStateCalc(starting_board_map_);
    EXPECT_NE(zobrist_calculator->board_state(), 0);
  }

  template <BoardStateCalculatorConcept C>
  void TestUpdateBoardState() {
    auto zobrist_calculator = C::Create();
    zobrist_calculator->FullBoardStateCalc(starting_board_map_);

    gameboard::BoardSpace move_start{6, 0};
    gameboard::BoardSpace move_end{5, 0};

    auto executed_move = board_map_fixture_.GenerateOpeningExecutedMove(move_start, move_end);

    auto initial_state = zobrist_calculator->board_state();
    zobrist_calculator->UpdateBoardState(executed_move);
    auto post_move_state = zobrist_calculator->board_state();
    zobrist_calculator->UpdateBoardState(executed_move);
    auto final_state = zobrist_calculator->board_state();

    EXPECT_NE(initial_state, post_move_state);
    EXPECT_EQ(initial_state, final_state);
  }
};

TEST_F(
    ZobristCalculatorConceptTest,
    ZobristCalculatorCompliesWithBoardStateCalculatorConcept
) {
  CheckComplianceWithBoardStateCalculatorConcept<NullBoardStateCalculator>();
  CheckComplianceWithBoardStateCalculatorConcept<
      boardstate::ZobristCalculatorForConcepts<uint32_t>>();
  CheckComplianceWithBoardStateCalculatorConcept<
      boardstate::ZobristCalculatorForConcepts<uint64_t>>();
  CheckComplianceWithBoardStateCalculatorConcept<
      boardstate::ZobristCalculatorForConcepts<__uint128_t>>();
}

TEST_F(
    ZobristCalculatorConceptTest,
    ZobristCalculatorCompliesWithSingleBoardStateProviderConcept
) {
  // Not testing NullBoardStateCalculator here b/c it does not need to comply w/
  // SingleBoardStateProviderConcept (will never build a
  // MultiBoardStateProviderConcept w/ NullBoardStateCalculator)
  CheckComplianceWithSingleBoardStateProviderConcept<
      boardstate::ZobristCalculatorForConcepts<uint32_t>>();
  CheckComplianceWithSingleBoardStateProviderConcept<
      boardstate::ZobristCalculatorForConcepts<uint64_t>>();
  CheckComplianceWithSingleBoardStateProviderConcept<
      boardstate::ZobristCalculatorForConcepts<__uint128_t>>();
}

TEST_F(ZobristCalculatorConceptTest, CompliesWithCompositeConcept) {
  CheckComplianceWithSingleBoardStateProviderConcept<
      boardstate::ZobristCalculatorForConcepts<uint32_t>>();
  CheckComplianceWithSingleBoardStateProviderConcept<
      boardstate::ZobristCalculatorForConcepts<uint64_t>>();
  CheckComplianceWithSingleBoardStateProviderConcept<
      boardstate::ZobristCalculatorForConcepts<__uint128_t>>();
}

TEST_F(ZobristCalculatorConceptTest, CreateCalculator) {
  NullBoardStateCalculator::Create();
  boardstate::ZobristCalculatorForConcepts<uint32_t>::Create();
  boardstate::ZobristCalculatorForConcepts<uint64_t>::Create();
  boardstate::ZobristCalculatorForConcepts<__uint128_t>::Create();
}

TEST_F(ZobristCalculatorConceptTest, TestGetters) {
  TestGetters<boardstate::ZobristCalculatorForConcepts<uint32_t>>();
  TestGetters<boardstate::ZobristCalculatorForConcepts<uint64_t>>();
  TestGetters<boardstate::ZobristCalculatorForConcepts<__uint128_t>>();
}

TEST_F(ZobristCalculatorConceptTest, FullBoardStateCalc) {
  TestFullBoardStateCalc<boardstate::ZobristCalculatorForConcepts<uint32_t>>();
  TestFullBoardStateCalc<boardstate::ZobristCalculatorForConcepts<uint64_t>>();
  TestFullBoardStateCalc<boardstate::ZobristCalculatorForConcepts<__uint128_t>>();
}

TEST_F(ZobristCalculatorConceptTest, UpdateBoardState) {
  TestUpdateBoardState<boardstate::ZobristCalculatorForConcepts<uint32_t>>();
  TestUpdateBoardState<boardstate::ZobristCalculatorForConcepts<uint64_t>>();
  TestUpdateBoardState<boardstate::ZobristCalculatorForConcepts<__uint128_t>>();
}
