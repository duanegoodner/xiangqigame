#include "move_calculator.hpp"
#include "board_utilities_free.hpp"

#include <iostream>
#include <omp.h>

using namespace std;
using namespace board_utilities_free;

typedef MoveCollection (PieceMoves::*MethodPtr_t)(PieceColor, BoardSpace);

MoveCalculator::MoveCalculator(const BoardMap_t &board_map)
    : board_map_{board_map},
      piece_moves_{PieceMoves(*this)},
      utils_{BoardUtilities(board_map)}
{
    piece_dispatch_[PieceType::kSol] = &PieceMoves::SoldierMoves;
    piece_dispatch_[PieceType::kCan] = &PieceMoves::CannonMoves;
    piece_dispatch_[PieceType::kCha] = &PieceMoves::ChariotMoves;
    piece_dispatch_[PieceType::kHor] = &PieceMoves::HorseMoves;
    piece_dispatch_[PieceType::kEle] = &PieceMoves::ElephantMoves;
    piece_dispatch_[PieceType::kAdv] = &PieceMoves::AdvisorMoves;
    piece_dispatch_[PieceType::kGen] = &PieceMoves::GeneralMoves;
}

// https://opensource.com/article/21/2/ccc-method-pointers
// https://stackoverflow.com/questions/6265851
// https://stackoverflow.com/questions/55520876/
// https://en.cppreference.com/w/cpp/utility/any/any_cast
MoveCollection MoveCalculator::CalcMovesFrom(BoardSpace space)
{
    // auto piece_type = utils_.GetType(space);
    auto piece_type = get_type(board_map_, space);
    // auto color = utils_.GetColor(space);
    auto color = get_color(board_map_, space);
    auto move_func = piece_dispatch_.find(piece_type)->second;
    auto move_func_ptr = any_cast<MethodPtr_t>(move_func);
    return (piece_moves_.*move_func_ptr)(color, space);
}

MoveCollection MoveCalculator::CalcAllMovesNoCheckTest(PieceColor color)
{
    auto untested_moves = MoveCollection(120);
    // auto occ_spaces = utils_.GetAllSpacesOccupiedBy(color);
    auto occ_spaces = get_all_spaces_occupied_by(board_map_, color);
    for (auto space = 0; space < occ_spaces.size(); space++)
    {
        auto moves_from_space = CalcMovesFrom(occ_spaces[space]);
            untested_moves.Concat(moves_from_space);
    }
    return untested_moves;
}

PieceMoves::PieceMoves(MoveCalculator &p) : parent_{p} {};

MoveCollection PieceMoves::SoldierMoves(PieceColor color, BoardSpace space)
{
    auto soldier_moves = MoveCollection(3);

    // auto fwd_space = space + parent_.utils_.FwdDirection(color);
    auto fwd_space = space + fwd_direction(color);

    if (parent_.utils_.ExistsAndPassesColorTest(fwd_space, color))
    {
        soldier_moves.Append(Move{space, fwd_space});
    }

    if (not space.IsInHomelandOf(color))
    {
        for (auto side_vector : board_utilities_free::kSideDirections)
        {
            auto side_space = space + side_vector;
            if (parent_.utils_.ExistsAndPassesColorTest(side_space, color))
            {
                soldier_moves.Append(Move{space, side_space});
            }
        }
    }

    return soldier_moves;
}

MoveCollection PieceMoves::CannonMoves(PieceColor color, BoardSpace space)
{
    auto cannon_moves = MoveCollection(17);

    for (auto direction : board_utilities_free::kAllOrthogonalDirections)
    {
        auto search_result = parent_.utils_.SearchSpaces(space, direction);

        for (auto empty_space : search_result.empty_spaces)
        {
            cannon_moves.Append(Move{space, empty_space});
        }

        if (search_result.first_occupied_space.size())
        {
            auto second_search = parent_.utils_.SearchSpaces(
                search_result.first_occupied_space[0], direction);

            if (second_search.first_occupied_space.size() &&
                parent_.utils_.GetColor(second_search.first_occupied_space[0]) ==
                    opponent_of(color))
            {
                cannon_moves.Append(
                    Move{space, second_search.first_occupied_space[0]});
            }
        }
    }

    return cannon_moves;
}

