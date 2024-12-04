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
    return boardstate::ZobristComponentForConcepts<C, NumConfKeys>::CreateFromSeed();
  }

  template <SingleBoardStateProviderConcept C, size_t NumConfKeys>
  void TestCreateFromZobristComponent() {
    auto zobrist_component = BuildComponentFromSeed<C, NumConfKeys>();
    auto zobrist_coordinator = boardstate::ZobristCoordinatorForConcepts<
        boardstate::ZobristComponentForConcepts<C, NumConfKeys>>::
        CreateFromZobristComponent(zobrist_component);
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
    static_assert()
    // using CalculatorType = boardstate::ZobristCalculatorForConcepts<KeyType>;
    // using ComponentType =
    //     boardstate::ZobristComponentForConcepts<CalculatorType, NumConfKeys>;
    // using CoordinatorType = boardstate::ZobristCoordinatorForConcepts<ComponentType>;
    // static_assert(
    //     BoardStateCoordinatorConcept<CoordinatorType>,
    //     "ZobristCoordinatorForConcepts must satisfy BoardStateCoordinatorConcept"
    // );
  }
};

TEST_F(ZobristCoordinatorConceptTest, SatisfiesBoardStateCoordinatorConcept) {
  TestSatisfiesBoardStateCoordinatorConcept<uint64_t, 0>();
  TestSatisfiesBoardStateCoordinatorConcept<uint32_t, 0>();
  TestSatisfiesBoardStateCoordinatorConcept<__uint128_t, 0>();
  TestSatisfiesBoardStateCoordinatorConcept<uint64_t, 1>();
  TestSatisfiesBoardStateCoordinatorConcept<uint32_t, 1>();
  TestSatisfiesBoardStateCoordinatorConcept<__uint128_t, 1>();
  // static_assert(
  //     BoardStateCoordinatorConcept<
  //         boardstate::ZobristCoordinatorForConcepts<uint64_t, 0>,
  //         uint64_t>,
  //     "ZobristCoordinatorForConcepts must satisfy BoardStateCoordinatorConcept"
  // );
}

TEST_F(ZobristCoordinatorConceptTest, TestCreateFromZobristComponent) {
  TestCreateFromZobristComponent<boardstate::ZobristCalculatorForConcepts<uint32_t>, 0>();
  TestCreateFromZobristComponent<boardstate::ZobristCalculatorForConcepts<uint64_t>, 0>();
  TestCreateFromZobristComponent<boardstate::ZobristCalculatorForConcepts<__uint128_t>, 0>();
  TestCreateFromZobristComponent<boardstate::ZobristCalculatorForConcepts<uint32_t>, 1>();
  TestCreateFromZobristComponent<boardstate::ZobristCalculatorForConcepts<uint64_t>, 1>();
  TestCreateFromZobristComponent<boardstate::ZobristCalculatorForConcepts<__uint128_t>, 1>();
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

//! todo redo after changing to new builder approach for ZobristCalculatorForConcepts
// TEST_F(ZobristCoordinatorConceptTest, RecordAndReadData) {

//   //   boardstate::ZobristCoordinatorForConcepts<uint64_t, 1>
//   //       coordinator{};

//   auto start = BoardSpace{6, 0};
//   auto end = BoardSpace{5, 0};
//   auto move = Move{start, end};
//   auto moving_piece = GamePiece{PieceType::kSol, PieceColor::kRed};
//   auto destination_piece = GamePiece{PieceType::kNnn, PieceColor::kNul};
//   auto executed_move = ExecutedMove{move, moving_piece, destination_piece};

//   coordinator_->FullBoardStateCalc(board_map_);
//   auto initial_state = coordinator_->GetState();

//   MoveCollection dummy_move_collection{};
//   dummy_move_collection.Append(move);

//   moveselection::EqualScoreMoves dummy_equal_score_moves{1, dummy_move_collection};
//   int dummy_search_depth = 1;
//   int dummy_access_index_at_write = 0;

//   coordinator_->RecordTrData(
//       dummy_search_depth,
//       moveselection::MinimaxResultType::kFullyEvaluatedNode,
//       dummy_equal_score_moves,
//       dummy_access_index_at_write
//   );

//   int dummy_access_index_at_read = 1;
//   auto retrieved_data =
//       coordinator_->GetTrData(dummy_search_depth, dummy_access_index_at_read);

//   EXPECT_TRUE(retrieved_data.found());
// }

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}