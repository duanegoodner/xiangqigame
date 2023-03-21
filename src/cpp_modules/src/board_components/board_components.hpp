
#ifndef _BOARD_COMPONENTS_
#define _BOARD_COMPONENTS_

#include <array>
#include <cassert>
#include <random>
#include <vector>
#include <common.hpp>

using namespace std;

namespace board_components {

struct GamePiece {
  PieceType piece_type;
  PieceColor piece_color;

  GamePiece() {
    piece_type = PieceType::kNnn;
    piece_color = PieceColor::kNul;
  }

  GamePiece(int int_piece) {
    piece_type = static_cast<PieceType>(abs(int_piece));
    piece_color = (int_piece == 0)
                      ? PieceColor::kNul
                      : static_cast<PieceColor>(copysign(1, int_piece));
  }

  GamePiece(PieceType type, PieceColor color) {
    piece_type = type;
    piece_color = color;
  }

  bool operator==(const GamePiece &other) const {
    return (piece_type == other.piece_type) &&
           (piece_color == other.piece_color);
  }
};

inline PieceColor opponent_of(PieceColor color) {
  return static_cast<PieceColor>(-1 * color);
}

// /////////////////////////
// Board Datatypes
// ////////////////////////
typedef array<array<GamePiece, kNumFiles>, kNumRanks> BoardMap_t;
typedef array<array<int, kNumFiles>, kNumRanks> BoardMapInt_t;


// /////////////////////////
// Board location tracking
// ////////////////////////

struct BoardSpace {

  BoardIdx_t rank, file;

  bool IsOnBoard() const {
    return (
        (0 <= rank) && (rank < kNumRanks) && (0 <= file) && (file < kNumFiles)
    );
  }

  bool IsInHomelandOf(const PieceColor color) const {
    assert(color != PieceColor::kNul);
    if (color == PieceColor::kRed) {
      return rank >= kRedRiverEdge;
    } else {
      return rank <= kBlackRiverEdge;
    }
  }

  bool IsInCastleOf(const PieceColor color) const {
    assert(color != PieceColor::kNul);
    auto castle_edges =
        color == PieceColor::kRed ? kRedCastleEdges : kBlackCastleEdges;
    return (castle_edges.min_rank <= rank) &&
           (rank <= castle_edges.max_rank) &&
           (castle_edges.min_file <= file) && (file <= castle_edges.max_file);
  }

  BoardSpace operator+(const BoardDirection direction) const {
    return BoardSpace{rank + direction.rank, file + direction.file};
  }

  bool operator==(const BoardSpace other) const {
    return (rank == other.rank) && (file == other.file);
  }

  bool operator!=(const BoardSpace other) const {
    return (rank != other.rank) || (file != other.file);
  }
};

struct Move {
  BoardSpace start;
  BoardSpace end;

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

  bool ContainsMove(const Move &move) const {
    for (auto entry : moves) {
      if ((move.start == entry.start) && (move.end == entry.end)) {
        return true;
      }
    }
    return false;
  }

  bool ContainsDestination(const BoardSpace& space) {
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
  GamePiece moving_piece;
  GamePiece destination_piece;
};

// ////////////////////////
// Convenience dataype and functions for generating array of castle spaces
// ////////////////////////
typedef array<BoardSpace, 9> Castle_t;
inline constexpr Castle_t calc_castle_spaces(const CastleEdges &edges) {
  std::array<BoardSpace, 9> spaces{};

  for (auto rank = edges.min_rank; rank <= edges.max_rank; rank++) {
    for (auto file = edges.min_file; file <= edges.max_file; file++) {
      spaces[3 * (rank - edges.min_rank) + file - edges.min_file].rank = rank;
      spaces[3 * (rank - edges.min_rank) + file - edges.min_file].file = file;
    }
  }
  return spaces;
}

constexpr Castle_t red_castle_spaces() {
  return calc_castle_spaces(kRedCastleEdges);
}

constexpr Castle_t black_castle_spaces() {
  return calc_castle_spaces(kBlackCastleEdges);
}
} // namespace board_components

#endif // _SHARED_COMPONENTS_