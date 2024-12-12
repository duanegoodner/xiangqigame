#include <concept_move_evaluator.hpp>
#include <gtest/gtest.h>
#include <iostream>
#include <move_data_structs.hpp>
#include <move_evaluator_human_for_concepts.hpp>
#include <move_translator.hpp>
#include <sstream>

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

  gameboard::MoveCollection dummy_allowed_moves_;

  std::string red_horse_move_input_str_{"b1, c3"};

  std::istringstream red_horse_move_input_stream_;

  void SetUp() override {
    red_horse_move_input_stream_.str(red_horse_move_input_str_);
    auto parsed_red_horse_move = movetranslation::Tokenize(red_horse_move_input_str_);
    
    auto red_horse_move_tokens =
        movetranslation::Tokenize(red_horse_move_input_str_);
    auto red_horse_algebraic_move = movetranslation::AlgebraicMove::Create(red_horse_move_tokens);
    auto red_horse_game_board_move = red_horse_algebraic_move.ToGameBoardMove();
    dummy_allowed_moves_.Append(red_horse_game_board_move);
  }
};

TEST_F(HumanMoveEvaluatorTest, TestCreateWithStaticFactoryMethod) {
  auto human_move_evaluator_red =
      moveselection::HumanMoveEvaluatorForConcepts::Create(gameboard::PieceColor::kRed);
}

TEST_F(HumanMoveEvaluatorTest, TestCreateWithFactoryClass) {
  auto human_move_evaluator_red =
      human_move_evaluator_factory_.Create(gameboard::PieceColor::kRed);
}

TEST_F(HumanMoveEvaluatorTest, TestCreateWithIstringStreamInput) {
  auto human_move_evaluator_red = human_move_evaluator_factory_.Create(
      gameboard::PieceColor::kRed,
      red_horse_move_input_stream_
  );
}

TEST_F(HumanMoveEvaluatorTest, TestSelectMoveWithIstringStreamInput) {

  auto human_move_evaluator_red = human_move_evaluator_factory_.Create(
      gameboard::PieceColor::kRed,
      red_horse_move_input_stream_
  );

  auto result = human_move_evaluator_red->SelectMove(dummy_allowed_moves_);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}