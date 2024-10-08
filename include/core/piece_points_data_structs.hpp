//! @file piece_points_data_structs.hpp
//! Typedefs for collections of piece points

#pragma once

#include <board_data_structs.hpp>
#include <game_piece.hpp>
#include <string>
#include <unordered_map>

using namespace std;

namespace piece_points {

typedef int Points_t;
inline size_t size_of_points_type() { return sizeof(Points_t); }
inline bool is_signed_points_type() { return numeric_limits<Points_t>::is_signed; }

//! 2-D array of points values for single piece at each position on a
//! gameboard::GameBoard.board_map_.
typedef array<array<int, gameboard::kNumFiles>, gameboard::kNumRanks> PiecePointsArray_t;

inline bool operator==(const PiecePointsArray_t &a, const PiecePointsArray_t &b) {
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

// 3-D array of all piece points values for a team
typedef array<PiecePointsArray_t, gamepiece::kNumPieceTypeVals> TeamPointsArray_t;
typedef unordered_map<gamepiece::PieceType, PiecePointsArray_t> TeamPointsEMap_t;
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

// 4-D array of both teams points values for a game
typedef array<TeamPointsArray_t, 2> GamePointsArray_t;
typedef unordered_map<gamepiece::PieceColor, TeamPointsEMap_t> GamePointsEMap_t;
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

// types for base-points offset (BPO) spec
typedef unordered_map<gamepiece::PieceType, Points_t> TeamBasePoints_t;
typedef unordered_map<string, Points_t> BasePointsSMap_t;
} // namespace piece_points
