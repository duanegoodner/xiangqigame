#include <board_map_fixture.hpp>
#include <concept_board_state_coordinator.hpp>
// #include <concept_board_state_tracker.hpp>
#include <builders.hpp>
#include <concepts>
#include <game_board.hpp>
#include <gtest/gtest.h>
#include <memory>
#include <nlohmann/json.hpp>
// #include <shared_ptr_builder_tests.hpp>
#include <string>
#include <type_traits>
#include <zobrist_for_concepts.hpp>
#include <zobrist_component_with_exposed_calculators_fixture.hpp>

//! Check shared_ptr behavior of pointer returned by ZobristCoordinatorBuilder
// using ZobristCoorinatorBuilderTestTypes = ::testing::Types<BuilderObjectPair<
//     boardstate::ZobristCoordinatorBuilder<uint64_t, 1>,
//     boardstate::ZobristCoordinatorForConcepts<uint64_t, 1>>>;
// INSTANTIATE_TYPED_TEST_SUITE_P(
//     ZobristCoordinatorBuilderTestInstance,
//     SharedPtrBuilderTest,
//     ZobristCoorinatorBuilderTestTypes
// );

class ZobristCoordinatorConceptTest : public ::testing::Test {
protected:
  fixtures::BoardMapFixture board_map_fixture_;

  template <typename KeyType, size_t NumConfKeys>
  std::shared_ptr<boardstate::ZobristComponentForConcepts<KeyType, NumConfKeys>>
  BuildComponentFromSeed() {
    return boardstate::ZobristComponentForConcepts<KeyType, NumConfKeys>::CreateFromSeed(
    );
  }

  template <typename KeyType, size_t NumConfKeys>
  void TestCreateFromZobristComponent() {
    auto zobrist_component = BuildComponentFromSeed<KeyType, NumConfKeys>();
    auto zobrist_coordinator =
        boardstate::ZobristCoordinatorForConcepts<KeyType, NumConfKeys>::
            CreateFromZobristComponent(zobrist_component);
  }

  template <typename KeyType, size_t NumConfKeys>
  void TestExecuteAndUndoMove() {
    auto accessible_component =
        fixtures::ZobristComponentWithExposedCalculators<KeyType, NumConfKeys>::Create();
    accessible_component->TestCoordinatorExecuteAndUndoMove();
  }

  template <typename KeyType, size_t NumConfKeys>
  void TestRecordAndReadData() {
    auto accessible_component =
        fixtures::ZobristComponentWithExposedCalculators<KeyType, NumConfKeys>::Create();
    accessible_component->TestCoordinatorRecordAndReadData();
  }


};

TEST_F(ZobristCoordinatorConceptTest, SatisfiesBoardStateCoordinatorConcept) {
  static_assert(
      BoardStateCoordinatorConcept<
          boardstate::ZobristCoordinatorForConcepts<uint64_t, 0>,
          uint64_t>,
      "ZobristCoordinatorForConcepts must satisfy BoardStateCoordinatorConcept"
  );
}

TEST_F(ZobristCoordinatorConceptTest, TestCreateFromZobristComponent) {
  TestCreateFromZobristComponent<uint32_t, 0>();
  TestCreateFromZobristComponent<uint64_t, 0>();
  TestCreateFromZobristComponent<__uint128_t, 0>();
  TestCreateFromZobristComponent<uint32_t, 1>();
  TestCreateFromZobristComponent<uint64_t, 1>();
  TestCreateFromZobristComponent<__uint128_t, 1>();
}

TEST_F(ZobristCoordinatorConceptTest, TestExecuteAndUndoMove) {
  TestExecuteAndUndoMove<uint32_t, 0>();
  TestExecuteAndUndoMove<uint64_t, 0>();
  TestExecuteAndUndoMove<__uint128_t, 0>();
  TestExecuteAndUndoMove<uint32_t, 1>();
  TestExecuteAndUndoMove<uint64_t, 1>();
  TestExecuteAndUndoMove<__uint128_t, 1>();
}

TEST_F(ZobristCoordinatorConceptTest, TestRecordAndReadData) {
  TestRecordAndReadData<uint32_t, 0>();
  TestRecordAndReadData<uint64_t, 0>();
  TestRecordAndReadData<__uint128_t, 0>();
  TestRecordAndReadData<uint32_t, 1>();
  TestRecordAndReadData<uint64_t, 1>();
  TestRecordAndReadData<__uint128_t, 1>();
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