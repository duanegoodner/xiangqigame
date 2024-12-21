#pragma once

#include <concepts>
#include <gameboard/board_data_structs.hpp>
#include <evaluator_data_structs.hpp>
#include <integer_types.hpp>
#include <memory>
#include <move_data_structs.hpp>

// template <typename T, typename KeyType>
template <typename T>
concept BoardStateCoordinatorConcept = requires(
    T t,
    const gameboard::BoardMap_t &board_map,
    const gameboard::ExecutedMove &executed_move,
    DepthType search_depth,
    moveselection::MinimaxResultType result_type,
    moveselection::EqualScoreMoves &similar_moves,
    MoveCountType access_index
) {
    typename T::KeyType;
    
    // {t.FullBoardStateCalc(board_map)} -> std::same_as<void>;
    // {t.UpdateBoardState(executed_move)} -> std::same_as<void>;   
    {t.GetState()} -> std::same_as<typename T::KeyType>;
    {t.RecordTrData(search_depth, result_type, similar_moves, access_index)} -> std::same_as<void>;
    {t.GetTrData(search_depth, access_index)} -> std::same_as<moveselection::TranspositionTableSearchResult>;
    {t.GetTrTableSize()} -> std::same_as<size_t>;
    {t.UpdateMoveCounter()} -> std::same_as<void>;
};