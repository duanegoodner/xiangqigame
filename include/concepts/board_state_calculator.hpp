#pragma once

#include <gameboard/board_data_structs.hpp>
#include <concepts>
#include <memory>
#include <move_data_structs.hpp>

template <typename T>
concept BoardStateCalculatorConcept = requires(
    T t,
    const gameboard::BoardMap_t &board_map,
    const gameboard::ExecutedMove &executed_move,
    uint32_t seed

) {
  { t.FullBoardStateCalc(board_map) } -> std::same_as<void>;
  { t.UpdateBoardState(executed_move) } -> std::same_as<void>;
  { t.Create(seed) } -> std::same_as<std::shared_ptr<T>>;
};

class NullBoardStateCalculator {

public:
  static std::shared_ptr<NullBoardStateCalculator> Create(uint32_t seed = 0) {
    return std::shared_ptr<NullBoardStateCalculator>(new NullBoardStateCalculator());
  }
  void FullBoardStateCalc(const gameboard::BoardMap_t &board_map) {}
  void UpdateBoardState(const gameboard::ExecutedMove &executed_move) {}

private:
  NullBoardStateCalculator() = default;
};
