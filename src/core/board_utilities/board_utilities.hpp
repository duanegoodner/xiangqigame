// Filename: board_components.hpp
// Author: Duane Goodner
// Created: 2022-12-02
// Last Modified: 2024-09-30

// Description:
// Contains functions for obtaining info about the state of a board map.
// Functions are inlined to improve speed.

#ifndef _BOARD_UTILITIES_FREE_
#define _BOARD_UTILITIES_FREE_

#include <board_components.hpp>
#include <common.hpp>

// ///////////////////////////////////////////////////////////////////////////////////////
// All board_utilities functions defined in header to allow inlining
// (perfomance improvement)
// ///////////////////////////////////////////////////////////////////////////////////////

namespace board_utilities {
using namespace board_components;

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

} // namespace board_utilities

#endif