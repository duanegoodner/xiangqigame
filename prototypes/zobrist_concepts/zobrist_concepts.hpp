#include <array>
#include <concepts>
#include <cstddef>
#include <type_traits>

template <typename T>
concept ZCalculatorConcept = requires(T t) {
  typename T::KeyType;
  requires std::unsigned_integral<typename T::KeyType>;
  requires(sizeof(typename T::KeyType) * 8) % 32 == 0;
  { t.board_state() } -> std::same_as<typename T::KeyType>;
};

template <typename T>
concept ZComponentConcept = requires(T t) {
  typename T::KeyType;
  requires std::unsigned_integral<typename T::KeyType>;
  requires(sizeof(typename T::KeyType) * 8) % 32 == 0;

  { t.primary_board_state() } -> std::same_as<typename T::KeyType>;
  requires std::same_as<std::remove_reference_t<decltype(T::NumKeys)>, const size_t>;

  {
    t.secondary_board_states()
  } -> std::same_as<std::array<typename T::KeyType, T::NumKeys>>;
};
