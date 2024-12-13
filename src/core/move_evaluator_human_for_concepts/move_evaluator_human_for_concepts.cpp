
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
    auto proposed_move = GetSyntacticallyValidMove(input_stream_);
    if (allowed_moves.ContainsMove(proposed_move)) {
      legal_move_proposed = true;
      legal_move = proposed_move;
    } else {
      io_messages_.NotifyIllegalMove();
    }
  }
  return legal_move;
}

void HumanMoveEvaluatorForConcepts::NotifyIllegalMove() {
  io_messages_.NotifyIllegalMove();
}

// Factory methods

std::unique_ptr<HumanMoveEvaluatorForConcepts> HumanMoveEvaluatorFactory::Create(
    gameboard::PieceColor evaluating_player,
    std::istream &input_stream
) {
  return HumanMoveEvaluatorForConcepts::Create(evaluating_player, input_stream);
}

gameboard::Move HumanMoveEvaluatorForConcepts::GetSyntacticallyValidMove(
    std::istream &input_stream
) {
  gameboard::Move result;

  bool obtained_syntactically_valid_move = false;

  while (!obtained_syntactically_valid_move) {
    io_messages_.DisplayInputPrompt();
    auto raw_input = movetranslation::GetInput(input_stream);
    auto input_tokens = movetranslation::Tokenize(raw_input);
    if (movetranslation::IsValidAlgebraicMove(input_tokens)) {
      obtained_syntactically_valid_move = true;
      auto algebraic_move = movetranslation::AlgebraicMove::Create(input_tokens);
      result = algebraic_move.ToGameBoardMove();
    } else {
      io_messages_.NotifyInvalidInput();
    }
  }

  return result;
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