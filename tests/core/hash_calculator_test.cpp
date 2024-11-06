// #include <common.hpp>
#include <game_board.hpp>
#include <gtest/gtest.h>
#include <hash_calculator.hpp>
#include <nlohmann/json.hpp>
#include <string>

using namespace boardstate;
using namespace nlohmann::json_abi_v3_11_3;
using namespace std;

class ZobristCalculatorTest : public ::testing::Test {
protected:
  BoardMap_t board_map = int_board_to_game_pieces(kStartingBoard);
};

TEST_F(ZobristCalculatorTest, DefaultInit) {
  auto zobrist_keys_064 = ZobristCalculator<uint64_t>(123456);
  EXPECT_NE(0, zobrist_keys_064.turn_key());
  EXPECT_EQ(2, zobrist_keys_064.zarray().size());
  EXPECT_EQ(kNumPieceTypeVals, zobrist_keys_064.zarray()[0].size());
  EXPECT_EQ(kNumRanks, zobrist_keys_064.zarray()[0][0].size());
  EXPECT_EQ(kNumFiles, zobrist_keys_064.zarray()[0][0][0].size());
}

TEST_F(ZobristCalculatorTest, InitFromSeed) {
  auto zobrist_keys_064 = ZobristCalculator<uint64_t>(123456);
  EXPECT_NE(0, zobrist_keys_064.turn_key());
  EXPECT_EQ(2, zobrist_keys_064.zarray().size());
  EXPECT_EQ(kNumPieceTypeVals, zobrist_keys_064.zarray()[0].size());
  EXPECT_EQ(kNumRanks, zobrist_keys_064.zarray()[0][0].size());
  EXPECT_EQ(kNumFiles, zobrist_keys_064.zarray()[0][0][0].size());
}

TEST_F(ZobristCalculatorTest, GetHashValueAt) {
  auto zobrist_keys_064 = ZobristCalculator<uint64_t>(1234567);
  auto sample_key_value = zobrist_keys_064.GetHashValueAt(
      PieceColor::kBlk,
      PieceType::kAdv,
      BoardSpace{0, 3}
  );
}

template <typename KeyType>
  struct CalcFullBoardStateResult {
    std::vector<KeyType> initial_states;
    KeyType initial_primary_state;
    std::vector<KeyType> final_states;
    KeyType final_primary_state;
  };

class ZobristComponentTest : public ::testing::Test {
protected:
  ZobristCalculator<uint64_t> zobrist_calculator_064_a{};
  ZobristCalculator<uint64_t> zobrist_calculator_064_b{};

  BoardMap_t board_map{int_board_to_game_pieces(kStartingBoard)};

  template <typename KeyType>
  CalcFullBoardStateResult<KeyType> TestCalcFullBoardState(int num_calculators) {
    ZobristComponent<KeyType> zobrist_component{num_calculators};
    auto board_states_vector_initial = zobrist_component.GetAllBoardStates();
    auto primary_board_state_initial = zobrist_component.GetPrimaryBoardState();
    zobrist_component.FullBoardStateCalc(board_map);
    auto board_states_vector_final = zobrist_component.GetAllBoardStates();
    auto primary_board_state_final = zobrist_component.GetPrimaryBoardState();

    return CalcFullBoardStateResult{
        board_states_vector_initial,
        primary_board_state_initial,
        board_states_vector_final,
        primary_board_state_final
    };
  }
};

TEST_F(ZobristComponentTest, InitFromVectorOfCalculators) {
  std::vector<ZobristCalculator<uint64_t>> calculators{
      {zobrist_calculator_064_a},
      {zobrist_calculator_064_b}
  };
  ZobristComponent<uint64_t> component_064{calculators};
}

TEST_F(ZobristComponentTest, InitFromNumCalculators) {
  ZobristComponent<uint64_t> component_064_single{1};
  ZobristComponent<uint64_t> component_064_dual{2};
  ZobristComponent<uint64_t> component_064_triple{3};
}

