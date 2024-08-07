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