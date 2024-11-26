#pragma once

#include <concepts>
#include <move_data_structs.hpp>

template <typename T>
concept MoveEvaluatorConcept = requires(T t, gameboard::MoveCollection &allowed_moves) {
  { t.SelectMove(allowed_moves) } -> std::same_as<gameboard::Move>;
};