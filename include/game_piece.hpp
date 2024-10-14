//! @file game_piece.hpp
//! Defines GamePiece and supporting constants and free functions.

#pragma once

#include <cmath>
#include <string>
#include <unordered_map>

using namespace std;

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

//! A Xiangqi game piece described by its gameboard::PieceType and its gameboard::PieceColor. 
struct GamePiece {
  PieceType piece_type;
  PieceColor piece_color;

  GamePiece() {
    piece_type = PieceType::kNnn;
    piece_color = PieceColor::kNul;
  }

  GamePiece(int int_piece) {
    piece_type = static_cast<PieceType>(abs(int_piece));
    piece_color = (int_piece == 0) ? PieceColor::kNul
                                   : static_cast<PieceColor>(copysign(1, int_piece));
  }

  GamePiece(PieceType type, PieceColor color) {
    piece_type = type;
    piece_color = color;
  }

  bool operator==(const GamePiece &other) const {
    return (piece_type == other.piece_type) && (piece_color == other.piece_color);
  }
};

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

} // namespace gameboard