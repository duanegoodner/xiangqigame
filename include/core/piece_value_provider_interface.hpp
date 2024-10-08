//! @file piece_value_provider_interface.hpp
//! Definition of PieceValueProvider CRTP interface.

#pragma once

#include <board_data_structs.hpp>

using namespace gameboard;

//! CRTP interface with method to retrieve points value for a specific
//! gamepiece::GamePiece at a specific gameboard::BoardSpace (concrete example:
//! piece_points::PiecePositionPoints).
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