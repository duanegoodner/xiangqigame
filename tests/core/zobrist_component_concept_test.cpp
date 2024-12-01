#include <board_map_fixture.hpp>
#include <gtest/gtest.h>
#include <zobrist_calculator_for_concepts.hpp>
#include <zobrist_component_with_exposed_calculators_fixture.hpp>
#include <zobrist_for_concepts.hpp>

class ZobristComponentConceptTest : public ::testing::Test {
protected:
  fixtures::BoardMapFixture board_map_fixture_;

  template <typename KeyType, size_t NumConfKeys>
  std::shared_ptr<boardstate::ZobristComponentForConcepts<KeyType, NumConfKeys>>
  BuildComponentFromSeed() {
    return boardstate::ZobristComponentForConcepts<KeyType, NumConfKeys>::CreateFromSeed(
    );
  }

  template <typename KeyType, size_t NumConfKeys>
  void TestCreateFromSeed() {
    auto zobrist_component =
        boardstate::ZobristComponentForConcepts<KeyType, NumConfKeys>::CreateFromSeed();
  }

  template <typename KeyType, size_t NumConfKeys>
  void TestCreateFromExistingCalculators() {
    auto primary_calculator =
        boardstate::ZobristCalculatorForConcepts<KeyType>::Create();
    std::array<
        std::shared_ptr<boardstate::ZobristCalculatorForConcepts<KeyType>>,
        NumConfKeys>
        confirmation_calculators;
    for (auto calculator : confirmation_calculators) {
      calculator = boardstate::ZobristCalculatorForConcepts<KeyType>::Create();
    }
    auto zobrist_component =
        boardstate::ZobristComponentForConcepts<KeyType, NumConfKeys>::
            CreateFromCalculators(primary_calculator, confirmation_calculators);
  }

  template <typename KeyType, size_t NumConfKeys>
  void TestZobristCoordinatorWithExposedCalculators() {
    auto zobrist_component_with_exposed_calculators =
        fixtures::ZobristCoordinatorWithExposedCalculators<KeyType, NumConfKeys>::Create();
  }

  template <typename KeyType, size_t NumConfKeys>
  void TestGetters() {
    auto zobrist_component = BuildComponentFromSeed<KeyType, NumConfKeys>();
    auto primary_board_state = zobrist_component->primary_board_state();
    auto confirmation_board_states = zobrist_component->confirmation_board_states();
    auto prng_seed = zobrist_component->prng_seed();

    EXPECT_EQ(primary_board_state, 0);
    for (auto confirmation_state : confirmation_board_states) {
      EXPECT_EQ(confirmation_state, 0);
    }
    EXPECT_NE(prng_seed, 0);
  }

  template <typename KeyType, size_t NumConfKeys>
  void TestNumConfKeys() {
    auto zobrist_component = BuildComponentFromSeed<KeyType, NumConfKeys>();
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
  TestCreateFromSeed<uint32_t, 0>();
  TestCreateFromSeed<uint64_t, 0>();
  TestCreateFromSeed<__uint128_t, 0>();
  TestCreateFromSeed<uint32_t, 1>();
  TestCreateFromSeed<uint64_t, 1>();
  TestCreateFromSeed<__uint128_t, 1>();
}

TEST_F(ZobristComponentConceptTest, TestCreateFromExistingCalculators) {
  TestCreateFromExistingCalculators<uint32_t, 0>();
  TestCreateFromExistingCalculators<uint64_t, 0>();
  TestCreateFromExistingCalculators<__uint128_t, 0>();
  TestCreateFromExistingCalculators<uint32_t, 1>();
  TestCreateFromExistingCalculators<uint64_t, 1>();
  TestCreateFromExistingCalculators<__uint128_t, 1>();
}

TEST_F(ZobristComponentConceptTest, TestGetters) {
  TestGetters<uint32_t, 0>();
  TestGetters<uint64_t, 0>();
  TestGetters<__uint128_t, 0>();
  TestGetters<uint32_t, 1>();
  TestGetters<uint64_t, 1>();
  TestGetters<__uint128_t, 1>();
}

TEST_F(ZobristComponentConceptTest, TestNumConfKeys) {
  TestNumConfKeys<uint32_t, 0>();
  TestNumConfKeys<uint64_t, 0>();
  TestNumConfKeys<__uint128_t, 0>();
  TestNumConfKeys<uint32_t, 1>();
  TestNumConfKeys<uint64_t, 1>();
  TestNumConfKeys<__uint128_t, 1>();
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