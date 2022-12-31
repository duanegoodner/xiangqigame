#ifndef _BOARD_UTILITIES_FREE_
#define _BOARD_UTILITIES_FREE_

#include <board_components.hpp>

// ///////////////////////////////////////////////////////////////////////////////////////
// All board_utilities functions defined in header to allow inlining (perfomance improvement)
// ///////////////////////////////////////////////////////////////////////////////////////

namespace board_utilities
{
    using namespace board_components;

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

    inline bool is_occupied(const BoardMap_t &board_map, const BoardSpace& space)
    {
        return board_map[space.rank][space.file].piece_color != PieceColor::kNul;
    }

    inline PieceColor get_color(const BoardMap_t &board_map, const BoardSpace& space)
    {
        return board_map[space.rank][space.file].piece_color;
    }

    inline PieceType get_type(const BoardMap_t &board_map, const BoardSpace& space)
{
        return board_map[space.rank][space.file].piece_type;

//        return static_cast<PieceType>(abs(board_map[space.rank][space.file]));
}

    inline bool exists_and_passes_color_test(
    const BoardMap_t &board_map, const BoardSpace &space, PieceColor moving_piece_color)
{
    return space.IsOnBoard() &&
           get_color(board_map, space) !=
               static_cast<PieceColor>(moving_piece_color);
}

    inline BoardDirection fwd_direction(PieceColor color)
{
    return BoardDirection{static_cast<int>(color), 0};
}

    inline BoardDirection rev_direction(PieceColor color)
{
    return BoardDirection{-1 * static_cast<int>(color), 0};
}

    inline BoardSpace get_general_position(
    const BoardMap_t &board_map, const PieceColor color)
{
    auto castle = (color == PieceColor::kRed) ? red_castle_spaces() : black_castle_spaces();

    // auto color_val = static_cast<int>(color);
    BoardSpace found_space;

    for (BoardSpace board_space : castle)
    {
        auto piece = board_map[board_space.rank][board_space.file];
        if (piece.piece_type == PieceType::kGen)
        {
            found_space = board_space;
        }
    }
    return found_space;
}

    inline vector<BoardSpace> get_all_spaces_occupied_by(
        const BoardMap_t &board_map, const PieceColor color)
    {
        vector<BoardSpace> occupied_spaces;
        occupied_spaces.reserve(16);
        for (auto rank = 0; rank < kNumRanks; rank++)
        {
            for (auto file = 0; file < kNumFiles; file++)
            {
                // auto cur_color = get_color(board_map, BoardSpace{rank, file});
                if (get_color(board_map, BoardSpace{rank, file}) == color)
                {
                    occupied_spaces.emplace_back(BoardSpace{rank, file});
                }
            }
        }
        return occupied_spaces;
    }

    inline bool is_space_any_destination_of_moves(
    const BoardSpace &space, const MoveCollection &moves)
{
    for (auto move : moves.moves)
    {
        if (move.end == space)
        {
            return true;
        }
    }
    return false;
}
}

#endif