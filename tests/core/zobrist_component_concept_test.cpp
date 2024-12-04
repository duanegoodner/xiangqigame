#include <board_map_fixture.hpp>
#include <gtest/gtest.h>
#include <zobrist_calculator_for_concepts.hpp>
#include <zobrist_component_with_exposed_calculators_fixture.hpp>
#include <zobrist_for_concepts.hpp>

class ZobristComponentConceptTest : public ::testing::Test {
protected:
  fixtures::BoardMapFixture board_map_fixture_;

  template <SingleBoardStateProviderConcept C, size_t NumConfKeys>
  std::shared_ptr<boardstate::ZobristComponentForConcepts<C, NumConfKeys>>
  BuildComponentFromSeed() {
    return boardstate::ZobristComponentForConcepts<C, NumConfKeys>::CreateFromSeed(
    );
  }

  template <SingleBoardStateProviderConcept C, size_t NumConfKeys>
  void TestCreateFromSeed() {
    auto zobrist_component =
        boardstate::ZobristComponentForConcepts<C, NumConfKeys>::CreateFromSeed();
  }

  template <SingleBoardStateProviderConcept C, size_t NumConfKeys>
  void TestCreateFromExistingCalculators() {
    auto primary_calculator =
        C::Create();
    std::array<
        std::shared_ptr<C>,
        NumConfKeys>
        confirmation_calculators;
    for (auto calculator : confirmation_calculators) {
      calculator = C::Create();
    }
    auto zobrist_component =
        boardstate::ZobristComponentForConcepts<C, NumConfKeys>::
            CreateFromCalculators(primary_calculator, confirmation_calculators);
  }

  template <typename KeyType, size_t NumConfKeys>
  void TestZobristCoordinatorWithExposedCalculators() {
    auto zobrist_component_with_exposed_calculators =
        fixtures::ZobristCoordinatorWithExposedCalculators<KeyType, NumConfKeys>::Create();
  }

  template <SingleBoardStateProviderConcept C, size_t NumConfKeys>
  void TestGetters() {
    auto zobrist_component = BuildComponentFromSeed<C, NumConfKeys>();
    auto primary_board_state = zobrist_component->primary_board_state();
    auto confirmation_board_states = zobrist_component->confirmation_board_states();
    auto prng_seed = zobrist_component->prng_seed();

    EXPECT_EQ(primary_board_state, 0);
    for (auto confirmation_state : confirmation_board_states) {
      EXPECT_EQ(confirmation_state, 0);
    }
    EXPECT_NE(prng_seed, 0);
  }

  template <SingleBoardStateProviderConcept C, size_t NumConfKeys>
  void TestNumConfKeys() {
    auto zobrist_component = BuildComponentFromSeed<C, NumConfKeys>();
    EXPECT_EQ(zobrist_component->confirmation_board_states().size(), NumConfKeys);
  }

  template <typename KeyType, size_t NumConfKeys>
  void TestFullBoardStateCalc() {

    // crate object with ZobristComponent and its calculator epxposed
    auto accessible_component =
        fixtures::ZobristCoordinatorWithExposedCalculators<KeyType, NumConfKeys>::Create();
    accessible_component->TestComponentFullBoardStateCalc();
  }

  template <typename KeyType, size_t NumConfKeys>
  void TestUpdateBoardState() {
    // crate object with ZobristComponent and its calculator epxposed
    auto accessible_component =
        fixtures::ZobristCoordinatorWithExposedCalculators<KeyType, NumConfKeys>::Create();
    accessible_component->TestComponentUpdateBoardState();
  }
};

TEST_F(ZobristComponentConceptTest, TestCreateFromSeed) {
  TestCreateFromSeed<boardstate::ZobristCalculatorForConcepts<uint32_t>, 0>();
  TestCreateFromSeed<boardstate::ZobristCalculatorForConcepts<uint64_t>, 0>();
  TestCreateFromSeed<boardstate::ZobristCalculatorForConcepts<__uint128_t>, 0>();
  TestCreateFromSeed<boardstate::ZobristCalculatorForConcepts<uint32_t>, 1>();
  TestCreateFromSeed<boardstate::ZobristCalculatorForConcepts<uint64_t>, 1>();
  TestCreateFromSeed<boardstate::ZobristCalculatorForConcepts<__uint128_t>, 1>();
}

TEST_F(ZobristComponentConceptTest, TestCreateFromExistingCalculators) {
  TestCreateFromExistingCalculators<boardstate::ZobristCalculatorForConcepts<uint32_t>, 0>();
  TestCreateFromExistingCalculators<boardstate::ZobristCalculatorForConcepts<uint64_t>, 0>();
  TestCreateFromExistingCalculators<boardstate::ZobristCalculatorForConcepts<__uint128_t>, 0>();
  TestCreateFromExistingCalculators<boardstate::ZobristCalculatorForConcepts<uint32_t>, 1>();
  TestCreateFromExistingCalculators<boardstate::ZobristCalculatorForConcepts<uint64_t>, 1>();
  TestCreateFromExistingCalculators<boardstate::ZobristCalculatorForConcepts<__uint128_t>, 1>();
}

TEST_F(ZobristComponentConceptTest, TestGetters) {
  TestGetters<boardstate::ZobristCalculatorForConcepts<uint32_t>, 0>();
  TestGetters<boardstate::ZobristCalculatorForConcepts<uint64_t>, 0>();
  TestGetters<boardstate::ZobristCalculatorForConcepts<__uint128_t>, 0>();
  TestGetters<boardstate::ZobristCalculatorForConcepts<uint32_t>, 1>();
  TestGetters<boardstate::ZobristCalculatorForConcepts<uint64_t>, 1>();
  TestGetters<boardstate::ZobristCalculatorForConcepts<__uint128_t>, 1>();
}

TEST_F(ZobristComponentConceptTest, TestNumConfKeys) {
  TestNumConfKeys<boardstate::ZobristCalculatorForConcepts<uint32_t>, 0>();
  TestNumConfKeys<boardstate::ZobristCalculatorForConcepts<uint64_t>, 0>();
  TestNumConfKeys<boardstate::ZobristCalculatorForConcepts<__uint128_t>, 0>();
  TestNumConfKeys<boardstate::ZobristCalculatorForConcepts<uint32_t>, 1>();
  TestNumConfKeys<boardstate::ZobristCalculatorForConcepts<uint64_t>, 1>();
  TestNumConfKeys<boardstate::ZobristCalculatorForConcepts<__uint128_t>, 1>();
}

TEST_F(ZobristComponentConceptTest, TesFullBoardStateCalc) {
  TestFullBoardStateCalc<uint32_t, 0>();
  TestFullBoardStateCalc<uint64_t, 0>();
  TestFullBoardStateCalc<__uint128_t, 0>();
  TestFullBoardStateCalc<uint32_t, 1>();
  TestFullBoardStateCalc<uint64_t, 1>();
  TestFullBoardStateCalc<__uint128_t, 1>();
}

TEST_F(ZobristComponentConceptTest, TesUpdateBoardState) {
  TestUpdateBoardState<uint32_t, 0>();
  TestUpdateBoardState<uint64_t, 0>();
  TestUpdateBoardState<__uint128_t, 0>();
  TestUpdateBoardState<uint32_t, 1>();
  TestUpdateBoardState<uint64_t, 1>();
  TestUpdateBoardState<__uint128_t, 1>();
}