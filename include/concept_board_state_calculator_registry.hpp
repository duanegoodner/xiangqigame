#pragma once

#include <concept_board_state_calculator.hpp>
#include <concept_space_info_provider.hpp>
#include <concepts>
#include <memory>


template <typename T, typename C>
concept BoardStateCalculatorRegistryConcept =
    BoardStateCalculatorConcept<C> &&
    requires(T t, std::shared_ptr<C> calculator, gameboard::PieceColor color) {
      { t.AttachCalculator(calculator, color) } -> std::same_as<void>;
    };


