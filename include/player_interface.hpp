#pragma once

#include <game_board.hpp>
#include <game_piece.hpp>
#include <move_data_structs.hpp>
#include <space_info_provider_interface.hpp>

enum PlayerType : int { kHuman = 0, kAI = 1, kScripted = 2 };
enum EvaluatorType : int { kNullEvaluator = 0, kMinimax = 1, kRandom = 2 };

template <typename ConcretePlayer>
class Player {
public:
  gameboard::PieceColor piece_color() {
    return static_cast<ConcretePlayer *>(this)->implement_piece_color();
  }

  PlayerType player_type() {
    return static_cast<ConcretePlayer *>(this)->implement_player_type();
  }

  EvaluatorType evaluator_type() {
    return static_cast<ConcretePlayer *>(this)->implement_evaluator_type();
  }

  gameboard::Move ProposeMove(const gameboard::MoveCollection &allowed_moves) {
    return static_cast<ConcretePlayer *>(this)->ImplementProposeMove(allowed_moves);
  }

  void IllegalMoveNoticeResponse(const gameboard::Move &illegal_move) {
    return static_cast<ConcretePlayer *>(this)->ImplementIllegalMoveNoticeResponse(
        illegal_move
    );
  }
};
