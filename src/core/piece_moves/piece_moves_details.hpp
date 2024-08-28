#ifndef B5EFD030_08BC_4F78_AD84_F124D30BB6C8
#define B5EFD030_08BC_4F78_AD84_F124D30BB6C8

#include <board_components.hpp>
#include <board_utilities.hpp>
using namespace board_components;
using namespace board_utilities;

const array<BoardDirection, 2> kSideDirections = {
    BoardDirection{0, 1},
    BoardDirection{0, -1}};

const vector<pair<BoardDirection, vector<BoardDirection>>> kHorsePaths = {
    {BoardDirection{1, 0}, {BoardDirection{1, 1}, BoardDirection{1, -1}}},
    {BoardDirection{-1, 0}, {BoardDirection{-1, 1}, BoardDirection{-1, -1}}},
    {BoardDirection{0, 1}, {BoardDirection{1, 1}, BoardDirection{-1, 1}}},
    {BoardDirection{0, -1}, {BoardDirection{1, -1}, BoardDirection{-1, -1}}}};

const vector<BoardDirection> kAllOrthogonalDirections = {
    BoardDirection{0, 1},
    BoardDirection{0, -1},
    BoardDirection{1, 0},
    BoardDirection{-1, 0}};

const vector<BoardDirection> kAllDiagonalDirections = {
    BoardDirection{1, 1},
    BoardDirection{1, -1},
    BoardDirection{-1, 1},
    BoardDirection{-1, -1}};

inline BoardDirection fwd_direction(PieceColor color) {
  return BoardDirection{static_cast<int>(color), 0};
}

inline bool exists_and_passes_color_test(
    const BoardMap_t &board_map,
    const BoardSpace &space,
    PieceColor moving_piece_color
) {
  return space.IsOnBoard() && get_color(board_map, space) !=
                                  static_cast<PieceColor>(moving_piece_color);
}

#endif /* B5EFD030_08BC_4F78_AD84_F124D30BB6C8 */
