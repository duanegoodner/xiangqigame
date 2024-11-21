#include <board_state_coordinator_concept.hpp>
#include <concepts>
#include <game_board.hpp>
#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <string>
#include <type_traits>
#include <zobrist.hpp>

class ZobristCoordinatorConceptTest : public ::testing::Test {
protected:
  BoardMap_t board_map_ = int_board_to_game_pieces(kStartingBoard);
  boardstate::ZobristCalculator<uint64_t> primary_calculator_{};
  std::array<boardstate::ZobristCalculator<uint64_t>, 1> confirmation_calculators_{
      boardstate::ZobristCalculator<uint64_t>()
  };
  boardstate::ZobristComponentNew<uint64_t, 1> zobrist_component_{
      primary_calculator_,
      confirmation_calculators_
  };
  boardstate::TranspositionTable<uint64_t, 1> tr_table_{};
  boardstate::TranspositionTableGuard tr_table_guard_{};
  boardstate::TranspositionTablePruner<uint64_t, 1> tr_table_pruner_{
      tr_table_,
      tr_table_guard_
  };
};

TEST_F(ZobristCoordinatorConceptTest, SatisfiesBoardStateCoordinatorConcept) {
  static_assert(
      BoardStateCoordinatorConcept<
          boardstate::ZobristCoordinatorForConcept<uint64_t, 0>,
          uint64_t>,
      "GameBoardNew must satisfy SpaceInfoProviderConcept"
  );
}

TEST_F(ZobristCoordinatorConceptTest, Init) {
  boardstate::ZobristCoordinatorForConcept<uint64_t, 1>
      coordinator_{zobrist_component_, tr_table_, tr_table_guard_, tr_table_pruner_};
}

TEST_F(ZobristCoordinatorConceptTest, ExecuteAndUndoMove) {
  boardstate::ZobristCoordinatorForConcept<uint64_t, 1>
      coordinator{zobrist_component_, tr_table_, tr_table_guard_, tr_table_pruner_};

  auto start = BoardSpace{6, 0};
  auto end = BoardSpace{5, 0};
  auto move = Move{start, end};
  auto moving_piece = GamePiece{PieceType::kSol, PieceColor::kRed};
  auto destination_piece = GamePiece{PieceType::kNnn, PieceColor::kNul};
  auto executed_move = ExecutedMove{move, moving_piece, destination_piece};

  coordinator.FullBoardStateCalc(board_map_);
  auto initial_state = coordinator.GetState();
  coordinator.UpdateBoardState(executed_move);
  auto post_move_state = coordinator.GetState();
  coordinator.UpdateBoardState(executed_move);
  auto final_state = coordinator.GetState();

  EXPECT_NE(initial_state, post_move_state);
  EXPECT_EQ(initial_state, final_state);
}

TEST_F(ZobristCoordinatorConceptTest, RecordAndReadData) {

  boardstate::ZobristCoordinatorForConcept<uint64_t, 1>
      coordinator{zobrist_component_, tr_table_, tr_table_guard_, tr_table_pruner_};

  auto start = BoardSpace{6, 0};
  auto end = BoardSpace{5, 0};
  auto move = Move{start, end};
  auto moving_piece = GamePiece{PieceType::kSol, PieceColor::kRed};
  auto destination_piece = GamePiece{PieceType::kNnn, PieceColor::kNul};
  auto executed_move = ExecutedMove{move, moving_piece, destination_piece};

  coordinator.FullBoardStateCalc(board_map_);
  auto initial_state = coordinator.GetState();

  MoveCollection dummy_move_collection{};
  dummy_move_collection.Append(move);

  moveselection::EqualScoreMoves dummy_equal_score_moves{1, dummy_move_collection};
  int dummy_search_depth = 1;
  int dummy_access_index_at_write = 0;

  coordinator.RecordTrData(
      dummy_search_depth,
      moveselection::MinimaxResultType::kFullyEvaluatedNode,
      dummy_equal_score_moves,
      dummy_access_index_at_write
  );

  int dummy_access_index_at_read = 1;
  auto retrieved_data =
      coordinator.GetTrData(dummy_search_depth, dummy_access_index_at_read);

  EXPECT_TRUE(retrieved_data.found());
}