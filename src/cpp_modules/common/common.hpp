#ifndef _COMMON_
#define _COMMON_

#include <array>
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
    {"soldier", PieceType::kSol}};

enum PieceColor : int { kRed = -1, kNul = 0, kBlk = 1 };
const int kNumPieceColorVals = 3;
const unordered_map<string, PieceColor> kPieceColorStringToEnum = {
    {"red", PieceColor::kRed},
    {"null", PieceColor::kNul},
    {"black", PieceColor::kBlk}};

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

// GamePointsArray_t and its sub-array types
typedef array<array<int, kNumFiles>, kNumRanks> PiecePositionPoints_t;
typedef array<PiecePositionPoints_t, kNumPieceTypeVals> TeamPointsArray_t;
typedef array<TeamPointsArray_t, 2> GamePointsArray_t;

inline bool operator==(
    const PiecePositionPoints_t& a,
    const PiecePositionPoints_t& b
) {
  bool are_equal = true;

  for (auto rank = 0; rank < kNumRanks; rank++) {
    for (auto file = 0; file < kNumFiles; file++) {
      if (a[rank][file] != b[rank][file]) {
        are_equal = false;
        return are_equal;
      }
    }
  }
  return are_equal;
}

// types for bpo spec
typedef unordered_map<PieceType, Points_t> TeamBasePoints_t;
typedef array<Points_t, kNumPieceTypeVals> base_points_array_t;
typedef unordered_map<string, Points_t> base_points_map_t;

typedef unordered_map<PieceType, PiecePositionPoints_t> TeamPointsEMap_t;
typedef unordered_map<string, PiecePositionPoints_t> TeamPointsSMap_t;

inline bool operator==(TeamPointsSMap_t& a, TeamPointsSMap_t& b) {
  bool are_equal = true;

  for (auto piece : a) {
    if (a[piece.first] != b[piece.first]) {
      are_equal = false;
      return are_equal;
    }
  }
  return are_equal;
}

typedef unordered_map<PieceColor, TeamPointsEMap_t> GamePointsEMap_t;
typedef unordered_map<string, TeamPointsSMap_t> GamePointsSMap_t;

inline bool operator==(GamePointsSMap_t& a, GamePointsSMap_t& b) {
  bool are_equal = true;
  for (auto color : a) {
    if (a[color.first] != b[color.first]) {
      are_equal = false;
      return are_equal;
    }
  }
  return are_equal;
}
// array version

// json piece points

// Piece Points all arrays

#endif