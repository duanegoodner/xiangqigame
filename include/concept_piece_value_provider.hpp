#pragma once

#include <board_data_structs.hpp>
#include <concepts>
#include <game_piece.hpp>
#include <integer_types.hpp>

template <typename T>
concept PieceValueProviderConcept = requires(
    T t,
    gameboard::PieceColor color,
    gameboard::PieceType piece_type,
    gameboard::BoardSpace &space
) {
  { t.GetValueOfPieceAtPosition(color, piece_type, space) } -> std::same_as<Points_t>;
};