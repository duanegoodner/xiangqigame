#ifndef _COMMON_
#define _COMMON_

#include <array>
#include <cstddef>
#include <string>
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
const unordered_map<string, PieceType> kPieceTypeStringToEnum = {
    {"null", PieceType::kNnn},
    {"general", PieceType::kGen},
    {"advisor", PieceType::kAdv},
    {"elephant", PieceType::kEle},
    {"chariot", PieceType::kCha},
    {"horse", PieceType::kHor},
    {"cannon", PieceType::kCan},
    {"soldier", PieceType::kSol}
};

enum PieceColor : int { kRed = -1, kNul = 0, kBlk = 1 };
const int kNumPieceColorVals = 3;
const unordered_map<string, PieceColor> kPieceColorStringToEnum = {
  {"red", PieceColor::kRed},
  {"null", PieceColor::kNul},
  {"black", PieceColor::kBlk}
};

// converts red/black: -1/1 of PieceColor enum to 0/1 used in some arrays
inline size_t get_zcolor_index(PieceColor color) {
  return (size_t)(color + (int)(color < 0));
}
// converts red/black: 0/1 to -1/1
inline PieceColor get_piece_color(size_t zcolor_index) {
  int piece_color_val = (int)(zcolor_index - (size_t)(zcolor_index == 0));
  return static_cast<PieceColor>(piece_color_val);
}

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

// internal GamePoints array and sub-array types
typedef array<array<int, kNumFiles>, kNumRanks> PiecePositionPoints_t;
typedef array<PiecePositionPoints_t, kNumPieceTypeVals> TeamPointsArray_t;
typedef array<TeamPointsArray_t, 2> GamePointsArray_t;

// maps with string keys; to decompose GamePointsArray_t into easy-reading
// format
typedef unordered_map<string, PiecePositionPoints_t> TeamPointsSMap_t;
typedef unordered_map<string, TeamPointsSMap_t> GamePointsSMap_t;

// maps w/ string keys, for external storage of black-base / red-offset spec
typedef unordered_map<string, Points_t> base_points_map_t;
typedef unordered_map<string, PiecePositionPoints_t> position_points_map_t;

// maps w/ enum keys; internal storage of black-base / red-offset spec
typedef unordered_map<PieceType, Points_t> TeamBasePoints_t;
typedef unordered_map<PieceType, PiecePositionPoints_t>
    TeamPositionPointsMap_t;

// old style of internal points data struct
typedef unordered_map<PieceColor, TeamPositionPointsMap_t> GamePointsMap_t;

#endif