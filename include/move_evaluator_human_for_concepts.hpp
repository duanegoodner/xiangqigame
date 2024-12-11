#pragma once

#include <game_piece.hpp>
#include <iostream>
#include <memory>
#include <move_data_structs.hpp>
#include <string>

namespace moveselection {
class HumanMoveEvaluatorForConcepts {
  gameboard::PieceColor evaluating_player_;

public:
  static std::unique_ptr<HumanMoveEvaluatorForConcepts> Create(
      gameboard::PieceColor evaluating_player
  );

  gameboard::Move SelectMove(gameboard::MoveCollection &allowed_moves);
  std::string GetInput(std::istream &input_stream);
  

private:
  HumanMoveEvaluatorForConcepts(gameboard::PieceColor evaluating_player);
  
};

class HumanMoveEvaluatorFactory {
    public:
    std::unique_ptr<HumanMoveEvaluatorForConcepts> Create(
        gameboard::PieceColor evaluating_player
    );
};
} // namespace moveselection
