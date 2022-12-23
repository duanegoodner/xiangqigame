#include "board_components.hpp"
#include "board_utilities_free.hpp"

using namespace board_utilities_free;
using namespace board_components;

bool board_utilities_free::is_occupied(BoardMap_t &board_map, BoardSpace space)
{
    return board_map[space.rank][space.file] != 0;
}

PieceColor board_utilities_free::get_color(BoardMap_t &board_map, BoardSpace space)
{
    auto piece = board_map[space.rank][space.file];
    return (piece == 0) ? PieceColor::kNul
                        : static_cast<PieceColor>(copysign(1, piece));
}

PieceType board_utilities_free::get_type(BoardMap_t &board_map, BoardSpace space)
{
    return static_cast<PieceType>(abs(board_map[space.rank][space.file]));
}

bool board_utilities_free::exists_and_passes_color_test(
    BoardMap_t &board_map, BoardSpace space, PieceColor moving_piece_color)
{
    return space.IsOnBoard() &&
           get_color(board_map, space) !=
               static_cast<PieceColor>(moving_piece_color);
}

BoardDirection board_utilities_free::fwd_direction(PieceColor color)
{
    return BoardDirection{static_cast<int>(color), 0};
}

BoardDirection board_utilities_free::rev_direction(PieceColor color)
{
    return BoardDirection{-1 * static_cast<int>(color), 0};
}

BoardSpace board_utilities_free::get_general_position(BoardMap_t &board_map, PieceColor color)
{
    auto castle = (color == PieceColor::kRed) ? red_castle_spaces() : black_castle_spaces();

    auto color_val = static_cast<int>(color);
    BoardSpace found_space;

    for (BoardSpace board_space : castle)
    {
        auto piece_val = board_map[board_space.rank][board_space.file];
        if (piece_val == color_val)
        {
            found_space = board_space;
        }
    }
    return found_space;
}

OrthogonalSpaceSearchResult board_utilities_free::search_spaces(
    BoardMap_t &board_map,
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

vector<BoardSpace> board_utilities_free::get_all_spaces_occupied_by(
    BoardMap_t &board_map, PieceColor color)
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

bool board_utilities_free::is_space_any_destination_of_moves(
    BoardMap_t &board_map, BoardSpace space, MoveCollection moves)
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
