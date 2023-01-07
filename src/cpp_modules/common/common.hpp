#ifndef _COMMON_
#define _COMMON_

#include <array>
#include <unordered_map>

using namespace std;

// ///////
// Pieces
// //////
enum PieceType : int {
  kNnn = 0,
  kGen = 1,
  kAdv = 2,
  kEle = 3,
  kHor = 4,
  kCha = 5,
  kCan = 6,
  kSol = 7
};
const int kNumPieceTypeVals = 8;
enum PieceColor : int { kRed = -1, kNul = 0, kBlk = 1 };


// //////
// Board 
// //////
typedef int BoardIdx_t;
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
constexpr CastleEdges kRedCastleEdges = {7, 9, 3, 5};
constexpr CastleEdges kBlackCastleEdges = {0, 2, 3, 5};
struct BoardDirection {
  BoardIdx_t rank, file;
};

// //////////////
// HashCalculator 
// /////////////
typedef unsigned long long zkey_t;
typedef array<array<zkey_t, kNumFiles>, kNumRanks> piece_zarray_t;
typedef array<piece_zarray_t, kNumPieceTypeVals> team_zarray_t;
typedef array<team_zarray_t, 2> game_zarray_t;

// //////////////
// Piece Points
// /////////////
typedef int Points_t;
typedef unordered_map<PieceType, Points_t> TeamBasePoints_t;
typedef array<array<int, kNumFiles>, kNumRanks> PiecePositionPoints_t;
typedef unordered_map<PieceType, PiecePositionPoints_t> TeamPositionPoints_t;
typedef unordered_map<PieceColor, TeamPositionPoints_t> GamePositionPoints_t;


#endif