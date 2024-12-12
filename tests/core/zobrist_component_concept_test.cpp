#include <array>
#include <board_map_fixture.hpp>
#include <builders.hpp>
#include <concept_multi_board_state_provider.hpp>
#include <concept_single_board_state_provider.hpp>
#include <game_board_for_concepts.hpp>
#include <gtest/gtest.h>
#include <memory>
#include <vector>
#include <zobrist_calculator_for_concepts.hpp>
#include <zobrist_for_concepts.hpp>

//! Base class that test suite class template will inherit from, so we can put
//! references/pointers to multiple instances in a conatainer to iterate over inside
//! TEST_F calls.
class ZobristComponentTestSuiteBase {
public:
  virtual void CheckComplianceWithMultiBoardStateProviderConcept() = 0;
  virtual void TestCreateFromSeed() = 0;
  virtual void TestCreateFromExistingCalculators() = 0;
  virtual void TestCreateWithBuilder() = 0;
  virtual void TestGetters() = 0;
  virtual void TestNumConfKeys() = 0;
  virtual void TestUpdateBoardState() = 0;
  virtual void TestUpdateBoardStateReversal() = 0;
  virtual ~ZobristComponentTestSuiteBase() = default;
};

//! Suite of tests and supporting objects & types for ZobristComponent. Since
//! ZobristComponent and ZobristComponentFactory are templates, we make the test suite a
//! template too.
template <typename KeyType, size_t NumConfKeys>
class ZobristComponentTestSuite : public ZobristComponentTestSuiteBase {
  gameboard::GameBoardFactory game_board_factory_;
  std::shared_ptr<gameboard::GameBoardForConcepts> game_board_ =
      game_board_factory_.Create();

public:
  using ZobristCalculatorFactoryType =
      boardstate::ZobristCalculatorFactory<KeyType, gameboard::GameBoardForConcepts>;
  using ZobristCalculatorType = typename ZobristCalculatorFactoryType::ZobristCalculatorType;
  using ZobristComponentFactoryType = boardstate::
      ZobristComponentFactory<KeyType, NumConfKeys, gameboard::GameBoardForConcepts>;
  using ZobristComponentType = typename ZobristComponentFactoryType::ZobristComponentType;

  ZobristCalculatorFactoryType zobrist_calculator_factory_;
  ZobristComponentFactoryType zobrist_component_factory_;

  std::shared_ptr<ZobristComponentType> zobrist_component_unregistered_ =
      ZobristComponentType::Create();
  std::shared_ptr<ZobristComponentType> zobrist_component_registered_ =
      zobrist_component_factory_.CreateRegisteredComponent(game_board_);

  void CheckComplianceWithMultiBoardStateProviderConcept() override {
    static_assert(
        MultiBoardStateProviderConcept<ZobristComponentType>,
        "ZobristComponent must comply with MultiBoardStateProviderConcept"
    );
  }

  void TestCreateFromSeed() { auto zobrist_component = ZobristComponentType::Create(); }

  void TestCreateFromExistingCalculators() {
    auto primary_calculator = ZobristCalculatorType::Create();
    std::array<std::shared_ptr<ZobristCalculatorType>, NumConfKeys>
        confirmation_calculators;
    for (auto idx = 0; idx < NumConfKeys; ++idx) {
      confirmation_calculators[idx] = ZobristCalculatorType::Create();
    }

    auto zobrist_component =
        ZobristComponentType::Create(primary_calculator, confirmation_calculators);
  }

  void TestCreateWithBuilder() {
    auto zobrist_component =
        zobrist_component_factory_.CreateRegisteredComponent(game_board_);
  }