TEST_F(ZobristComponentTest, InitFromVectorOfSeeds) {
  std::vector<uint32_t> one_seed{1234};
  ZobristComponent<uint64_t> component_064_single{one_seed};

  std::vector<uint32_t> two_seeds{12345, 678910};
  ZobristComponent<uint64_t> component_064_dual{two_seeds};

  std::vector<uint32_t> three_seeds{1234, 5678, 9101};
  ZobristComponent<uint64_t> component_064_triple{three_seeds};
}

TEST_F(ZobristComponentTest, CalcFullBoardState) {
  auto component_064_single_result = TestCalcFullBoardState<uint64_t>(1);
  auto component_064_dual_result = TestCalcFullBoardState<uint64_t>(2);
  auto component_065_triple_result = TestCalcFullBoardState<uint64_t>(3);
}

TEST_F(ZobristComponentTest, ConvertBoardStatesToHexStrings) {
  auto component_064_dual_result = TestCalcFullBoardState<uint64_t>(2);
  auto initial_primary_state =
      boardstate::IntToHexString(component_064_dual_result.initial_primary_state);
  std::cout << initial_primary_state << std::endl;

  for (auto &initial_state : component_064_dual_result.initial_states) {
    auto state_as_string = IntToHexString(initial_state);
    std::cout << state_as_string << std::endl;
  }

  auto final_primary_state =
      boardstate::IntToHexString(component_064_dual_result.final_primary_state);
  std::cout << final_primary_state << std::endl;

  for (auto &final_state : component_064_dual_result.final_states) {
    auto state_as_string = IntToHexString(final_state);
    std::cout << state_as_string << std::endl;
  }
}

class ZobristTrackerTest : public ::testing::Test {
protected:
  ZobristComponent<uint64_t> component_064_single{1};
  ZobristComponent<uint64_t> component_064_dual{2};
  ZobristComponent<uint64_t> component_064_triple{3};

  BoardMap_t board_map{int_board_to_game_pieces(kStartingBoard)};

  template <typename KeyType>
  void TestCalcFullBoardState(int num_calculators) {
    ZobristTracker<KeyType> zobrist_tracker{num_calculators};
    auto board_state_initial = zobrist_tracker.GetState();
    EXPECT_EQ(board_state_initial, 0);
    zobrist_tracker.FullBoardStateCalc(board_map);
    auto board_state_final = zobrist_tracker.GetState();
    EXPECT_NE(board_state_initial, board_state_final);
  }
};

TEST_F(ZobristTrackerTest, DefaultConstructor) {
  ZobristTracker<uint64_t> tracker_064_default{};
}

TEST_F(ZobristTrackerTest, InitFromComponent) {

  ZobristTracker<uint64_t> tracker_064_single{component_064_single};
  ZobristTracker<uint64_t> tracker_064_dual{component_064_dual};
  ZobristTracker<uint64_t> tracker_064_triple{component_064_triple};
}

TEST_F(ZobristTrackerTest, InitFromSeeds) {

  ZobristTracker<uint64_t> tracker_064_single{std::vector<uint32_t>{1234}};
  ZobristTracker<uint64_t> tracker_064_double{std::vector<uint32_t>{1234, 5678}};
  ZobristTracker<uint64_t> tracker_064_triple{std::vector<uint32_t>{1234, 5678, 9100}};
}

TEST_F(ZobristTrackerTest, InitFromNumCalculators) {
  ZobristTracker<uint64_t> tracker_064_single{1};
  ZobristTracker<uint64_t> tracker_064_double{2};
  ZobristTracker<uint64_t> tracker_064_triple{3};
}

TEST_F(ZobristTrackerTest, FullBoardStateCalc) {
  TestCalcFullBoardState<uint64_t>(1);
  TestCalcFullBoardState<uint64_t>(2);
  TestCalcFullBoardState<uint64_t>(3);
}

class SingleZobristTrackerTest : public ::testing::Test {

protected:
  BoardMap_t board_map = int_board_to_game_pieces(kStartingBoard);
  ZobristCalculator<uint32_t> zobrist_keys_032{78910};
  ZobristCalculator<uint64_t> zobrist_keys_064{654321};
  ZobristCalculator<__uint128_t> zobrist_keys_128{2468};
};

