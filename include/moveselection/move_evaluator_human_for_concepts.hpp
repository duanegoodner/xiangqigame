#pragma once

#include <interfaces/base_evaluator_factory.hpp>
#include <interfaces/base_move_evaluator.hpp>
#include <gameboard/game_piece.hpp>
#include <iostream>
#include <memory>
#include <gameboard/move_data_structs.hpp>
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
class HumanMoveEvaluatorForConcepts : public MoveEvaluatorBase {
  gameboard::PieceColor evaluating_player_;
  humanplayerio::InputRetrievalMessages io_messages_;
  std::istream &input_stream_;

public:
  static std::unique_ptr<HumanMoveEvaluatorForConcepts> Create(
      gameboard::PieceColor evaluating_player,
      std::istream &input_stream = std::cin
  );

  gameboard::Move SelectMove(const gameboard::MoveCollection &allowed_moves);
  void NotifyIllegalMove();

private:
  HumanMoveEvaluatorForConcepts(
      gameboard::PieceColor evaluating_player,
      std::istream &input_stream
  );
  gameboard::Move GetSyntacticallyValidMove(std::istream &input_stream);
};

class HumanMoveEvaluatorFactory : public MoveEvaluatorFactoryBase {
  std::istream &input_stream_;

public:
  HumanMoveEvaluatorFactory(std::istream &input_stream = std::cin)
      : input_stream_{input_stream} {}
  std::unique_ptr<MoveEvaluatorBase> Create(gameboard::PieceColor evaluating_player
  ) override;
};
} // namespace moveselection
