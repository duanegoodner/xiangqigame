#include <array>
#include <cstdint>
#include <memory>
#include <zobrist_concepts.hpp>

template <typename K>
class ZCalculatorImplementation {
public:
  using KeyType = K;

private:
  KeyType board_state_;

public:
  KeyType board_state() { return board_state_; }
};

template <ZCalculatorConcept C, size_t N>
class ZComponentImplementation {
public:
  using KeyType = C::KeyType;
  static size_t constexpr NumKeys = N;

private:
  std::shared_ptr<C> primary_calculator_;
  std::array<std::shared_ptr<C>, NumKeys> secondary_calculators_;

public:
  KeyType primary_board_state() { return primary_calculator_->board_state(); }
  std::array<KeyType, NumKeys> secondary_board_states() {
    std::array<KeyType, NumKeys> secondary_states;
    for (auto idx = 0; idx < N; ++idx) {
      secondary_states[idx] = secondary_calculators_[idx]->board_state();
    }
    return secondary_states;
  }
};

int main() {
  static_assert(
      ZCalculatorConcept<ZCalculatorImplementation<uint64_t>>,
      "ZCalculatorImplementation must comply with ZCalculator Concept"
  );

  static_assert(
      ZComponentConcept<
          ZComponentImplementation<ZCalculatorImplementation<uint64_t>, 1>>,
      "ZComponentImplementation must comply with ZComponentConcept"
  );

  return 0;
}