  void TestGetters() {
    auto unregistered_primary_board_state =
        zobrist_component_unregistered_->primary_board_state();
    auto registered_primary_board_state =
        zobrist_component_registered_->primary_board_state();
    // unregistered states should be zero because FullBoardStateCalc never ran
    EXPECT_EQ(unregistered_primary_board_state, 0);
    // registered states should NOT be zero because FullBoarStateCalc callec by Factory
    EXPECT_NE(registered_primary_board_state, 0);

    auto unregistered_confirmation_board_states =
        zobrist_component_unregistered_->confirmation_board_states();
    auto registered_confirmation_board_states =
        zobrist_component_registered_->confirmation_board_states();
    for (auto idx = 0; idx < NumConfKeys; ++idx) {
      EXPECT_EQ(unregistered_confirmation_board_states[idx], 0);
      EXPECT_NE(registered_confirmation_board_states[idx], 0);
    }

    auto unregistered_prng_seed = zobrist_component_unregistered_->prng_seed();
    auto registred_prng_seed = zobrist_component_registered_->prng_seed();
  }

  void TestNumConfKeys() {
    auto num_conf_keys_unregistered =
        zobrist_component_unregistered_->confirmation_board_states().size();
    EXPECT_EQ(num_conf_keys_unregistered, NumConfKeys);

    auto num_conf_keys_registered =
        zobrist_component_registered_->confirmation_board_states().size();
    EXPECT_EQ(num_conf_keys_registered, NumConfKeys);
  }

  void TestUpdateBoardState() {
    // Get pre-move board states
    auto initial_primary_state = zobrist_component_registered_->primary_board_state();
    auto initial_confirmation_states =
        zobrist_component_registered_->confirmation_board_states();

    // Generate a Move and execute it. This should call UpdateBoardState of each
    // ZobristCalculator in ZobristComponent since calculaltors in
    // zobrist_component_registered_ are registered with game_board_.
    gameboard::BoardSpace move_start{6, 0};
    gameboard::BoardSpace move_end{5, 0};
    gameboard::Move move{move_start, move_end};
    auto executed_move = game_board_->ExecuteMove(move);

    // get post-move states
    auto post_move_primary_state = zobrist_component_registered_->primary_board_state();
    auto post_move_confirmation_states =
        zobrist_component_registered_->confirmation_board_states();

    // confirm first call to UpdateBoardState caused state to change
    EXPECT_NE(initial_primary_state, post_move_primary_state);

    // perform same UpdateBoardState effect checks on confirmation calculators
    for (auto idx = 0; idx < NumConfKeys; ++idx) {
      EXPECT_NE(initial_confirmation_states[idx], post_move_confirmation_states[idx]);

      // also confirm confirmation state(s) differ from primary state
      EXPECT_NE(initial_confirmation_states[idx], initial_primary_state);
    }
  }

  //! For this test, need to be able to call ZobristCalculator->UpdateBoardState(), so
  //! instantiate ZobristCalculators using ZobristCalculatorFactory and then pass to
  //! static ZobristComponent::Build() method instead of using ZobristComponent created
  //! by ZobristComponentFactory.
  void TestUpdateBoardStateReversal() {
    auto primary_calculator =
        zobrist_calculator_factory_.CreateRegisteredCalculator(game_board_);
    std::array<std::shared_ptr<ZobristCalculatorType>, NumConfKeys>
        confirmation_calculators;
    for (auto idx = 0; idx < NumConfKeys; ++idx) {
      confirmation_calculators[idx] =
          zobrist_calculator_factory_.CreateRegisteredCalculator(game_board_);
    }

    // Create a ZobristComponent using our already initialized ZobristCalculator objects
    auto zobrist_component =
        ZobristComponentType::Create(primary_calculator, confirmation_calculators);

    // Get pre-move board states
    auto initial_primary_state = zobrist_component->primary_board_state();
    auto initial_confirmation_states = zobrist_component->confirmation_board_states();

    // Generate a Move and execute it. This should call UpdateBoardState of each
    // ZobristCalculator in ZobristComponent
    gameboard::BoardSpace move_start{6, 0};
    gameboard::BoardSpace move_end{5, 0};
    gameboard::Move move{move_start, move_end};
    auto executed_move = game_board_->ExecuteMove(move);

    // get post-move states
    auto post_move_primary_state = zobrist_component->primary_board_state();
    auto post_move_confirmation_states = zobrist_component->confirmation_board_states();

    // Call each ZobristCalculator's UpdateBoardState method again with same
    // ExecutedMove. Due to XOR nature of ZobristCalculator, this should take each back
    // to pre-move state.
    primary_calculator->UpdateBoardState(executed_move);
    for (auto calculator : confirmation_calculators) {
      calculator->UpdateBoardState(executed_move);
    }

    // Get final states
    auto final_primary_state = zobrist_component->primary_board_state();
    auto final_confirmation_states = zobrist_component->confirmation_board_states();

    // confirm first call to UpdateBoardState caused state to change
    EXPECT_NE(initial_primary_state, post_move_primary_state);
    // confirm second call (using same ExecutedMove)
    EXPECT_EQ(initial_primary_state, final_primary_state);

    // perform same UpdateBoardState effect checks on confirmation calculators
    for (auto idx = 0; idx < NumConfKeys; ++idx) {
      EXPECT_NE(initial_confirmation_states[idx], post_move_confirmation_states[idx]);
      EXPECT_EQ(initial_confirmation_states[idx], final_confirmation_states[idx]);

      // also confirm confirmation state(s) differ from primary state
      EXPECT_NE(initial_confirmation_states[idx], initial_primary_state);
      EXPECT_NE(post_move_confirmation_states[idx], post_move_primary_state);
    }
  }
};

