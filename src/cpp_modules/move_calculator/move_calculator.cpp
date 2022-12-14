#include "move_calculator.hpp"

#include <iostream>
#include <omp.h>

using namespace std;

typedef MoveCollection (PieceMoves::*MethodPtr_t)(PieceColor, BoardSpace);

MoveCalculator::MoveCalculator(const BoardMap_t& board_map)
    : board_map_{board_map},
      piece_moves_{PieceMoves(*this)},
      utils_{BoardUtilities(board_map)} {
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
MoveCollection MoveCalculator::CalcMovesFrom(BoardSpace space) {
    auto piece_type = utils_.GetType(space);
    auto color = utils_.GetColor(space);
    auto move_func = piece_dispatch_.find(piece_type)->second;
    auto move_func_ptr = any_cast<MethodPtr_t>(move_func);
    return (piece_moves_.*move_func_ptr)(color, space);
}

MoveCollection MoveCalculator::CalcAllMovesNoCheckTest(PieceColor color) {
    MoveCollection untested_moves;
    untested_moves.moves.reserve(120);
    
    auto occ_spaces = utils_.GetAllSpacesOccupiedBy(color);

    for (auto space = 0; space < occ_spaces.size(); space++) {
        auto moves_from_space = CalcMovesFrom(occ_spaces[space]);
        untested_moves.moves.insert(untested_moves.moves.end(), moves_from_space.moves.begin(),
                              moves_from_space.moves.end());
    }
    return untested_moves;
}


PieceMoves::PieceMoves(MoveCalculator & p) : parent_{p} {};

MoveCollection PieceMoves::SoldierMoves(PieceColor color, BoardSpace space) {
    MoveCollection soldier_moves;
    soldier_moves.moves.reserve(3);

    auto fwd_space = space + parent_.utils_.FwdDirection(color);

    if (parent_.utils_.ExistsAndPassesColorTest(fwd_space, color)) {
        soldier_moves.Append(Move{space, fwd_space});
    }

    if (not space.IsInHomelandOf(color)) {
        for (auto side_vector : kSideDirections) {
            auto side_space = space + side_vector;
            if (parent_.utils_.ExistsAndPassesColorTest(side_space, color)) {
                soldier_moves.Append(Move{space, side_space});
            }
        }
    }

    return soldier_moves;
}

MoveCollection PieceMoves::CannonMoves(PieceColor color, BoardSpace space) {
    MoveCollection cannon_moves;
    cannon_moves.moves.reserve(17);
    for (auto direction : kAllOrthogonalDirections) {
        auto search_result = parent_.utils_.SearchSpaces(space, direction);

        for (auto empty_space : search_result.empty_spaces) {
            cannon_moves.Append(Move{space, empty_space});
        }

        if (search_result.first_occupied_space.size()) {
            auto second_search = parent_.utils_.SearchSpaces(
                search_result.first_occupied_space[0], direction);

            if (second_search.first_occupied_space.size() &&
                parent_.utils_.GetColor(second_search.first_occupied_space[0]) ==
                    opponent_of(color)) {
                cannon_moves.Append(
                    Move{space, second_search.first_occupied_space[0]});
            }
        }
    }

    return cannon_moves;
}

MoveCollection PieceMoves::ChariotMoves(PieceColor color, BoardSpace space) {
    MoveCollection chariot_moves;
    chariot_moves.moves.reserve(17);
    
    for (auto direction : kAllOrthogonalDirections) {
        auto search_result = parent_.utils_.SearchSpaces(space, direction);
        for (auto empty_space : search_result.empty_spaces) {
            chariot_moves.Append(Move{space, empty_space});
        }
        if (search_result.first_occupied_space.size() &&
            parent_.utils_.GetColor(search_result.first_occupied_space[0]) ==
                opponent_of(color)) {
            chariot_moves.Append(
                Move{space, search_result.first_occupied_space[0]});
        }
    }
    return chariot_moves;
}

MoveCollection PieceMoves::HorseMoves(PieceColor color, BoardSpace space) {
    MoveCollection horse_moves;
    horse_moves.moves.reserve(8);

    for (auto direction : kHorsePaths) {
        auto first_step = space + direction.first;
        if (first_step.IsOnBoard() && (not parent_.utils_.IsOccupied(first_step))) {
            for (auto direction : direction.second) {
                auto second_step = first_step + direction;
                if (parent_.utils_.ExistsAndPassesColorTest(second_step, color)) {
                    horse_moves.Append(Move{space, second_step});
                }
            }
        }
    }
    return horse_moves;
}

MoveCollection PieceMoves::ElephantMoves(PieceColor color, BoardSpace space) {
    MoveCollection elephant_moves;
    elephant_moves.moves.reserve(4);
    for (auto direction : kAllDiagonalDirections) {
        auto first_step = space + direction;
        if (first_step.IsOnBoard() && (not parent_.utils_.IsOccupied(first_step)) &&
            (first_step.IsInHomelandOf(color))) {
            auto second_step = first_step + direction;
            if (parent_.utils_.ExistsAndPassesColorTest(second_step, color)) {
                elephant_moves.Append(Move{space, second_step});
            }
        }
    }
    return elephant_moves;
}

MoveCollection PieceMoves::AdvisorMoves(PieceColor color, BoardSpace space) {
    MoveCollection advisor_moves;
    advisor_moves.moves.reserve(4);
    for (auto direction : kAllDiagonalDirections) {
        auto destination = space + direction;
        if (destination.IsInCastleOf(color) &&
            (parent_.utils_.GetColor(destination) != color)) {
            advisor_moves.Append(Move{space, destination});
        }
    }
    return advisor_moves;
}

MoveCollection PieceMoves::FlyingGeneralMove(PieceColor color,
                                               BoardSpace space) {
    MoveCollection flying_move;
    flying_move.moves.reserve(1);

    auto has_flying_move = true;
    auto opponent_color = opponent_of(color);
    auto other_gen_position = parent_.utils_.GetGeneralPosition(opponent_color);
    if (space.file != other_gen_position.file) {
        has_flying_move = false;
    } else {
        auto search_start = min(space.rank, other_gen_position.rank) + 1;
        auto search_end = max(space.rank, other_gen_position.rank) - 1;
        for (auto rank = search_start; rank <= search_end; rank++) {
            if (parent_.utils_.IsOccupied(BoardSpace{rank, space.file})) {
                has_flying_move = false;
            }
        }
    }

    if (has_flying_move) {
        flying_move.Append(Move{space, other_gen_position});
    }

    return flying_move;
}

MoveCollection PieceMoves::StandardGeneralMoves(PieceColor color,
                                                  BoardSpace space) {
    MoveCollection standard_general_moves;
    standard_general_moves.moves.reserve(4);
    for (auto direction : kAllOrthogonalDirections) {
        auto destination = space + direction;
        if (destination.IsInCastleOf(color) &&
            (parent_.utils_.GetColor(destination) != color)) {
            standard_general_moves.Append(Move{space, destination});
        }
    }
    return standard_general_moves;
}

MoveCollection PieceMoves::GeneralMoves(PieceColor color, BoardSpace space) {
    auto flying_move = FlyingGeneralMove(color, space);
    auto standard_moves = StandardGeneralMoves(color, space);
    MoveCollection general_moves;
    general_moves.moves.reserve(flying_move.moves.size() + standard_moves.moves.size());
    general_moves.moves.insert(general_moves.moves.end(), flying_move.moves.begin(),
                         flying_move.moves.end());
    general_moves.moves.insert(general_moves.moves.end(), standard_moves.moves.begin(),
                         standard_moves.moves.end());

    return general_moves;
}


