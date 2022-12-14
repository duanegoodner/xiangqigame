#include "board_utilities.hpp"

#include <cmath>

BoardUtilities::BoardUtilities(const BoardMap_t& board_map)
    : board_map_{board_map} {
    red_castle_ = calc_castle_spaces(kRedCastleEdges);
    black_castle_ = calc_castle_spaces(kBlackCastleEdges);
}

// bool BoardUtilities::IsOccupied(BoardSpace space) {
//     return board_map_[space.rank][space.file] != 0;
// }

// PieceColor BoardUtilities::GetColor(BoardSpace space) {
//     auto piece = board_map_[space.rank][space.file];
//     return (piece == 0) ? PieceColor::kNul
//                         : static_cast<PieceColor>(copysign(1, piece));
// }

// PieceType BoardUtilities::GetType(BoardSpace space) {
//     return static_cast<PieceType>(abs(board_map_[space.rank][space.file]));
// }

// bool BoardUtilities::ExistsAndPassesColorTest(
//     BoardSpace space, PieceColor moving_piece_color) {
//     return space.IsOnBoard() &&
//            GetColor(space) !=
//                static_cast<PieceColor>(moving_piece_color);
// }

// BoardDirection BoardUtilities::FwdDirection(PieceColor color) {
//     return BoardDirection{static_cast<int>(color), 0};
// }

// BoardDirection BoardUtilities::RevDirection(PieceColor color) {
//     return BoardDirection{-1 * static_cast<int>(color), 0};
// }

// BoardSpace BoardUtilities::GetGeneralPosition(PieceColor color) {
//     auto castle = (color == PieceColor::kRed) ? red_castle_ : black_castle_;

//     auto color_val = static_cast<int>(color);
//     BoardSpace found_space;

//     for (BoardSpace board_space : castle) {
//         auto piece_val = board_map_[board_space.rank][board_space.file];
//         if (piece_val == color_val) {
//             found_space = board_space;
//         }
//     }
//     return found_space;
// }

OrthogonalSpaceSearchResult BoardUtilities::SearchSpaces(
    BoardSpace start, BoardDirection direction) {
    vector<BoardSpace> empty_spaces;
    empty_spaces.reserve(9);
    vector<BoardSpace> first_occupied_space;
    first_occupied_space.reserve(1);

    auto next_step = start + direction;

    while (next_step.IsOnBoard() && (not IsOccupied(next_step))) {
        empty_spaces.emplace_back(next_step);
        next_step = next_step + direction;
    }
    if (next_step.IsOnBoard()) {
        first_occupied_space.emplace_back(next_step);
    }
    return OrthogonalSpaceSearchResult{empty_spaces, first_occupied_space};
}

// vector<BoardSpace> BoardUtilities::GetAllSpacesOccupiedBy(PieceColor color) {
//     vector<BoardSpace> occupied_spaces;
//     occupied_spaces.reserve(16);
//     for (auto rank = 0; rank < kNumRanks; rank++) {
//         for (auto file = 0; file < kNumFiles; file++) {
//             if (GetColor(BoardSpace{rank, file}) == color) {
//                 occupied_spaces.emplace_back(BoardSpace{rank, file});
//             }
//         }
//     }
//     return occupied_spaces;
// }

// bool BoardUtilities::IsSpaceAnyDestinationOfMoves(BoardSpace space, MoveCollection &moves) {
//         for (auto move : moves.moves) {
//             if (move.end == space) {
//                 return true;
//             }
//         }
//         return false;
//     }

