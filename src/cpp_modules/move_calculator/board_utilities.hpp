#include <vector>

#ifndef _BOARD_UTILITIES_
#define _BOARD_UTILITIES_

#include "shared_components.hpp"
using namespace board_components;

typedef array<BoardSpace, 9> Castle_t;
inline Castle_t calc_castle_spaces(const CastleEdges edges) {
    std::array<BoardSpace, 9> spaces;

    for (auto rank = edges.min_rank; rank <= edges.max_rank; rank++) {
        for (auto file = edges.min_file; file <= edges.max_file; file++) {
            spaces[3 * (rank - edges.min_rank) + file - edges.min_file].rank =
                rank;
            spaces[3 * (rank - edges.min_rank) + file - edges.min_file].file =
                file;
        }
    }
    return spaces;
}

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

class BoardUtilities {
   public:
    BoardUtilities(const BoardMap_t& board_map);
    bool IsOccupied(BoardSpace space);
    PieceColor GetColor(BoardSpace space);
    PieceType GetType(BoardSpace space);
    bool ExistsAndPassesColorTest(BoardSpace space,
                                  PieceColor moving_piece_color);
    BoardDirection FwdDirection(PieceColor color);
    BoardDirection RevDirection(PieceColor color);
    BoardSpace GetGeneralPosition(PieceColor color);
    OrthogonalSpaceSearchResult SearchSpaces(
        BoardSpace start, BoardDirection direction);
    vector<BoardSpace> GetAllSpacesOccupiedBy(PieceColor color);
    bool IsSpaceAnyDestinationOfMoves(BoardSpace space, MoveCollection &moves); 

   private:
    const BoardMap_t& board_map_;
    Castle_t red_castle_;
    Castle_t black_castle_;
};

#endif