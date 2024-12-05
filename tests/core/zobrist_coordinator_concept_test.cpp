#include <board_map_fixture.hpp>
#include <builders.hpp>
#include <concept_board_state_coordinator.hpp>
#include <concept_single_board_state_provider.hpp>
#include <concepts>
#include <game_board.hpp>
#include <gtest/gtest.h>
#include <memory>
#include <nlohmann/json.hpp>
#include <string>
#include <type_traits>
#include <zobrist_component_with_exposed_calculators_fixture.hpp>
#include <zobrist_for_concepts.hpp>

class ZobristCoordinatorConceptTest : public ::testing::Test {
protected:
  fixtures::BoardMapFixture board_map_fixture_;

  template <SingleBoardStateProviderConcept C, size_t NumConfKeys>
  std::shared_ptr<boardstate::ZobristComponentForConcepts<C, NumConfKeys>>
  BuildComponentFromSeed() {
    return boardstate::ZobristComponentForConcepts<C, NumConfKeys>::Create();
  }

  template <SingleBoardStateProviderConcept C, size_t NumConfKeys>
  void TestCreate() {
    auto zobrist_component = BuildComponentFromSeed<C, NumConfKeys>();
    auto zobrist_coordinator = boardstate::ZobristCoordinatorForConcepts<
        boardstate::ZobristComponentForConcepts<C, NumConfKeys>>::
        Create(zobrist_component);
  }

  template <SingleBoardStateProviderConcept C, size_t NumConfKeys>
  void TestExecuteAndUndoMove() {
    auto accessible_component =
        fixtures::ZobristCoordinatorWithExposedCalculators<C, NumConfKeys>::Create();
    accessible_component->TestCoordinatorExecuteAndUndoMove();
  }

  template <SingleBoardStateProviderConcept C, size_t NumConfKeys>
  void TestRecordAndReadData() {
    auto accessible_component =
        fixtures::ZobristCoordinatorWithExposedCalculators<C, NumConfKeys>::Create();
    accessible_component->TestCoordinatorRecordAndReadData();
  }

  template <SingleBoardStateProviderConcept C, size_t NumConfKeys>
  void TestSatisfiesBoardStateCoordinatorConcept() {
    // using CalculatorType = boardstate::ZobristCalculatorForConcepts<KeyType>;
    using ComponentType =
        boardstate::ZobristComponentForConcepts<C, NumConfKeys>;
    using CoordinatorType = boardstate::ZobristCoordinatorForConcepts<ComponentType>;
    static_assert(
        BoardStateCoordinatorConcept<CoordinatorType>,
        "ZobristCoordinatorForConcepts must satisfy BoardStateCoordinatorConcept"
    );
  }
};

TEST_F(ZobristCoordinatorConceptTest, SatisfiesBoardStateCoordinatorConcept) {
  TestSatisfiesBoardStateCoordinatorConcept<boardstate::ZobristCalculatorForConcepts<uint32_t>, 0>();
  TestSatisfiesBoardStateCoordinatorConcept<boardstate::ZobristCalculatorForConcepts<uint64_t>, 0>();
  TestSatisfiesBoardStateCoordinatorConcept<boardstate::ZobristCalculatorForConcepts<__uint128_t>, 0>();
  TestSatisfiesBoardStateCoordinatorConcept<boardstate::ZobristCalculatorForConcepts<uint32_t>, 1>();
  TestSatisfiesBoardStateCoordinatorConcept<boardstate::ZobristCalculatorForConcepts<uint64_t>, 1>();
  TestSatisfiesBoardStateCoordinatorConcept<boardstate::ZobristCalculatorForConcepts<__uint128_t>, 1>();
}

TEST_F(ZobristCoordinatorConceptTest, TestCreate) {
  TestCreate<boardstate::ZobristCalculatorForConcepts<uint32_t>, 0>();
  TestCreate<boardstate::ZobristCalculatorForConcepts<uint64_t>, 0>();
  TestCreate<boardstate::ZobristCalculatorForConcepts<__uint128_t>, 0>();
  TestCreate<boardstate::ZobristCalculatorForConcepts<uint32_t>, 1>();
  TestCreate<boardstate::ZobristCalculatorForConcepts<uint64_t>, 1>();
  TestCreate<boardstate::ZobristCalculatorForConcepts<__uint128_t>, 1>();
}

TEST_F(ZobristCoordinatorConceptTest, TestExecuteAndUndoMove) {
  TestExecuteAndUndoMove<boardstate::ZobristCalculatorForConcepts<uint32_t>, 0>();
  TestExecuteAndUndoMove<boardstate::ZobristCalculatorForConcepts<uint64_t>, 0>();
  TestExecuteAndUndoMove<boardstate::ZobristCalculatorForConcepts<__uint128_t>, 0>();
  TestExecuteAndUndoMove<boardstate::ZobristCalculatorForConcepts<uint32_t>, 1>();
  TestExecuteAndUndoMove<boardstate::ZobristCalculatorForConcepts<uint64_t>, 1>();
  TestExecuteAndUndoMove<boardstate::ZobristCalculatorForConcepts<__uint128_t>, 1>();
}

TEST_F(ZobristCoordinatorConceptTest, TestRecordAndReadData) {
  TestRecordAndReadData<boardstate::ZobristCalculatorForConcepts<uint32_t>, 0>();
  TestRecordAndReadData<boardstate::ZobristCalculatorForConcepts<uint64_t>, 0>();
  TestRecordAndReadData<boardstate::ZobristCalculatorForConcepts<__uint128_t>, 0>();
  TestRecordAndReadData<boardstate::ZobristCalculatorForConcepts<uint32_t>, 1>();
  TestRecordAndReadData<boardstate::ZobristCalculatorForConcepts<uint64_t>, 1>();
  TestRecordAndReadData<boardstate::ZobristCalculatorForConcepts<__uint128_t>, 1>();
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}