#pragma once

#include <move_data_structs.hpp>
#include <concepts>


template <typename T>
concept BoardStateTrackerConcept = requires(
    T t,
    const gameboard::ExecutedMove &executed_move
) {
    {t.UpdateBoardState(executed_move)} -> std::same_as<void>;
};


