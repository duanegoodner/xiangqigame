#include <builders.hpp>
#include <concepts/board_state_coordinator.hpp>
#include <concepts/single_board_state_provider.hpp>
#include <concepts>
#include <gameboard/game_board_for_concepts.hpp>
#include <gtest/gtest.h>
#include <memory>
#include <string>
#include <type_traits>
#include <vector>
#include <boardstate/zobrist_calculator_for_concepts.hpp>
#include <boardstate/zobrist_for_concepts.hpp>

class ZobristCoordinatorTestSuiteBase {
public:
  virtual void TestSatisfiesBoardStateCoordinatorConcept() = 0;
  virtual void TestCreateFromExistingComponent() = 0;
  virtual void TestCreateWithFactory() = 0;
  virtual void TestExecuteMove() = 0;
  virtual void TestRecordAndReadData() = 0;
  virtual ~ZobristCoordinatorTestSuiteBase() = default;
};

template <typename KeyType, size_t NumConfKeys>
class ZobristCoordinatorTestSuite : public ZobristCoordinatorTestSuiteBase {
  gameboard::GameBoardFactory game_board_factory_;
  std::shared_ptr<gameboard::GameBoardForConcepts> starting_game_board_ =
      game_board_factory_.Create();

  gameboard::BoardSpace exmaple_move_start_{6, 0};
  gameboard::BoardSpace example_move_end_{5, 0};
  gameboard::Move example_move_{exmaple_move_start_, example_move_end_};

  using CalculatorFactoryType =
      boardstate::ZobristCalculatorFactory<KeyType, gameboard::GameBoardForConcepts>;
  using CalculatorType = typename CalculatorFactoryType::ZobristCalculatorType;
  using ComponentFactoryType = boardstate::
      ZobristComponentFactory<KeyType, NumConfKeys, gameboard::GameBoardForConcepts>;
  using ComponentType = typename ComponentFactoryType::ZobristComponentType;
  using CoordinatorFactoryType = boardstate::
      ZobristCoordinatorFactory<KeyType, NumConfKeys, gameboard::GameBoardForConcepts>;
  using CoordinatorType = typename CoordinatorFactoryType::ZobristCoordinatorType;

public:
  void TestSatisfiesBoardStateCoordinatorConcept() {
    // using CalculatorType = boardstate::ZobristCalculatorForConcepts<KeyType>;
    static_assert(
        BoardStateCoordinatorConcept<CoordinatorType>,
        "ZobristCoordinatorForConcepts must satisfy BoardStateCoordinatorConcept"
    );
  }

  void TestCreateFromExistingComponent() {
    auto zobrist_component = ComponentFactoryType().CreateUnregisteredComponent();
    auto zobrist_coordinator = CoordinatorType::Create(zobrist_component);

    CalculatorFactoryType calculator_factory;
    auto primary_calculator = calculator_factory.CreateUnregistereCalculator();

    ComponentFactoryType zobrist_component_factory;
    auto coordinator_state = zobrist_coordinator->GetState();
    // coordinator state should zero because not registered (FullBoardStateCalc never
    // called)
    EXPECT_EQ(coordinator_state, 0);
  }

  void TestCreateWithFactory() {
    auto zobrist_coordinator =
        CoordinatorFactoryType().CreateRegisteredCoordinator(starting_game_board_);
    auto coordinator_state = zobrist_coordinator->GetState();

    // coorinator state should be nonzero due to to initialization performed by
    // Factory.Create()
    EXPECT_NE(coordinator_state, 0);
  }

  void TestExecuteMove() {
    auto zobrist_coordinator =
        CoordinatorFactoryType().CreateRegisteredCoordinator(starting_game_board_);
    auto initial_state = zobrist_coordinator->GetState();
    starting_game_board_->ExecuteMove(example_move_);
    auto post_move_state = zobrist_coordinator->GetState();
    EXPECT_NE(initial_state, post_move_state);
  }

  void TestRecordAndReadData() {
    auto zobrist_coordinator =
        CoordinatorFactoryType().CreateRegisteredCoordinator(starting_game_board_);
    auto executed_move = starting_game_board_->ExecuteMove(example_move_);

    gameboard::MoveCollection dummy_move_collection{};
    dummy_move_collection.Append(executed_move.spaces);

    moveselection::EqualScoreMoves dummy_equal_score_moves{1, dummy_move_collection};
    DepthType dummy_search_depth = 1;
    MoveCountType dummy_access_at_write = 0;

    zobrist_coordinator->RecordTrData(
        dummy_search_depth,
        moveselection::MinimaxResultType::kFullyEvaluatedNode,
        dummy_equal_score_moves,
        dummy_access_at_write
    );

    MoveCountType dummy_access_index_at_read = 1;
    auto retrieved_data =
        zobrist_coordinator->GetTrData(dummy_search_depth, dummy_access_index_at_read);
    EXPECT_TRUE(retrieved_data.found());
  }
};

class ZobristCoordinatorConceptTest : public ::testing::Test {
protected:
  std::vector<std::shared_ptr<ZobristCoordinatorTestSuiteBase>> test_suites_;

  ZobristCoordinatorConceptTest() {
    test_suites_.emplace_back(std::make_shared<ZobristCoordinatorTestSuite<uint32_t, 0>>(
    ));
    test_suites_.emplace_back(std::make_shared<ZobristCoordinatorTestSuite<uint64_t, 0>>(
    ));
    test_suites_.emplace_back(
        std::make_shared<ZobristCoordinatorTestSuite<__uint128_t, 0>>()
    );
    test_suites_.emplace_back(std::make_shared<ZobristCoordinatorTestSuite<uint32_t, 1>>(
    ));
    test_suites_.emplace_back(std::make_shared<ZobristCoordinatorTestSuite<uint64_t, 1>>(
    ));
    test_suites_.emplace_back(
        std::make_shared<ZobristCoordinatorTestSuite<__uint128_t, 1>>()
    );
  }
};

TEST_F(ZobristCoordinatorConceptTest, TestSatisfiesBoardStateCoordinatorConcept) {
  for (auto &suite : test_suites_) {
    suite->TestSatisfiesBoardStateCoordinatorConcept();
  }
}

TEST_F(ZobristCoordinatorConceptTest, TestCreateFromExistingComponent) {
  for (auto &suite : test_suites_) {
    suite->TestCreateFromExistingComponent();
  }
}

TEST_F(ZobristCoordinatorConceptTest, TestCreateWithFactory) {
  for (auto &suite : test_suites_) {
    suite->TestCreateWithFactory();
  }
}

TEST_F(ZobristCoordinatorConceptTest, TestExecuteMove) {
  for (auto &suite : test_suites_) {
    suite->TestExecuteMove();
  }
}

TEST_F(ZobristCoordinatorConceptTest, TestRecordAndReadData) {
  for (auto &suite : test_suites_) {
    suite->TestRecordAndReadData();
  }
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}