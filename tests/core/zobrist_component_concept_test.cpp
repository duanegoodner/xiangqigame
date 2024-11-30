#include <board_map_fixture.hpp>
#include <gtest/gtest.h>
#include <zobrist_calculator_for_concepts.hpp>
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
    // Build ZobristCalculators that will be members of ZobristComponent
    auto primary_calculator =
        boardstate::ZobristCalculatorForConcepts<KeyType>::Create();
    std::array<
        std::shared_ptr<boardstate::ZobristCalculatorForConcepts<KeyType>>,
        NumConfKeys>
        confirmation_calculators;
    for (auto calculator : confirmation_calculators) {
      calculator = boardstate::ZobristCalculatorForConcepts<KeyType>::Create();
    }

    // Build the ZobristComponent using ZobristCalculators created above
    auto zobrist_component =
        boardstate::ZobristComponentForConcepts<KeyType, NumConfKeys>::
            CreateFromCalculators(primary_calculator, confirmation_calculators);

    auto starting_boardmap = board_map_fixture_.starting_boardmap();

    // Call all ZobristCalculator objects' FullBoardStateCalc method
    primary_calculator->FullBoardStateCalc(starting_boardmap);
    for (auto calculator : confirmation_calculators) {
      calculator->FullBoardStateCalc(starting_boardmap);
    }

    // Confirm that all calculators' board_state is non-zero
    EXPECTE_NE(zobrist_component->primary_board_state(), 0);
    for (auto board_state : zobrist_component->confirmation_board_states()) {
      EXPECT_NE(board_state, 0);
    }
  }

  template <typename KeyType, size_t NumConfKeys>
  void TestUpdateBoardState() {

    // Build ZobristCalculators that will be members of ZobristComponent
    auto primary_calculator =
        boardstate::ZobristCalculatorForConcepts<KeyType>::Create();
    std::array<
        std::shared_ptr<boardstate::ZobristCalculatorForConcepts<KeyType>>,
        NumConfKeys>
        confirmation_calculators;
    for (auto calculator : confirmation_calculators) {
      calculator = boardstate::ZobristCalculatorForConcepts<KeyType>::Create();
    }

    // Build the ZobristComponent using ZobristCalculators created above
    auto zobrist_component =
        boardstate::ZobristComponentForConcepts<KeyType, NumConfKeys>::
            CreateFromCalculators(primary_calculator, confirmation_calculators);

    auto starting_boardmap = board_map_fixture_.starting_boardmap();

    // Call all ZobristCalculator objects' FullBoardStateCalc method
    primary_calculator->FullBoardStateCalc(starting_boardmap);
    for (auto calculator : confirmation_calculators) {
      calculator->FullBoardStateCalc(starting_boardmap);
    }

    //  Generate an ExecutedMove to pass to UpdateBoardState methods
    gameboard::BoardSpace move_start{6, 0};
    gameboard::BoardSpace move_end{5, 0};
    auto executed_move = board_map_fixture_.GenerateExecutedMove(move_start, move_end);

    // Get pre-move board states
    auto initial_primary_state = zobrist_component->primary_board_state();
    auto initial_confirmation_states = zobrist_component->confirmation_board_states();

    // Update calculators' states based on executed move
    primary_calculator->UpdateBoardState(executed_move);
    for (auto calculator : confirmation_calculators) {
      calculator->UpdateBoardState(executed_move);
    }

    // get post-move states
    auto post_move_primary_state = zobrist_component->primary_board_state();
    auto post_move_confirmation_states = zobrist_component->confirmation_board_states();

    // Update calclators' again with same executed move (since Zobrist re-applies same
    // XOR, this should take us back to original states)
    primary_calculator->UpdateBoardState(executed_move);
    for (auto calculator : confirmation_calculators) {
      calculator->UpdateBoardState(executed_move);
    }
    auto final_primary_state = zobrist_component->primary_board_state();
    auto final_confirmation_states = zobrist_component->confirmation_board_states();

    EXPECT_NE(initial_primary_state, post_move_primary_state);
    EXPECT_EQ(initial_primary_state, final_primary_state);

    for (auto idx = 0; idx < NumConfKeys; ++idx) {
      EXPECT_NE(initial_confirmation_states[idx], post_move_confirmation_states[idx]);
      EXPECT_EQ(initial_confirmation_states[idx], final_confirmation_states[idx]);
    }
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