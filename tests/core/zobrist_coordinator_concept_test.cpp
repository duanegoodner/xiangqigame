#include <board_state_coordinator_concept.hpp>
#include <concepts>
#include <game_board.hpp>
#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <shared_ptr_builder_tests.hpp>
#include <string>
#include <type_traits>
#include <zobrist_for_concepts.hpp>

//! Check shared_ptr behavior of pointer returned by ZobristCoordinatorBuilder
using ZobristCoorinatorBuilderTestTypes = ::testing::Types<BuilderObjectPair<
    boardstate::ZobristCoordinatorBuilder<uint64_t, 1>,
    boardstate::ZobristCoordinatorForConcepts<uint64_t, 1>>>;
INSTANTIATE_TYPED_TEST_SUITE_P(
    ZobristCoordinatorBuilderTestInstance,
    SharedPtrBuilderTest,
    ZobristCoorinatorBuilderTestTypes
);

class ZobristCoordinatorConceptTest : public ::testing::Test {
protected:
  boardstate::ZobristCoordinatorBuilder<uint64_t, 1> builder_;
  std::shared_ptr<boardstate::ZobristCoordinatorForConcepts<uint64_t, 1>> coordinator_ =
      builder_.build();
  BoardMap_t board_map_ = int_board_to_game_pieces(kStandardInitialBoard);
};

TEST_F(ZobristCoordinatorConceptTest, SatisfiesBoardStateCoordinatorConcept) {
  static_assert(
      BoardStateCoordinatorConcept<
          boardstate::ZobristCoordinatorForConcepts<uint64_t, 0>,
          uint64_t>,
      "GameBoardForConcepts must satisfy SpaceInfoProviderConcept"
  );
}


TEST_F(ZobristCoordinatorConceptTest, ExecuteAndUndoMove) {
  auto start = BoardSpace{6, 0};
  auto end = BoardSpace{5, 0};
  auto move = Move{start, end};
  auto moving_piece = GamePiece{PieceType::kSol, PieceColor::kRed};
  auto destination_piece = GamePiece{PieceType::kNnn, PieceColor::kNul};
  auto executed_move = ExecutedMove{move, moving_piece, destination_piece};

  coordinator_->FullBoardStateCalc(board_map_);
  auto initial_state =  coordinator_->GetState();
   coordinator_->UpdateBoardState(executed_move);
  auto post_move_state = coordinator_->GetState();
   coordinator_->UpdateBoardState(executed_move);
  auto final_state =  coordinator_->GetState();

  EXPECT_NE(initial_state, post_move_state);
  EXPECT_EQ(initial_state, final_state);
}

TEST_F(ZobristCoordinatorConceptTest, RecordAndReadData) {

//   boardstate::ZobristCoordinatorForConcepts<uint64_t, 1>
//       coordinator{};

  auto start = BoardSpace{6, 0};
  auto end = BoardSpace{5, 0};
  auto move = Move{start, end};
  auto moving_piece = GamePiece{PieceType::kSol, PieceColor::kRed};
  auto destination_piece = GamePiece{PieceType::kNnn, PieceColor::kNul};
  auto executed_move = ExecutedMove{move, moving_piece, destination_piece};

  coordinator_->FullBoardStateCalc(board_map_);
  auto initial_state = coordinator_->GetState();

  MoveCollection dummy_move_collection{};
  dummy_move_collection.Append(move);

  moveselection::EqualScoreMoves dummy_equal_score_moves{1, dummy_move_collection};
  int dummy_search_depth = 1;
  int dummy_access_index_at_write = 0;

  coordinator_->RecordTrData(
      dummy_search_depth,
      moveselection::MinimaxResultType::kFullyEvaluatedNode,
      dummy_equal_score_moves,
      dummy_access_index_at_write
  );

  int dummy_access_index_at_read = 1;
  auto retrieved_data =
      coordinator_->GetTrData(dummy_search_depth, dummy_access_index_at_read);

  EXPECT_TRUE(retrieved_data.found());
}