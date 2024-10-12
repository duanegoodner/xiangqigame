//! @file piece_value_provider_interface.hpp
//! Definition of PieceValueProvider CRTP interface.

#pragma once

#include <board_data_structs.hpp>

using namespace gameboard;

//! CRTP interface with method to retrieve points value for a specific
//! gameboard::GamePiece at a specific gameboard::BoardSpace (concrete example:
//! piecepoints::PiecePositionPoints).
template <typename ConcretePieceValueProvider>
class PieceValueProvider {
public:
  Points_t GetValueOfPieceAtPosition(
      PieceColor color,
      PieceType piece_type,
      BoardSpace space
  ) {
    return static_cast<ConcretePieceValueProvider *>(this)
        ->ImplementGetValueOfPieceAtPosition(color, piece_type, space);
  }
};