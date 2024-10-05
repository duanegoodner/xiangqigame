// Filename: common.hpp
// Author: Duane Goodner
// Created: 2022-11-15
// Last Modified: 2024-08-16

// Description:
// Contains typedefs, enums, structs and constants for game piece definitions,
// and points specs. Also contains typedefs and simple operators for points
// calcs and zobrish hashmaps with minimax algo.

#ifndef _COMMON_
#define _COMMON_

#include <array>
#include <limits>
#include <random>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

// ///////
// Pieces
// //////

namespace gameboard {

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


const unordered_map<string, PieceType> kPieceTypeStringToEnum = [] {
    unordered_map<string, PieceType> temp;
    temp.insert(make_pair("null", PieceType::kNnn));
    temp.insert(make_pair("general", PieceType::kGen));
    temp.insert(make_pair("advisor", PieceType::kAdv));
    temp.insert(make_pair("elephant", PieceType::kEle));
    temp.insert(make_pair("chariot", PieceType::kCha));
    temp.insert(make_pair("horse", PieceType::kHor));
    temp.insert(make_pair("cannon", PieceType::kCan));
    temp.insert(make_pair("soldier", PieceType::kSol));
    return temp;
}();

enum PieceColor : int { kRed = -1, kNul = 0, kBlk = 1 };
const int kNumPieceColorVals = 3;
// const unordered_map<string, PieceColor> kPieceColorStringToEnum = {
//     {"red", PieceColor::kRed},
//     {"null", PieceColor::kNul},
//     {"black", PieceColor::kBlk}
// };

// Use lambda function because hpp2plantuml can't parse {{
const unordered_map<string, PieceColor> kPieceColorStringToEnum = [] {
  unordered_map<string, PieceColor> temp;
  temp.insert(make_pair("red", PieceColor::kRed));
  temp.insert(make_pair("null", PieceColor::kNul));
  temp.insert(make_pair("black", PieceColor::kBlk));
  return temp;
}();



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

}

// //////////////
// Piece Points
// /////////////
typedef int Points_t;

inline size_t size_of_points_type() {
  return sizeof(Points_t);
}

inline bool is_signed_points_type() {
  return numeric_limits<Points_t>::is_signed;
}

// 2-D array of points values for single piece
typedef array<array<int, gameboard::kNumFiles>, gameboard::kNumRanks> PiecePointsArray_t;

// 3-D array of all piece points values for a team
typedef array<PiecePointsArray_t, gameboard::kNumPieceTypeVals> TeamPointsArray_t;

// 4-D array of both teams points values for a game
typedef array<TeamPointsArray_t, 2> GamePointsArray_t;

inline bool operator==(
    const PiecePointsArray_t &a,
    const PiecePointsArray_t &b
) {
  bool are_equal = true;

  for (auto rank = 0; rank < gameboard::kNumRanks; rank++) {
    for (auto file = 0; file < gameboard::kNumFiles; file++) {
      if (a[rank][file] != b[rank][file]) {
        are_equal = false;
        return are_equal;
      }
    }
  }
  return are_equal;
}

// types for base-points offset (BPO) spec
typedef unordered_map<gameboard::PieceType, Points_t> TeamBasePoints_t;
typedef array<Points_t, gameboard::kNumPieceTypeVals> base_points_array_t;
typedef unordered_map<string, Points_t> BasePointsSMap_t;
typedef unordered_map<gameboard::PieceType, PiecePointsArray_t> TeamPointsEMap_t;
typedef unordered_map<string, PiecePointsArray_t> TeamPointsSMap_t;

inline bool operator==(TeamPointsSMap_t &a, TeamPointsSMap_t &b) {
  bool are_equal = true;

  for (auto piece : a) {
    if (a[piece.first] != b[piece.first]) {
      are_equal = false;
      return are_equal;
    }
  }
  return are_equal;
}

typedef pair<BasePointsSMap_t, TeamPointsSMap_t> BasePositionSMapPair_t;
typedef unordered_map<string, BasePositionSMapPair_t> BPOSpecSMap_t;
typedef unordered_map<gameboard::PieceColor, TeamPointsEMap_t> GamePointsEMap_t;
typedef unordered_map<string, TeamPointsSMap_t> GamePointsSMap_t;

inline bool operator==(GamePointsSMap_t &a, GamePointsSMap_t &b) {
  bool are_equal = true;
  for (auto color : a) {
    if (a[color.first] != b[color.first]) {
      are_equal = false;
      return are_equal;
    }
  }
  return are_equal;
}

#endif