#include <iostream>
#include <move_calculator.hpp>
#include <board_utilities.hpp>


using namespace std;
using namespace board_utilities;


MoveCollection PieceMoves::SoldierMoves(
    const BoardMap_t &board_map, PieceColor color, BoardSpace space)
{
    auto soldier_moves = MoveCollection(3);

    auto fwd_space = space + fwd_direction(color);

    if (exists_and_passes_color_test(board_map, fwd_space, color))
    {
        soldier_moves.Append(Move{space, fwd_space});
    }

    if (not space.IsInHomelandOf(color))
    {
        for (auto side_vector : board_utilities::kSideDirections)
        {
            auto side_space = space + side_vector;
            if (exists_and_passes_color_test(board_map, fwd_space, color))
            {
                soldier_moves.Append(Move{space, side_space});
            }
        }
    }

    return soldier_moves;
}

MoveCollection PieceMoves::CannonMoves(
    const BoardMap_t &board_map, PieceColor color, BoardSpace space)
{
    auto cannon_moves = MoveCollection(17);

    for (auto direction : board_utilities::kAllOrthogonalDirections)
    {
        auto search_result = search_spaces(board_map, space, direction);

        for (auto empty_space : search_result.empty_spaces)
        {
            cannon_moves.Append(Move{space, empty_space});
        }

        if (search_result.first_occupied_space.size())
        {
            auto second_search = search_spaces(
                board_map, search_result.first_occupied_space[0], direction);

            if (second_search.first_occupied_space.size() &&
                get_color(board_map, second_search.first_occupied_space[0]) ==
                    opponent_of(color))
            {
                cannon_moves.Append(
                    Move{space, second_search.first_occupied_space[0]});
            }
        }
    }

    return cannon_moves;
}

MoveCollection PieceMoves::ChariotMoves(
    const BoardMap_t &board_map, PieceColor color, BoardSpace space)
{
    auto chariot_moves = MoveCollection(17);
    for (auto direction : board_utilities::kAllOrthogonalDirections)
    {
        auto search_result = search_spaces(board_map, space, direction);
        for (auto empty_space : search_result.empty_spaces)
        {
            chariot_moves.Append(Move{space, empty_space});
        }
        if (search_result.first_occupied_space.size() &&
            get_color(board_map, search_result.first_occupied_space[0]) ==
                opponent_of(color))
        {
            chariot_moves.Append(
                Move{space, search_result.first_occupied_space[0]});
        }
    }
    return chariot_moves;
}

MoveCollection PieceMoves::HorseMoves(
    const BoardMap_t &board_map, PieceColor color, BoardSpace space)
{
    auto horse_moves = MoveCollection(8);

    for (auto direction : board_utilities::kHorsePaths)
    {
        auto first_step = space + direction.first;
        if (first_step.IsOnBoard() && (not is_occupied(board_map, first_step)))
        {
            for (auto direction : direction.second)
            {
                auto second_step = first_step + direction;
                if (exists_and_passes_color_test(board_map, second_step, color))

                {
                    horse_moves.Append(Move{space, second_step});
                }
            }
        }
    }
    return horse_moves;
}

MoveCollection PieceMoves::ElephantMoves(
    const BoardMap_t &board_map, PieceColor color, BoardSpace space)
{
    auto elephant_moves = MoveCollection(4);
    for (auto direction : board_utilities::kAllDiagonalDirections)
    {
        auto first_step = space + direction;
        if (first_step.IsOnBoard() && (not is_occupied(board_map, first_step)) &&
            (first_step.IsInHomelandOf(color)))
        {
            auto second_step = first_step + direction;
            if (exists_and_passes_color_test(board_map, second_step, color))
            {
                elephant_moves.Append(Move{space, second_step});
            }
        }
    }
    return elephant_moves;
}

MoveCollection PieceMoves::AdvisorMoves(
    const BoardMap_t &board_map, PieceColor color, BoardSpace space)
{
    auto advisor_moves = MoveCollection(4);
    for (auto direction : board_utilities::kAllDiagonalDirections)
    {
        auto destination = space + direction;
        if (destination.IsInCastleOf(color) &&
            (get_color(board_map, destination) != color))

        {
            advisor_moves.Append(Move{space, destination});
        }
    }
    return advisor_moves;
}

MoveCollection PieceMoves::FlyingGeneralMove(
    const BoardMap_t &board_map, PieceColor color, BoardSpace space)
{
    auto flying_move = MoveCollection(1);

    auto has_flying_move = true;
    auto opponent_color = opponent_of(color);
    auto other_gen_position = get_general_position(board_map, opponent_color);
    if (space.file != other_gen_position.file)
    {
        has_flying_move = false;
    }
    else
    {
        auto search_start = min(space.rank, other_gen_position.rank) + 1;
        auto search_end = max(space.rank, other_gen_position.rank) - 1;
        for (auto rank = search_start; rank <= search_end; rank++)
        {
            if (is_occupied(board_map, BoardSpace{rank, space.file}))
            {
                has_flying_move = false;
            }
        }
    }

    if (has_flying_move)
    {
        flying_move.Append(Move{space, other_gen_position});
    }

    return flying_move;
}

MoveCollection PieceMoves::StandardGeneralMoves(
    const BoardMap_t &board_map, PieceColor color, BoardSpace space)
{
    auto standard_general_moves = MoveCollection(4);

    for (auto direction :board_utilities::kAllOrthogonalDirections)
    {
        auto destination = space + direction;
         if (destination.IsInCastleOf(color) &&
            (get_color(board_map, destination) != color))
        {
            standard_general_moves.Append(Move{space, destination});
        }
    }
    return standard_general_moves;
}

MoveCollection PieceMoves::GeneralMoves(
    const BoardMap_t &board_map, PieceColor color, BoardSpace space
)
{
    auto flying_move = FlyingGeneralMove(board_map, color, space);
    auto standard_moves = StandardGeneralMoves(board_map, color, space);
    auto general_moves = MoveCollection(flying_move.moves.size() + standard_moves.moves.size());
    general_moves.Concat(flying_move);
    general_moves.Concat(standard_moves);

    return general_moves;
}
