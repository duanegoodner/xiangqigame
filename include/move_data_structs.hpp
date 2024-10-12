//! @file move_data_structs.hpp
//! Definitions and implementations of moves::Move and other move-related structs.

#pragma once

#include <board_data_structs.hpp>
#include <game_piece.hpp>

using namespace gameboard;

namespace moves {

//! A gameboard::BoardSpace pair (start and end).
//! Does not have a gameboard::GamePiece object associated with it (unlike a
//! moves::ExecutedMove). To know what gameboard::GamePiece would be associated with a
//! moves::Move, must compare with a gameboard::GameBoard.board_map_.
struct Move {
  gameboard::BoardSpace start;
  gameboard::BoardSpace end;

  bool operator==(const Move other) {
    return (start == other.start) && (end == other.end);
  }
};

//! A container for multiple moves::Move objects. Typically used to either hold every
//! moves::Move that *could* be made given a particular state of a gameboard::GameBoard,
//! or a collection of the *best* moves::Move objects, as determined by a MoveEvaluator.
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

  bool ContainsMove(const Move &move) const {
    for (auto entry : moves) {
      if ((move.start == entry.start) && (move.end == entry.end)) {
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

//! A change in the state of a gameboard::GameBoard represented by a moves::Move, and
//! each of the gameboard::GamePiece objects located at the **start** and **end**
//! locations of the Move. moves::ExecutedMove.moving_piece
//! is located at moves::ExecutedMove.spaces.start, and
//! moves::ExecutedMove.destication_piece is located at moves::ExecutedMove.spaces.end
//! *rior to the change in state*.
struct ExecutedMove {
  Move spaces;
  gameboard::GamePiece moving_piece;
  gameboard::GamePiece destination_piece;

  bool operator==(const ExecutedMove other) {
    return (other.spaces == spaces) && (other.moving_piece == moving_piece) &&
           (other.destination_piece == destination_piece);
  }
};

} // namespace moves