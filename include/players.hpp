#pragma once

#include <game_board.hpp>
#include <game_piece.hpp>
#include <move_data_structs.hpp>
#include <move_evaluator_interface.hpp>
#include <player_interface.hpp>
#include <stdexcept>
#include <sstream>
#include <string>

class IllegalMoveException : public std::runtime_error {
public:
    IllegalMoveException(const Move& illegal_move)
        : std::runtime_error("Illegal move attempted: " + formatMove(illegal_move)) {}

private:
    static std::string formatMove(const Move& move) {
        std::ostringstream ss;
        ss << "from (" << move.start.rank << ", " << move.start.file
           << ") to (" << move.end.rank << ", " << move.end.file << ")";
        return ss.str();
    }
};

template <typename ConcreteMoveEvaluator>
class AIPlayer : public Player<AIPlayer> {
  gameboard::PieceColor piece_color_;
  PlayerType player_type_;
  EvaluatorType evaluator_type_;
  MoveEvaluator<ConcreteMoveEvaluator> &move_evaluator_;

public:
  AIPlayer(
      gameboard::PieceColor piece_color,
      EvaluatorType evaluator_type,
      MoveEvaluator &move_evaluator
  )
      : piece_color_{piece_color}
      , player_type_{kAI}
      , evaluator_type_{evaluator_type}
      , move_evaluator_{move_evaluator} {}

  gameboard::PieceColor implement_piece_color() { return piece_color_; }
  PlayerType implement_player_type() { return player_type_; }
  EvaluatorType implement_evaluator_type() { return evaluator_type_; }

  gameboard::Move ImplementProposeMove(const gameboard::MoveCollection &allowed_moves) {
    return MoveEvaluator.SelectMove(allowed_moves);
  }

  void ImplementIllegalMoveNoticeResponse(const Move &illegal_move) {
    throw IllegalMoveException(illegal_move);
  }
};
