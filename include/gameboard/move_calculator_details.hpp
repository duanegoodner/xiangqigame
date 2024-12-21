//! @file gameboard/move_calculator_details.hpp
//! Typedefs and data struct used by gameboard::MoveCalculator to select appropriate method
//! from gameboard::PieceMoves.

#pragma once

#include <array>
#include <gameboard/board_data_structs.hpp>
#include <gameboard/piece_moves.hpp>

using namespace std;
using namespace gameboard;
using namespace gameboard;

namespace gameboard {
typedef void (PieceMoves::*
                  MethodPtr_t)(const BoardMap_t &, PieceColor, const BoardSpace &, MoveCollection &);

typedef array<MethodPtr_t, kNumPieceTypeVals> PieceDispatchArray_t;

inline constexpr PieceDispatchArray_t build_piece_dispatch_array() {
  PieceDispatchArray_t dispatch_array{};
  dispatch_array[PieceType::kNnn] = {};
  dispatch_array[PieceType::kSol] = &PieceMoves::SoldierMoves;
  dispatch_array[PieceType::kCan] = &PieceMoves::CannonMoves;
  dispatch_array[PieceType::kCha] = &PieceMoves::ChariotMoves;
  dispatch_array[PieceType::kHor] = &PieceMoves::HorseMoves;
  dispatch_array[PieceType::kEle] = &PieceMoves::ElephantMoves;
  dispatch_array[PieceType::kAdv] = &PieceMoves::AdvisorMoves;
  dispatch_array[PieceType::kGen] = &PieceMoves::GeneralMoves;

  return dispatch_array;
}
} // namespace gameboard
