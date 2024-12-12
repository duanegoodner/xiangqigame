
#include <iostream>
#include <memory>
#include <move_evaluator_human_for_concepts.hpp>
#include <move_translator.hpp>

namespace moveselection {

// HumanMoveEvaluator methods

HumanMoveEvaluatorForConcepts::HumanMoveEvaluatorForConcepts(
    gameboard::PieceColor evaluating_player,
    std::istream &input_stream
)
    : evaluating_player_{evaluating_player}
    , io_messages_{}
    , input_stream_{input_stream} {}

std::unique_ptr<HumanMoveEvaluatorForConcepts> HumanMoveEvaluatorForConcepts::Create(
    gameboard::PieceColor evaluating_player,
    std::istream &input_stream
) {
  return std::unique_ptr<HumanMoveEvaluatorForConcepts>(
      new HumanMoveEvaluatorForConcepts(evaluating_player, input_stream)
  );
}

gameboard::Move HumanMoveEvaluatorForConcepts::SelectMove(
    gameboard::MoveCollection &allowed_moves
) {

  gameboard::Move legal_move;
  bool legal_move_proposed = false;

  while (!legal_move_proposed) {
    auto parsed_input = GetSyntacticallyValidInput(input_stream_);
    auto proposed_move = movetranslation::ConvertParsedInputToMove(parsed_input);
    if (allowed_moves.ContainsMove(proposed_move)) {
      legal_move_proposed = true;
      legal_move = proposed_move;
    } else {
      io_messages_.NotifyIllegalMove();
    }
  }
  return legal_move;
}

void HumanMoveEvaluatorForConcepts::IllegalMoveNotice() {
  io_messages_.NotifyInvalidInput();
}

// Factory methods

std::unique_ptr<HumanMoveEvaluatorForConcepts> HumanMoveEvaluatorFactory::Create(
    gameboard::PieceColor evaluating_player,
    std::istream &input_stream
) {
  return HumanMoveEvaluatorForConcepts::Create(evaluating_player, input_stream);
}

std::vector<std::string> HumanMoveEvaluatorForConcepts::GetSyntacticallyValidInput(
    std::istream &input_stream
) {
  std::vector<std::string> valid_input;

  while (valid_input.size() == 0) {
    io_messages_.DisplayInputPrompt();
    auto raw_input = movetranslation::GetInput(input_stream);
    auto parsed_input = movetranslation::ParseInput(raw_input);
    if (movetranslation::IsValidAlgebraicPair(parsed_input)) {
      valid_input = parsed_input;
    } else {
      io_messages_.NotifyInvalidInput();
    }
  }

  return valid_input;
}

} // namespace moveselection

namespace humanplayerio {
void InputRetrievalMessages::DisplayInputPrompt() {
  std::cout << input_prompt_ << std::endl;
}

void InputRetrievalMessages::NotifyInvalidInput() {
  std::cout << invalid_input_msg_ << std::endl;
}

void InputRetrievalMessages::NotifyIllegalMove() {
  std::cout << illegal_move_msg_ << std::endl;
}
} // namespace humanplayerio