//! @file move_data_structs.hpp 
//! Definitions and implementations of move-related structs.

#pragma once

#include <board_data_structs.hpp>

using namespace gameboard;

namespace moves {
struct Move {
  gameboard::BoardSpace start;
  gameboard::BoardSpace end;

  bool operator==(const Move other) {
    return (start == other.start) && (end == other.end);
  }
};

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