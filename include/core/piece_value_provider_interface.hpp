#pragma once

#include <board_data_structs.hpp>
#include <common.hpp>

using namespace gameboard;

// CRTP Interface: Evaluator <- GamePoints
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