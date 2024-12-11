#include <concept_move_evaluator.hpp>
#include <gtest/gtest.h>
#include <move_data_structs.hpp>
#include <move_evaluator_human_for_concepts.hpp>

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