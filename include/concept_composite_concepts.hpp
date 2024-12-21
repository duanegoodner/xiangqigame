#pragma once

#include <concept_board_state_calculator.hpp>
#include <concepts/board_state_calculator_registry.hpp>
#include <concepts/single_board_state_provider.hpp>
#include <concept_space_info_provider.hpp>
#include <concepts>

template <typename T>
concept SpaceInfoProviderAndCalculatorRegistryConcept =
    SpaceInfoProviderConcept<T> && BoardStateCalculatorRegistryConcept<T>;

template <typename T>
concept SingleBoardStateProviderAndBoardStateCalculatorConcept =
    SingleBoardStateProviderConcept<T> && BoardStateCalculatorConcept<T>;
