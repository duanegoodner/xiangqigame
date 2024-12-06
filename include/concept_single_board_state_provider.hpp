#pragma once

#include <concepts>
#include <memory>
#include <type_traits>

template <typename T>
concept SingleBoardStateProviderConcept = requires(T t, uint32_t seed) {
  typename T::KeyType;
  // KeyType must be unsigned integer with bitsize a multiple of 32
  requires std::unsigned_integral<typename T::KeyType>;
  requires(sizeof(typename T::KeyType) * 8) % 32 == 0;

  { t.board_state() } -> std::same_as<typename T::KeyType>;
  { t.Create(seed) } -> std::same_as<std::shared_ptr<T>>;
};