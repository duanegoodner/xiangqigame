#ifndef _BOARD_UTILITIES_FREE_
#define _BOARD_UTILITIES_FREE_

#include "board_components.hpp"

namespace board_utilities_free
{
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

    inline bool is_occupied(const BoardMap_t &board_map, BoardSpace space)
    {
        return board_map[space.rank][space.file] != 0;
    }

    inline PieceColor get_color(const BoardMap_t &board_map, BoardSpace space)
    {
        auto piece = board_map[space.rank][space.file];
        return (piece == 0) ? PieceColor::kNul
                            : static_cast<PieceColor>(copysign(1, piece));
    }

    PieceType get_type(const BoardMap_t &board_map, BoardSpace space);
    bool exists_and_passes_color_test(
        const BoardMap_t &board_map, BoardSpace space, PieceColor moving_piece_color);
    BoardDirection fwd_direction(PieceColor color);
    BoardDirection rev_direction(PieceColor color);
    BoardSpace get_general_position(const BoardMap_t &board_map, PieceColor color);

    inline OrthogonalSpaceSearchResult search_spaces(
        const BoardMap_t &board_map,
        BoardSpace start,
        BoardDirection direction)
    {
        vector<BoardSpace> empty_spaces;
        empty_spaces.reserve(9);
        vector<BoardSpace> first_occupied_space;
        first_occupied_space.reserve(1);

        auto next_step = start + direction;

        while (next_step.IsOnBoard() && (not is_occupied(board_map, next_step)))
        {
            empty_spaces.emplace_back(next_step);
            next_step = next_step + direction;
        }
        if (next_step.IsOnBoard())
        {
            first_occupied_space.emplace_back(next_step);
        }
        return OrthogonalSpaceSearchResult{empty_spaces, first_occupied_space};
    }

    inline vector<BoardSpace> get_all_spaces_occupied_by(
        const BoardMap_t &board_map, PieceColor color)
    {
        vector<BoardSpace> occupied_spaces;
        occupied_spaces.reserve(16);
        for (auto rank = 0; rank < kNumRanks; rank++)
        {
            for (auto file = 0; file < kNumFiles; file++)
            {
                if (get_color(board_map, BoardSpace{rank, file}) == color)
                {
                    occupied_spaces.emplace_back(BoardSpace{rank, file});
                }
            }
        }
        return occupied_spaces;
    }

    bool is_space_any_destination_of_moves(
        BoardSpace space, MoveCollection moves);
}

#endif