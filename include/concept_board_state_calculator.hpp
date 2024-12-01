#pragma once

#include <concepts>
#include <board_data_structs.hpp>
#include <memory>
#include <move_data_structs.hpp>


template <typename T>
concept BoardStateCalculatorConcept = requires(
    T t,
    const gameboard::BoardMap_t &board_map,
    const gameboard::ExecutedMove &executed_move,
    uint32_t seed

) {
    {t.FullBoardStateCalc(board_map)} -> std::same_as<void>;
    {t.UpdateBoardState(executed_move)} -> std::same_as<void>;
    {t.Create(seed)} -> std::same_as<std::shared_ptr<T>>;
};