MoveCollection PieceMoves::ChariotMoves(PieceColor color, BoardSpace space)
{
    auto chariot_moves = MoveCollection(17);
    for (auto direction : board_utilities_free::kAllOrthogonalDirections)
    {
        auto search_result = parent_.utils_.SearchSpaces(space, direction);
        for (auto empty_space : search_result.empty_spaces)
        {
            chariot_moves.Append(Move{space, empty_space});
        }
        if (search_result.first_occupied_space.size() &&
            parent_.utils_.GetColor(search_result.first_occupied_space[0]) ==
                opponent_of(color))
        {
            chariot_moves.Append(
                Move{space, search_result.first_occupied_space[0]});
        }
    }
    return chariot_moves;
}

MoveCollection PieceMoves::HorseMoves(PieceColor color, BoardSpace space)
{
    auto horse_moves = MoveCollection(8);

    for (auto direction : board_utilities_free::kHorsePaths)
    {
        auto first_step = space + direction.first;
        if (first_step.IsOnBoard() && (not parent_.utils_.IsOccupied(first_step)))
        {
            for (auto direction : direction.second)
            {
                auto second_step = first_step + direction;
                if (parent_.utils_.ExistsAndPassesColorTest(second_step, color))
                {
                    horse_moves.Append(Move{space, second_step});
                }
            }
        }
    }
    return horse_moves;
}

MoveCollection PieceMoves::ElephantMoves(PieceColor color, BoardSpace space)
{
    auto elephant_moves = MoveCollection(4);
    for (auto direction : board_utilities_free::kAllDiagonalDirections)
    {
        auto first_step = space + direction;
        if (first_step.IsOnBoard() && (not parent_.utils_.IsOccupied(first_step)) &&
            (first_step.IsInHomelandOf(color)))
        {
            auto second_step = first_step + direction;
            if (parent_.utils_.ExistsAndPassesColorTest(second_step, color))
            {
                elephant_moves.Append(Move{space, second_step});
            }
        }
    }
    return elephant_moves;
}

MoveCollection PieceMoves::AdvisorMoves(PieceColor color, BoardSpace space)
{
    auto advisor_moves = MoveCollection(4);
    for (auto direction : board_utilities_free::kAllDiagonalDirections)
    {
        auto destination = space + direction;
        if (destination.IsInCastleOf(color) &&
            (parent_.utils_.GetColor(destination) != color))
        {
            advisor_moves.Append(Move{space, destination});
        }
    }
    return advisor_moves;
}

MoveCollection PieceMoves::FlyingGeneralMove(PieceColor color,
                                             BoardSpace space)
{
    auto flying_move = MoveCollection(1);

    auto has_flying_move = true;
    auto opponent_color = opponent_of(color);
    auto other_gen_position = parent_.utils_.GetGeneralPosition(opponent_color);
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
            if (parent_.utils_.IsOccupied(BoardSpace{rank, space.file}))
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

MoveCollection PieceMoves::StandardGeneralMoves(PieceColor color,
                                                BoardSpace space)
{
    auto standard_general_moves = MoveCollection(4);

    for (auto direction :board_utilities_free::kAllOrthogonalDirections)
    {
        auto destination = space + direction;
        if (destination.IsInCastleOf(color) &&
            (parent_.utils_.GetColor(destination) != color))
        {
            standard_general_moves.Append(Move{space, destination});
        }
    }
    return standard_general_moves;
}

MoveCollection PieceMoves::GeneralMoves(PieceColor color, BoardSpace space)
{
    auto flying_move = FlyingGeneralMove(color, space);
    auto standard_moves = StandardGeneralMoves(color, space);
    auto general_moves = MoveCollection(flying_move.moves.size() + standard_moves.moves.size());
    general_moves.Concat(flying_move);
    general_moves.Concat(standard_moves);

    return general_moves;
}
