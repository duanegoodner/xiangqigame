#include <concept_move_evaluator.hpp>
#include <gtest/gtest.h>
#include <iostream>
#include <move_data_structs.hpp>
#include <move_evaluator_human_for_concepts.hpp>

class InputRetrievalMessagesTest : public ::testing::Test {
protected:
  humanplayerio::InputRetrievalMessages io_messages_{};
};

TEST_F(InputRetrievalMessagesTest, TestDisplayInputPrompt) {
  io_messages_.DisplayInputPrompt();
}

TEST_F(InputRetrievalMessagesTest, TestNotifyInvalidInput) {
  io_messages_.NotifyInvalidInput();
}

TEST_F(InputRetrievalMessagesTest, TestNotifyIllegalMove) {
  io_messages_.NotifyIllegalMove();
}

class HumanMoveEvaluatorTest : public ::testing::Test {
protected:
  moveselection::HumanMoveEvaluatorFactory human_move_evaluator_factory_;
};

TEST_F(HumanMoveEvaluatorTest, TestCreateWithStaticFactoryMethod) {
  auto human_move_evaluator_red =
      moveselection::HumanMoveEvaluatorForConcepts::Create(gameboard::PieceColor::kRed);
}

TEST_F(HumanMoveEvaluatorTest, TestCreateWithFactoryClass) {
  auto human_move_evaluator_red =
      human_move_evaluator_factory_.Create(gameboard::PieceColor::kRed);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}