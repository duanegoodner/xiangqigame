#pragma once

#include <concept_board_state_calculator.hpp>
#include <concept_space_info_provider.hpp>
#include <concepts>
#include <memory>

template <typename T>
concept BoardStateCalculatorRegistryConcept =
    requires(
        T t,
        std::shared_ptr<typename T::CalculatorTypeRed> calculator_red,
        std::shared_ptr<typename T::CalculatorTypeBlack> calculator_black,
        gameboard::PieceColor color,
        const gameboard::BoardMapInt_t &starting_board
    ) {
      { t.AttachCalculator(calculator_red, color) } -> std::same_as<void>;
      { t.AttachCalculator(calculator_black, color)} -> std::same_as<void>;
      {t.Create(starting_board)} -> std::same_as<std::shared_ptr<T>>;
    };
