#pragma once

#include <array>
#include <concepts/single_board_state_provider.hpp>
#include <concepts>

template <typename T>
concept MultiBoardStateProviderConcept = requires(T t) {
  typename T::KeyType;

  requires std::unsigned_integral<typename T::KeyType>;
  requires(sizeof(typename T::KeyType) * 8) % 32 == 0;

  { t.primary_board_state() } -> std::same_as<typename T::KeyType>;
  requires std::same_as<std::remove_reference_t<decltype(T::NumConfKeys)>, const size_t>;
  {
    t.confirmation_board_states()
  } -> std::same_as<std::array<typename T::KeyType, T::NumConfKeys>>;
};