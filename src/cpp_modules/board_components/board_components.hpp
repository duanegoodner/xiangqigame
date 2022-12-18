
#ifndef _BOARD_COMPONENTS_
#define _BOARD_COMPONENTS_

#include <array>
#include <cassert>
#include <random>
#include <vector>
#include "game_piece.hpp"

using namespace std;
using namespace game_piece;

namespace board_components {

typedef int Piece_t;
typedef int BoardIdx_t;

// const int kNumPieceTypeVals = 8;

const BoardIdx_t kNumRanks = 10;
const BoardIdx_t kNumFiles = 9;

const BoardIdx_t kRedRiverEdge = 5;
const BoardIdx_t kBlackRiverEdge = 4;

struct CastleEdges {
    BoardIdx_t min_rank;
    BoardIdx_t max_rank;
    BoardIdx_t min_file;
    BoardIdx_t max_file;
};

const CastleEdges kRedCastleEdges = {7, 9, 3, 5};
const CastleEdges kBlackCastleEdges = {0, 2, 3, 5};

typedef array<array<Piece_t, kNumFiles>, kNumRanks> BoardMap_t;

struct BoardDirection {
    BoardIdx_t rank, file;
};

struct BoardSpace {
    
    BoardIdx_t rank, file;
    
    bool IsOnBoard() {
        return ((0 <= rank) && (rank < kNumRanks) && (0 <= file) && (file < kNumFiles));
    }

    bool IsInHomelandOf(PieceColor color) {
        assert(color != PieceColor::kNul);
        if (color == PieceColor::kRed) {
            return rank >= kRedRiverEdge;
        } else {
            return rank <= kBlackRiverEdge;
        }
    }

    bool IsInCastleOf(PieceColor color) {
        assert(color != PieceColor::kNul);
        auto castle_edges =
            color == PieceColor::kRed ? kRedCastleEdges : kBlackCastleEdges;
        return (castle_edges.min_rank <= rank) && (rank <= castle_edges.max_rank) &&
               (castle_edges.min_file <= file) && (file <= castle_edges.max_file);
    }

    BoardSpace operator+(const BoardDirection direction) {
        return BoardSpace{rank + direction.rank, file + direction.file};
    }

    bool operator==(const BoardSpace other) {
        return (rank == other.rank) && (file == other.file);
    }

    bool operator!=(const BoardSpace other) {
        return (rank != other.rank) || (file != other.file);
    }
};


struct OrthogonalSpaceSearchResult {
    vector<BoardSpace> empty_spaces;
    vector<BoardSpace> first_occupied_space;
};

inline PieceColor opponent_of(PieceColor color) {
    return static_cast<PieceColor>(-1 * color);
}

}  // namespace board_components

#endif  // _SHARED_COMPONENTS_