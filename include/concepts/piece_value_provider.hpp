#pragma once

#include <gameboard/board_data_structs.hpp>
#include <concepts>
#include <gameboard/game_piece.hpp>
#include <utilities/integer_types.hpp>
#include <string>

template <typename T>
concept PieceValueProviderConcept = requires(
    T t,
    gameboard::PieceColor color,
    gameboard::PieceType piece_type,
    gameboard::BoardSpace &space,
    std::string json_file
) {
  { t.GetValueOfPieceAtPosition(color, piece_type, space) } -> std::same_as<Points_t>;
  { t.Create(json_file) } -> std::same_as<std::shared_ptr<T>>;
};