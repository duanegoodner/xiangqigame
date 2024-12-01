#pragma once

#include <board_map_fixture.hpp>
#include <zobrist_calculator_for_concepts.hpp>
#include <zobrist_for_concepts.hpp>

namespace fixtures {
//! Contains ZobristCoordinator with its constituents (ZobristComponent and
//! ZobristCalculators) exposed so calculators' FullBoardStateCalc and UpdateBoardState
//! methods can be called without attaching them to a GameBoard.
template <typename KeyType, size_t NumConfKeys>
class ZobristCoordinatorWithExposedCalculators {

  std::shared_ptr<boardstate::ZobristCalculatorForConcepts<KeyType>> primary_calculator_;
  std::array<
      std::shared_ptr<boardstate::ZobristCalculatorForConcepts<KeyType>>,
      NumConfKeys>
      confirmation_calculators_;
  std::shared_ptr<boardstate::ZobristComponentForConcepts<KeyType, NumConfKeys>>
      zobrist_component_;
  std::shared_ptr<boardstate::ZobristCoordinatorForConcepts<KeyType, NumConfKeys>>
      zobrist_coordinator_;

  fixtures::BoardMapFixture board_map_fixture_;

public:
  ZobristCoordinatorWithExposedCalculators(
      std::shared_ptr<boardstate::ZobristCalculatorForConcepts<KeyType>>
          primary_calculator,
      std::array<
          std::shared_ptr<boardstate::ZobristCalculatorForConcepts<KeyType>>,
          NumConfKeys> confirmation_calculators,
      std::shared_ptr<boardstate::ZobristComponentForConcepts<KeyType, NumConfKeys>>
          zobrist_component,
      std::shared_ptr<boardstate::ZobristCoordinatorForConcepts<KeyType, NumConfKeys>>
          zobrist_coordinator
  )
      : primary_calculator_{primary_calculator}
      , confirmation_calculators_{confirmation_calculators}
      , zobrist_component_{zobrist_component}
      , zobrist_coordinator_{zobrist_coordinator} {}

  std::shared_ptr<boardstate::ZobristCalculatorForConcepts<KeyType>> primary_calculator(
  ) {
    return primary_calculator_;
  }

  std::array<
      std::shared_ptr<boardstate::ZobristCalculatorForConcepts<KeyType>>,
      NumConfKeys>
  confirmation_calculators() {
    return confirmation_calculators_;
  }

  std::shared_ptr<boardstate::ZobristComponentForConcepts<KeyType, NumConfKeys>>
  zobrist_component() {
    return zobrist_component_;
  }

  static std::shared_ptr<ZobristCoordinatorWithExposedCalculators<KeyType, NumConfKeys>>
  Create() {
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
    auto zobrist_coordinator =
        boardstate::ZobristCoordinatorForConcepts<KeyType, NumConfKeys>::
            CreateFromZobristComponent(zobrist_component);

    return std::make_shared<
        ZobristCoordinatorWithExposedCalculators<KeyType, NumConfKeys>>(
        primary_calculator,
        confirmation_calculators,
        zobrist_component,
        zobrist_coordinator
    );
  }

  void FullBoardStateCalc(const gameboard::BoardMap_t &board_map) {
    // calcluate state of each calculator in ZobristComponent
    primary_calculator_->FullBoardStateCalc(board_map);
    for (auto calculator : confirmation_calculators_) {
      calculator->FullBoardStateCalc(board_map);
    }
  }

  void UpdateBoardState(const gameboard::ExecutedMove &executed_move) {
    primary_calculator_->UpdateBoardState(executed_move);
    for (auto calculator : confirmation_calculators_) {
      calculator->UpdateBoardState(executed_move);
    }
  }

  void TestComponentFullBoardStateCalc() {

    auto starting_boardmap = board_map_fixture_.starting_boardmap();
    FullBoardStateCalc(starting_boardmap);

    EXPECT_NE(zobrist_component_->primary_board_state(), 0);
    for (auto board_state : zobrist_component_->confirmation_board_states()) {
      EXPECT_NE(board_state, 0);
    }
  }

  void TestComponentUpdateBoardState() {
    auto starting_boardmap = board_map_fixture_.starting_boardmap();

    FullBoardStateCalc(starting_boardmap);

    // //  Generate an ExecutedMove to pass to UpdateBoardState methods
    gameboard::BoardSpace move_start{6, 0};
    gameboard::BoardSpace move_end{5, 0};
    auto executed_move = board_map_fixture_.GenerateExecutedMove(move_start, move_end);

    // Get pre-move board states
    auto initial_primary_state = zobrist_component_->primary_board_state();
    auto initial_confirmation_states = zobrist_component_->confirmation_board_states();

    UpdateBoardState(executed_move);

    // get post-move states
    auto post_move_primary_state = zobrist_component_->primary_board_state();
    auto post_move_confirmation_states = zobrist_component_->confirmation_board_states();

    // Update calclators' again with same executed move (since Zobrist re-applies same
    // XOR, this should take us back to original states)
    UpdateBoardState(executed_move);

    auto final_primary_state = zobrist_component_->primary_board_state();
    auto final_confirmation_states = zobrist_component_->confirmation_board_states();

    EXPECT_NE(initial_primary_state, post_move_primary_state);
    EXPECT_EQ(initial_primary_state, final_primary_state);

    for (auto idx = 0; idx < NumConfKeys; ++idx) {
      EXPECT_NE(initial_confirmation_states[idx], post_move_confirmation_states[idx]);
      EXPECT_EQ(initial_confirmation_states[idx], final_confirmation_states[idx]);
    }
  }

  void TestCoordinatorExecuteAndUndoMove() {
    gameboard::BoardSpace move_start{6, 0};
    gameboard::BoardSpace move_end{5, 0};
    auto executed_move = board_map_fixture_.GenerateExecutedMove(move_start, move_end);
    auto starting_board_map = board_map_fixture_.starting_boardmap();
    FullBoardStateCalc(starting_board_map);

    auto initial_state = zobrist_coordinator_->GetState();
    UpdateBoardState(executed_move);
    auto post_move_state = zobrist_coordinator_->GetState();
    UpdateBoardState(executed_move);
    auto final_state = zobrist_coordinator_->GetState();

    EXPECT_NE(initial_state, post_move_state);
    EXPECT_EQ(initial_state, final_state);
  }

  void TestCoordinatorRecordAndReadData() {
    gameboard::BoardSpace move_start{6, 0};
    gameboard::BoardSpace move_end{5, 0};
    auto executed_move = board_map_fixture_.GenerateExecutedMove(move_start, move_end);

    auto board_map = board_map_fixture_.starting_boardmap();

    FullBoardStateCalc(board_map);
    auto initial_state = zobrist_coordinator_->GetState();

    gameboard::MoveCollection dummy_move_collection{};
    dummy_move_collection.Append(executed_move.spaces);

    moveselection::EqualScoreMoves dummy_equal_score_moves{1, dummy_move_collection};
    int dummy_search_depth = 1;
    int dummy_access_index_at_write = 0;

    zobrist_coordinator_->RecordTrData(
        dummy_search_depth,
        moveselection::MinimaxResultType::kFullyEvaluatedNode,
        dummy_equal_score_moves,
        dummy_access_index_at_write
    );

    int dummy_access_index_at_read = 1;
    auto retrieved_data =
        zobrist_coordinator_->GetTrData(dummy_search_depth, dummy_access_index_at_read);

    EXPECT_TRUE(retrieved_data.found());
  }
};
} // namespace fixtures