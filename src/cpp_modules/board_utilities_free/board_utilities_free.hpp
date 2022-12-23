#ifndef _BOARD_UTILITIES_FREE_
#define _BOARD_UTILITIES_FREE_

#include "board_components.hpp"

namespace board_utilities_free {
    using namespace board_components;

    struct OrthogonalSpaceSearchResult
    {
        vector<BoardSpace> empty_spaces;
        vector<BoardSpace> first_occupied_space;
    };

    const array<BoardDirection, 2> kSideDirections = {BoardDirection{0, 1},
                                                      BoardDirection{0, -1}};

    const vector<pair<BoardDirection, vector<BoardDirection>>> kHorsePaths = {
        {BoardDirection{1, 0}, {BoardDirection{1, 1}, BoardDirection{1, -1}}},
        {BoardDirection{-1, 0}, {BoardDirection{-1, 1}, BoardDirection{-1, -1}}},
        {BoardDirection{0, 1}, {BoardDirection{1, 1}, BoardDirection{-1, 1}}},
        {BoardDirection{0, -1}, {BoardDirection{1, -1}, BoardDirection{-1, -1}}}};

    const vector<BoardDirection> kAllOrthogonalDirections = {
        BoardDirection{0, 1}, BoardDirection{0, -1}, BoardDirection{1, 0},
        BoardDirection{-1, 0}};

    const vector<BoardDirection> kAllDiagonalDirections = {
        BoardDirection{1, 1}, BoardDirection{1, -1}, BoardDirection{-1, 1},
        BoardDirection{-1, -1}};

    bool is_occupied(const BoardMap_t &board_map, BoardSpace space);
    PieceColor get_color(const BoardMap_t &board_map, BoardSpace space);
    PieceType get_type(const BoardMap_t &board_map, BoardSpace space);
    bool exists_and_passes_color_test(
        const BoardMap_t &board_map, BoardSpace space, PieceColor moving_piece_color);
    BoardDirection fwd_direction(PieceColor color);
    BoardDirection rev_direction(PieceColor color);
    BoardSpace get_general_position(const BoardMap_t &board_map, PieceColor color);
    OrthogonalSpaceSearchResult search_spaces(
        const BoardMap_t &board_map,
        BoardSpace start,
        BoardDirection direction);
    vector<BoardSpace> get_all_spaces_occupied_by(
        const BoardMap_t &board_map, PieceColor color);
    bool is_space_any_destination_of_moves(
        const BoardMap_t &board_map, BoardSpace space, MoveCollection moves);
}

#endif