#pragma once

#include <concepts>
#include <board_data_structs.hpp>
#include <move_data_structs.hpp>


template <typename T, typename KeyType>
concept BoardStateCalculatorConcept = requires(
    T t,
    const gameboard::BoardMap_t &board_map,
    const gameboard::ExecutedMove &executed_move
) {
    {t.FullBoardStateCalc(board_map)} -> std::same_as<void>;
    {t.UpdateBoardState(executed_move)} -> std::same_as<void>;
};
