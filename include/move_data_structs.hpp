//! @file move_data_structs.hpp
//! Definitions and implementations of gameboard::Move and other move-related structs.

#pragma once

#include <board_data_structs.hpp>
#include <game_piece.hpp>

using namespace gameboard;

namespace gameboard {

//! A gameboard::BoardSpace pair (start and end).
//! Does not have a gameboard::GamePiece object associated with it (unlike a
//! gameboard::ExecutedMove). To know what gameboard::GamePiece would be associated with
//! a gameboard::Move, must compare with a gameboard::GameBoard.board_map_.
struct Move {
  gameboard::BoardSpace start;
  gameboard::BoardSpace end;

  bool operator==(const Move other) {
    return (start == other.start) && (end == other.end);
  }
};

//! A container for multiple gameboard::Move objects. Typically used to either hold every
//! gameboard::Move that *could* be made given a particular state of a
//! gameboard::GameBoard, or a collection of the *best* gameboard::Move objects, as
//! determined by a MoveEvaluator.
struct MoveCollection {
  vector<Move> moves;
  MoveCollection()
      : moves{} {};
  MoveCollection(vector<Move> my_moves)
      : moves{my_moves} {};
  MoveCollection(size_t reserve_size)
      : moves{} {
    moves.reserve(reserve_size);
  }

  size_t Size() const { return moves.size(); }

  bool IsEmpty() const { return moves.empty(); }

  bool ContainsMove(const Move &move) const {
    for (auto entry : moves) {
      if ((move.start == entry.start) && (move.end == entry.end)) {
        return true;
      }
    }
    return false;
  }

  bool ContainsAnyMoveNotIn(const MoveCollection &other) const {
    for (auto& entry : moves) {
      if (!other.ContainsMove(entry)) {
        return true;
      }
    }
    return false;
  }



  bool ContainsDestination(const gameboard::BoardSpace &space) {
    for (auto move : moves) {
      if (move.end == space) {
        return true;
      }
    }
    return false;
  }

  void Append(Move move) { moves.emplace_back(move); }
  void Concat(vector<Move> other_moves) {
    moves.insert(moves.end(), other_moves.begin(), other_moves.end());
  }
  void Concat(MoveCollection other) {
    moves.insert(moves.end(), other.moves.begin(), other.moves.end());
  }
};

//! A change in the state of a gameboard::GameBoard represented by a gameboard::Move, and
//! each of the gameboard::GamePiece objects located at the **start** and **end**
//! locations of the Move. gameboard::ExecutedMove.moving_piece
//! is located at gameboard::ExecutedMove.spaces.start, and
//! gameboard::ExecutedMove.destication_piece is located at
//! gameboard::ExecutedMove.spaces.end *rior to the change in state*.
struct ExecutedMove {
  Move spaces;
  gameboard::GamePiece moving_piece;
  gameboard::GamePiece destination_piece;
  MoveCountType moves_since_last_capture;

  //! == operator overload; requires equality of pieces and space, but not
  //! moves_since_last capture.
  bool operator==(const ExecutedMove other) {
    return (other.spaces == spaces) && (other.moving_piece == moving_piece) &&
           (other.destination_piece == destination_piece);
  }
};

} // namespace gameboard