//! @file gameboard/board_data_structs.hpp
//! Constants, typedefs, and simple structs used by gameboard::GameBoard.

#pragma once

#include <array>
#include <cassert>
#include <game_piece.hpp>
#include <integer_types.hpp>
#include <random>
#include <vector>

using namespace std;

namespace gameboard {

const BoardIndexType kNumRanks = 10;
const BoardIndexType kNumFiles = 9;

const BoardIndexType kRedRiverEdge = 5;
const BoardIndexType kBlackRiverEdge = 4;

//! Defines a castle feature in terms of its min / max rank and file.
struct CastleEdges {
  BoardIndexType min_rank;
  BoardIndexType max_rank;
  BoardIndexType min_file;
  BoardIndexType max_file;
};
constexpr CastleEdges kRedCastleEdges = {7, 9, 3, 5};
constexpr CastleEdges kBlackCastleEdges = {0, 2, 3, 5};

//! Descirbes a direction on a gameboard::GameBoard.board_map_.
struct BoardDirection {
  BoardIndexType rank, file;
};

inline PieceColor opponent_of(PieceColor color) {
  return static_cast<PieceColor>(-1 * color);
}

//! 2-D array of gameboard::GamePiece objects.
//! This is the data type of gameboard::GameBoard.board_map_.
typedef array<array<GamePiece, kNumFiles>, kNumRanks> BoardMap_t;
//! 2-D array of integers; can be converted to gameboard::BoardMap_t using
//! gameboard::int_board_to_game_pieces.
typedef array<array<int, kNumFiles>, kNumRanks> BoardMapInt_t;

//! A pair of coordinate (rank, and file) with properties determined by comparison with
//! values of gameboard features: board size, river locations, and castle locations.
struct BoardSpace {

  BoardIndexType rank, file;

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

inline PieceColor get_color(const BoardMap_t &board_map, const BoardSpace &space) {
  return board_map[space.rank][space.file].piece_color;
}

inline PieceType get_type(const BoardMap_t &board_map, const BoardSpace &space) {
  return board_map[space.rank][space.file].piece_type;
}

inline BoardSpace get_general_position(
    const BoardMap_t &board_map,
    const PieceColor color
) {
  auto castle =
      (color == PieceColor::kRed) ? red_castle_spaces() : black_castle_spaces();

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
} // namespace gameboard