TEST_F(SingleZobristTrackerTest, InitializeFromKeysAndMap) {
  auto my_hash_calculator_064 = SingleZobristTracker<uint64_t>(zobrist_keys_064);
  auto my_hash_calculator_128 = SingleZobristTracker<__uint128_t>(zobrist_keys_128);
}

TEST_F(SingleZobristTrackerTest, DefaultInit) {
  auto my_hash_calculator_064 = SingleZobristTracker<uint64_t>(3579);
  auto my_hash_calculator_128 = SingleZobristTracker<__uint128_t>(8675309);

  auto dual_hash_calculator_064 = DualZobristTracker<uint64_t>();
  auto dual_hash_calculator_128 = DualZobristTracker<__uint128_t>();
}

TEST_F(SingleZobristTrackerTest, InitializeBoardState) {
  auto my_hash_calculator_064 = SingleZobristTracker<uint64_t>(22443355);
  EXPECT_EQ(my_hash_calculator_064.ImplementGetState(), 0);
  my_hash_calculator_064.ImplementFullBoardStateCalc(board_map);
  EXPECT_NE(my_hash_calculator_064.ImplementGetState(), 0);
}

TEST_F(SingleZobristTrackerTest, BoardStateHexString) {
  auto my_hash_calculator_032 = SingleZobristTracker<uint32_t>(22443355);
  my_hash_calculator_032.ImplementFullBoardStateCalc(board_map);
  auto hex_state_032 = my_hash_calculator_032.board_state_hex_str();
  std::cout << hex_state_032 << std::endl;

  auto my_hash_calculator_064 = SingleZobristTracker<uint64_t>(22443355);
  my_hash_calculator_064.ImplementFullBoardStateCalc(board_map);
  auto hex_state_064 = my_hash_calculator_064.board_state_hex_str();
  std::cout << hex_state_064 << std::endl;

  auto my_hash_calculator_128 = SingleZobristTracker<__uint128_t>(22443355);
  my_hash_calculator_128.ImplementFullBoardStateCalc(board_map);
  auto hex_state_128 = my_hash_calculator_128.board_state_hex_str();
  std::cout << hex_state_128 << std::endl;
}

TEST_F(SingleZobristTrackerTest, ExecuteAndUndoMove64) {

  auto my_hash_calculator = SingleZobristTracker<uint64_t>(zobrist_keys_064);

  auto start = BoardSpace{6, 0};
  auto end = BoardSpace{5, 0};
  auto move = Move{start, end};
  auto moving_piece = GamePiece{PieceType::kSol, PieceColor::kRed};
  auto destination_piece = GamePiece{PieceType::kNnn, PieceColor::kNul};
  auto executed_move = ExecutedMove{move, moving_piece, destination_piece};

  my_hash_calculator.ImplementFullBoardStateCalc(board_map);
  auto initial_state = my_hash_calculator.ImplementGetState();
  my_hash_calculator.ImplementUpdateBoardState(executed_move);
  auto post_move_state = my_hash_calculator.ImplementGetState();
  my_hash_calculator.ImplementUpdateBoardState(executed_move);
  auto final_state = my_hash_calculator.ImplementGetState();

  EXPECT_NE(initial_state, post_move_state);
  EXPECT_EQ(initial_state, final_state);
}

TEST_F(SingleZobristTrackerTest, ExecuteAndUndoMove128) {

  auto my_hash_calculator = SingleZobristTracker<__uint128_t>(zobrist_keys_128);

  auto start = BoardSpace{6, 0};
  auto end = BoardSpace{5, 0};
  auto move = Move{start, end};
  auto moving_piece = GamePiece{PieceType::kSol, PieceColor::kRed};
  auto destination_piece = GamePiece{PieceType::kNnn, PieceColor::kNul};
  auto executed_move = ExecutedMove{move, moving_piece, destination_piece};

  my_hash_calculator.ImplementFullBoardStateCalc(board_map);
  auto initial_state = my_hash_calculator.ImplementGetState();
  my_hash_calculator.ImplementUpdateBoardState(executed_move);
  auto post_move_state = my_hash_calculator.ImplementGetState();
  my_hash_calculator.ImplementUpdateBoardState(executed_move);
  auto final_state = my_hash_calculator.ImplementGetState();

  EXPECT_NE(initial_state, post_move_state);
  EXPECT_EQ(initial_state, final_state);
}

