// TODO: Revisit which methods are worth leaving in hpp file
// for inlining performance benefit and which are best in cpp

#include <cmath>
#include <vector>

#ifndef _BOARD_UTILITIES_
#define _BOARD_UTILITIES_

#include "board_components.hpp"

namespace board_utilities
{

    // using namespace board_components::data_types;
    using namespace board_components;

    // typedef array<BoardSpace, 9> Castle_t;
    // inline Castle_t calc_castle_spaces(const CastleEdges edges)
    // {
    //     std::array<BoardSpace, 9> spaces;

    //     for (auto rank = edges.min_rank; rank <= edges.max_rank; rank++)
    //     {
    //         for (auto file = edges.min_file; file <= edges.max_file; file++)
    //         {
    //             spaces[3 * (rank - edges.min_rank) + file - edges.min_file].rank =
    //                 rank;
    //             spaces[3 * (rank - edges.min_rank) + file - edges.min_file].file =
    //                 file;
    //         }
    //     }
    //     return spaces;
    // }

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

    class BoardUtilities
    {
    public:
        BoardUtilities(const BoardMap_t &board_map)
            : board_map_{board_map}
        {
            red_castle_ = calc_castle_spaces(kRedCastleEdges);
            black_castle_ = calc_castle_spaces(kBlackCastleEdges);
        }
        bool IsOccupied(BoardSpace space)
        {
            return board_map_[space.rank][space.file] != 0;
        };
        PieceColor GetColor(BoardSpace space)
        {
            auto piece = board_map_[space.rank][space.file];
            return (piece == 0) ? PieceColor::kNul
                                : static_cast<PieceColor>(copysign(1, piece));
        };
        PieceType GetType(BoardSpace space)
        {
            return static_cast<PieceType>(abs(board_map_[space.rank][space.file]));
        };
        bool ExistsAndPassesColorTest(BoardSpace space, PieceColor moving_piece_color)
        {
            return space.IsOnBoard() &&
                   GetColor(space) !=
                       static_cast<PieceColor>(moving_piece_color);
        }
        BoardDirection FwdDirection(PieceColor color)
        {
            return BoardDirection{static_cast<int>(color), 0};
        }
        BoardDirection RevDirection(PieceColor color)
        {
            return BoardDirection{-1 * static_cast<int>(color), 0};
        }
        BoardSpace GetGeneralPosition(PieceColor color)
        {
            auto castle = (color == PieceColor::kRed) ? red_castle_ : black_castle_;

            auto color_val = static_cast<int>(color);
            BoardSpace found_space;

            for (BoardSpace board_space : castle)
            {
                auto piece_val = board_map_[board_space.rank][board_space.file];
                if (piece_val == color_val)
                {
                    found_space = board_space;
                }
            }
            return found_space;
        }
        OrthogonalSpaceSearchResult SearchSpaces(
            BoardSpace start, BoardDirection direction)
        {
            vector<BoardSpace> empty_spaces;
            empty_spaces.reserve(9);
            vector<BoardSpace> first_occupied_space;
            first_occupied_space.reserve(1);

            auto next_step = start + direction;

            while (next_step.IsOnBoard() && (not IsOccupied(next_step)))
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
        vector<BoardSpace> GetAllSpacesOccupiedBy(PieceColor color)
        {
            vector<BoardSpace> occupied_spaces;
            occupied_spaces.reserve(16);
            for (auto rank = 0; rank < kNumRanks; rank++)
            {
                for (auto file = 0; file < kNumFiles; file++)
                {
                    if (GetColor(BoardSpace{rank, file}) == color)
                    {
                        occupied_spaces.emplace_back(BoardSpace{rank, file});
                    }
                }
            }
            return occupied_spaces;
        }
        bool IsSpaceAnyDestinationOfMoves(BoardSpace space, MoveCollection &moves)
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

    private:
        const BoardMap_t &board_map_;
        Castle_t red_castle_;
        Castle_t black_castle_;
    };

} // namespace board_utilities

#endif