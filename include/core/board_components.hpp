//! @file board_components.hpp
//! Constants, typedefs, and simple structs used by GameBoard.

#pragma once

#include <array>
#include <cassert>
#include <common.hpp>
#include <random>
#include <vector>

using namespace std;

namespace gameboard {

const BoardIdx_t kRedRiverEdge = 5;
const BoardIdx_t kBlackRiverEdge = 4;
struct CastleEdges {
  BoardIdx_t min_rank;
  BoardIdx_t max_rank;
  BoardIdx_t min_file;
  BoardIdx_t max_file;
};
constexpr CastleEdges kRedCastleEdges = {7, 9, 3, 5};
constexpr CastleEdges kBlackCastleEdges = {0, 2, 3, 5};
struct BoardDirection {
  BoardIdx_t rank, file;
};

struct GamePiece {
  PieceType piece_type;
  PieceColor piece_color;

  GamePiece() {
    piece_type = PieceType::kNnn;
    piece_color = PieceColor::kNul;
  }

  GamePiece(int int_piece) {
    piece_type = static_cast<PieceType>(abs(int_piece));
    piece_color = (int_piece == 0) ? PieceColor::kNul
                                   : static_cast<PieceColor>(copysign(1, int_piece));
  }

  GamePiece(PieceType type, PieceColor color) {
    piece_type = type;
    piece_color = color;
  }

  bool operator==(const GamePiece &other) const {
    return (piece_type == other.piece_type) && (piece_color == other.piece_color);
  }
};

inline PieceColor opponent_of(PieceColor color) {
  return static_cast<PieceColor>(-1 * color);
}

// We have two wayts to represent a board map:
// 1. As an array of GamePiece objects
typedef array<array<GamePiece, kNumFiles>, kNumRanks> BoardMap_t;
// 2. As an array of integers
typedef array<array<int, kNumFiles>, kNumRanks> BoardMapInt_t;

struct BoardSpace {

  BoardIdx_t rank, file;

  bool IsOnBoard() const {
    return ((0 <= rank) && (rank < kNumRanks) && (0 <= file) && (file < kNumFiles));
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
    auto castle_edges = color == PieceColor::kRed ? kRedCastleEdges : kBlackCastleEdges;
    return (castle_edges.min_rank <= rank) && (rank <= castle_edges.max_rank) &&
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

// Data struct for internal tracking of team's castle spaces as 1-D array
typedef array<BoardSpace, 9> Castle_t;

// Generates 1-D array of castle spaces from castle edge definitions
inline constexpr Castle_t calc_castle_spaces(const CastleEdges &edges) {
  Castle_t spaces{};

  for (auto rank = edges.min_rank; rank <= edges.max_rank; rank++) {
    for (auto file = edges.min_file; file <= edges.max_file; file++) {
      spaces[3 * (rank - edges.min_rank) + file - edges.min_file].rank = rank;
      spaces[3 * (rank - edges.min_rank) + file - edges.min_file].file = file;
    }
  }
  return spaces;
}

constexpr Castle_t red_castle_spaces() { return calc_castle_spaces(kRedCastleEdges); }

constexpr Castle_t black_castle_spaces() {
  return calc_castle_spaces(kBlackCastleEdges);
}

inline BoardMap_t int_board_to_game_pieces(const BoardMapInt_t int_board) {
  BoardMap_t game_piece_board;
  for (auto rank = 0; rank < kNumRanks; rank++) {
    for (auto file = 0; file < kNumFiles; file++) {
      game_piece_board[rank][file] = GamePiece(int_board[rank][file]);
    }
  }
  return game_piece_board;
}

inline bool is_occupied(const BoardMap_t &board_map, const BoardSpace &space) {
  return board_map[space.rank][space.file].piece_color != PieceColor::kNul;
}

inline PieceColor get_color(
    const BoardMap_t &board_map,
    const BoardSpace &space
) {
  return board_map[space.rank][space.file].piece_color;
}

inline PieceType get_type(
    const BoardMap_t &board_map,
    const BoardSpace &space
) {
  return board_map[space.rank][space.file].piece_type;
}

inline BoardSpace get_general_position(
    const BoardMap_t &board_map,
    const PieceColor color
) {
  auto castle = (color == PieceColor::kRed) ? red_castle_spaces()
                                            : black_castle_spaces();

  BoardSpace found_space;

  for (BoardSpace board_space : castle) {
    auto piece = board_map[board_space.rank][board_space.file];
    if (piece.piece_type == PieceType::kGen) {
      found_space = board_space;
    }
  }
  return found_space;
}

inline vector<BoardSpace> get_all_spaces_occupied_by(
    const BoardMap_t &board_map,
    const PieceColor color
) {
  vector<BoardSpace> occupied_spaces;
  occupied_spaces.reserve(16);
  for (auto rank = 0; rank < kNumRanks; rank++) {
    for (auto file = 0; file < kNumFiles; file++) {
      if (get_color(board_map, BoardSpace{rank, file}) == color) {
        occupied_spaces.emplace_back(BoardSpace{rank, file});
      }
    }
  }
  return occupied_spaces;
}
} // namespace board_components

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



}
