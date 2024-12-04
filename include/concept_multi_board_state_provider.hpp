#pragma once

#include <array>
#include <concept_single_board_state_provider.hpp>
#include <concepts>

template <typename T>
concept MultiBoardStateProviderConcept = requires(T t)
{
  // Assuming T defines a type alias 'KeyType' and a static constexpr 'NumConfKeys'
  typename T::KeyType;

  // Use SingleBoardStateProviderConcept on T::KeyType
  requires SingleBoardStateProviderConcept<typename T::KeyType>;


  // Check that T::NumConfKeys is a size_t
  requires std::is_same_v<decltype(T::NumConfKeys), size_t>;
  
  { t.primary_board_state() } -> std::same_as<typename T::KeyType>;
  { t.confirmation_board_states() } -> std::same_as<std::array<typename T::KeyType, T::NumConfKeys>>;
};