#pragma once

#include <concept_board_state_calculator.hpp>
#include <concept_board_state_calculator_registry.hpp>
#include <concept_single_board_state_provider.hpp>
#include <concept_space_info_provider.hpp>
#include <concepts>

template <typename T, typename C>
concept SpaceInfoProviderAndCalculatorRegistryConcept =
    SpaceInfoProviderConcept<T> && BoardStateCalculatorRegistryConcept<T, C>;

template <typename T>
concept SingleBoardStateProviderAndBoardStateCalculatorConcept =
    SingleBoardStateProviderConcept<T> && BoardStateCalculatorConcept<T>;
