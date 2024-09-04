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
#include <random>
#include <string>
#include <unordered_map>
#include <vector>

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

// struct ZobristKey64 {
//   uint64_t value;

//   bool operator==(const ZobristKey64 & other) const {
//     return value == other.value;
//   }

//   ZobristKey64 operator^(const ZobristKey64 &other) const {
//     return ZobristKey64{value ^ other.value};
//   }

//   static const ZobristKey64 pseudorandom_from_existing_64bit_gen(
//     std::mt19937_64 &gen_64
//   ) {
//     return ZobristKey64{gen_64()};
//   }

//   static const ZobristKey64 pseudorandom_from_new_64bit_gen() {
//     std::random_device rd;
//     uint32_t seed = rd();
//     std::mt19937_64 gen_64{seed};
//     return pseudorandom_from_existing_64bit_gen(gen_64);
//   }
// };

// struct ZobristKey128 {
//   uint64_t value_high;
//   uint64_t value_low;

//   bool operator==(const ZobristKey128 &other) const {
//     return value_high == other.value_high && value_low == other.value_low;
//   }

//   ZobristKey128 operator^(const ZobristKey128 &other) const {
//     return ZobristKey128{value_high ^ other.value_high, value_low ^ other.value_low};
//   }

//   static const ZobristKey128 pseudorandom_from_existing_64bit_gen(std::mt19937_64 &gen_64) {
//     return ZobristKey128{gen_64(), gen_64()};
//   }

//   static const ZobristKey128 pseudo_random_from_new_64bit_gen() {
//     std::random_device rd{};
//     uint32_t seed = rd();
//     std::mt19937_64 gen_64{seed};
//     return pseudorandom_from_existing_64bit_gen(gen_64);
//   }
// };

typedef __uint128_t zkey_t;
typedef array<array<zkey_t, kNumFiles>, kNumRanks> piece_zarray_t;
typedef array<piece_zarray_t, kNumPieceTypeVals> team_zarray_t;
typedef array<team_zarray_t, 2> game_zarray_t;

// //////////////
// Piece Points
// /////////////
typedef int Points_t;

// 2-D array of points values for single piece
typedef array<array<int, kNumFiles>, kNumRanks> PiecePointsArray_t;

// 3-D array of all piece points values for a team
typedef array<PiecePointsArray_t, kNumPieceTypeVals> TeamPointsArray_t;

// 4-D array of both teams points values for a game
typedef array<TeamPointsArray_t, 2> GamePointsArray_t;

inline bool operator==(
    const PiecePointsArray_t &a,
    const PiecePointsArray_t &b
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

// types for base-points offset (BPO) spec
typedef unordered_map<PieceType, Points_t> TeamBasePoints_t;
typedef array<Points_t, kNumPieceTypeVals> base_points_array_t;
typedef unordered_map<string, Points_t> BasePointsSMap_t;
typedef unordered_map<PieceType, PiecePointsArray_t> TeamPointsEMap_t;
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
typedef unordered_map<PieceColor, TeamPointsEMap_t> GamePointsEMap_t;
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