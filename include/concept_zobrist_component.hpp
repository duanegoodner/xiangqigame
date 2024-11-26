#pragma once

#include <array>
#include <concepts>

template <typename T, typename KeyType, size_t NumConfKeys>
concept ZobristComponentConcept = requires(T t

) {
  { t.primary_board_state() } -> std::same_as<KeyType>;
  { t.confirmation_board_states() } -> std::same_as<std::array<KeyType, NumConfKeys>>;
};