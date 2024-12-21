#pragma once

#include <concepts/board_state_calculator.hpp>
#include <concepts/space_info_provider.hpp>
#include <concepts>
#include <functional>
#include <memory>
#include <move_data_structs.hpp>

template <typename T>
concept BoardStateCalculatorRegistryConcept = requires(
    T t,
    // std::shared_ptr<typename T::CalculatorTypeRed> calculator_red,
    // std::shared_ptr<typename T::CalculatorTypeBlack> calculator_black,
    gameboard::PieceColor color,
    const gameboard::BoardMapInt_t &starting_board,
    const function<void(const gameboard::ExecutedMove &)> &callback

) {
  // { t.AttachCalculator(calculator_red, color) } -> std::same_as<void>;
  // { t.AttachCalculator(calculator_black, color)} -> std::same_as<void>;
  { t.AttachMoveCallback(callback) } -> std::same_as<void>;
  { t.Create(starting_board) } -> std::same_as<std::shared_ptr<T>>;
};