class DualZobristTrackerTest : public ::testing::Test {
protected:
  BoardMap_t board_map = int_board_to_game_pieces(kStartingBoard);
  ZobristCalculator<uint32_t> zobrist_calculator_032_a{78910};
  ZobristCalculator<uint32_t> zobrist_calculator_032_b{910789};

  ZobristCalculator<uint64_t> zobrist_calculator_064_a{654321};
  ZobristCalculator<uint64_t> zobrist_calculator_064_b{321654};

  ZobristCalculator<__uint128_t> zobrist_calculator_128_a{2468};
  ZobristCalculator<__uint128_t> zobrist_calculator_128_b{6824};
};

TEST_F(DualZobristTrackerTest, DefaultInit) {
  auto dual_tracker_032 = DualZobristTracker<uint32_t>();
  auto dual_tracker_064 = DualZobristTracker<uint64_t>();
  auto dual_tracker_128 = DualZobristTracker<__uint128_t>();
}

TEST_F(DualZobristTrackerTest, InitWithExistingCalculators) {
  auto dual_tracker_032 =
      DualZobristTracker<uint32_t>(zobrist_calculator_032_a, zobrist_calculator_032_b);
  auto dual_tracker_064 =
      DualZobristTracker<uint64_t>(zobrist_calculator_064_a, zobrist_calculator_064_b);
  auto dual_tracker_128 = DualZobristTracker<__uint128_t>(
      zobrist_calculator_128_a,
      zobrist_calculator_128_b
  );
}

TEST_F(DualZobristTrackerTest, InitFromSeed) {
  DualZobristTracker<uint32_t> dual_tracker_032{20241031};
  DualZobristTracker<uint64_t> dual_tracker_064{20241030};
  DualZobristTracker<__uint128_t> dual_tracker_128{20241029};
}

TEST_F(DualZobristTrackerTest, ExecuteAndUndoMove64) {

  DualZobristTracker<uint64_t> dual_tracker_064{};

  auto start = BoardSpace{6, 0};
  auto end = BoardSpace{5, 0};
  auto move = Move{start, end};
  auto moving_piece = GamePiece{PieceType::kSol, PieceColor::kRed};
  auto destination_piece = GamePiece{PieceType::kNnn, PieceColor::kNul};
  auto executed_move = ExecutedMove{move, moving_piece, destination_piece};

  dual_tracker_064.ImplementFullBoardStateCalc(board_map);
  auto initial_state = dual_tracker_064.ImplementGetState();
  dual_tracker_064.ImplementUpdateBoardState(executed_move);
  auto post_move_state = dual_tracker_064.ImplementGetState();
  dual_tracker_064.ImplementUpdateBoardState(executed_move);
  auto final_state = dual_tracker_064.ImplementGetState();

  EXPECT_NE(initial_state, post_move_state);
  EXPECT_EQ(initial_state, final_state);
}

TEST_F(DualZobristTrackerTest, RecordAndReadData) {

  DualZobristTracker<uint64_t> dual_tracker_064{};

  auto start = BoardSpace{6, 0};
  auto end = BoardSpace{5, 0};
  auto move = Move{start, end};
  auto moving_piece = GamePiece{PieceType::kSol, PieceColor::kRed};
  auto destination_piece = GamePiece{PieceType::kNnn, PieceColor::kNul};
  auto executed_move = ExecutedMove{move, moving_piece, destination_piece};

  dual_tracker_064.ImplementFullBoardStateCalc(board_map);
  auto initial_state = dual_tracker_064.ImplementGetState();

  MoveCollection dummy_move_collection{};
  dummy_move_collection.Append(move);

  EqualScoreMoves dummy_equal_score_moves{1, dummy_move_collection};

  dual_tracker_064
      .RecordTrData(1, MinimaxResultType::kFullyEvaluatedNode, dummy_equal_score_moves);

  auto retrieved_data = dual_tracker_064.GetTrData(1);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}