//! Instantiates and runs methods of ZobristComponentTestSuite objects.
class ZobristComponentConceptTest : public ::testing::Test {
protected:
  std::vector<std::shared_ptr<ZobristComponentTestSuiteBase>> test_suites_;

  ZobristComponentConceptTest() {
    test_suites_.emplace_back(std::make_shared<ZobristComponentTestSuite<uint32_t, 0>>()
    );
    test_suites_.emplace_back(std::make_shared<ZobristComponentTestSuite<uint64_t, 0>>()
    );
    test_suites_.emplace_back(
        std::make_shared<ZobristComponentTestSuite<__uint128_t, 0>>()
    );
    test_suites_.emplace_back(std::make_shared<ZobristComponentTestSuite<uint32_t, 1>>()
    );
    test_suites_.emplace_back(std::make_shared<ZobristComponentTestSuite<uint64_t, 1>>()
    );
    test_suites_.emplace_back(
        std::make_shared<ZobristComponentTestSuite<__uint128_t, 1>>()
    );
  }
};

TEST_F(ZobristComponentConceptTest, CheckComplianceWithMultiBoardStateProviderConcept) {
  for (auto &suite : test_suites_) {
    suite->CheckComplianceWithMultiBoardStateProviderConcept();
  }
}

TEST_F(ZobristComponentConceptTest, TestCreateFromSeed) {
  for (auto &suite : test_suites_) {
    suite->TestCreateFromSeed();
  }
}

TEST_F(ZobristComponentConceptTest, TestCreateFromExistingCalculators) {
  for (auto &suite : test_suites_) {
    suite->TestCreateFromExistingCalculators();
  }
}

TEST_F(ZobristComponentConceptTest, TestCreateWithBuilder) {
  for (auto &suite : test_suites_) {
    suite->TestCreateWithBuilder();
  }
}

TEST_F(ZobristComponentConceptTest, TestGetters) {
  for (auto &suite : test_suites_) {
    suite->TestGetters();
  }
}

TEST_F(ZobristComponentConceptTest, TestNumConfKeys) {
  for (auto &suite : test_suites_) {
    suite->TestNumConfKeys();
  }
}

TEST_F(ZobristComponentConceptTest, TestUpdateBoardState) {
  for (auto &suite : test_suites_) {
    suite->TestUpdateBoardState();
  }
}

TEST_F(ZobristComponentConceptTest, TestUpdateBoardStateReveratl) {
  for (auto &suite : test_suites_) {
    suite->TestUpdateBoardStateReversal();
  }
}