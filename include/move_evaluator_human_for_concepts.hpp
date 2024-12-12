#pragma once

#include <game_piece.hpp>
#include <iostream>
#include <memory>
#include <move_data_structs.hpp>
#include <string>

namespace humanplayerio {
class InputRetrievalMessages {
  std::string input_prompt_{"Enter a move in the form 'from_space, to_space': "};
  std::string invalid_input_msg_{"Invalid input"};
  std::string illegal_move_msg_{"Illegal move. Please enter a different move."};

public:
  void DisplayInputPrompt();
  void NotifyInvalidInput();
  void NotifyIllegalMove();
};
} // namespace humanplayerio

namespace moveselection {
class HumanMoveEvaluatorForConcepts {
  gameboard::PieceColor evaluating_player_;
  humanplayerio::InputRetrievalMessages io_messages_;
  std::istream &input_stream_;

public:
  static std::unique_ptr<HumanMoveEvaluatorForConcepts> Create(
      gameboard::PieceColor evaluating_player,
      std::istream &input_stream = std::cin

  );

  gameboard::Move SelectMove(gameboard::MoveCollection &allowed_moves);
  void IllegalMoveNotice();

private:
  HumanMoveEvaluatorForConcepts(
      gameboard::PieceColor evaluating_player,
      std::istream &input_stream
  );
  std::vector<std::string> GetSyntacticallyValidInput(std::istream &input_stream);
};

class HumanMoveEvaluatorFactory {
public:
  std::unique_ptr<HumanMoveEvaluatorForConcepts> Create(
      gameboard::PieceColor evaluating_player,
      std::istream &input_stream = std::cin
  );
};
} // namespace moveselection
