#pragma once

#include <board_data_structs.hpp>
#include <game_piece.hpp>
#include <move_data_structs.hpp>
#include <concepts>
#include <functional>


template <typename T>
concept SpaceInfoProviderConcept = requires(
    T t,
    gameboard::PieceColor color,
    gameboard::BoardSpace &space,
    gameboard::Move &move,
    gameboard::ExecutedMove &executed_move,
    void (*callback)(const gameboard::ExecutedMove &)
) {
  {
    t.GetAllSpacesOccupiedBy(color)
  } -> std::same_as<std::vector<gameboard::BoardSpace>>;
  {t.GetColor(space)} -> std::same_as<gameboard::PieceColor>;
  {t.GetType(space)} -> std::same_as<gameboard::PieceType>;
  {t.CalcFinalMovesOf(color)} -> std::same_as<gameboard::MoveCollection>;
  {t.ExecuteMove(move)} -> std::same_as<gameboard::ExecutedMove>;
  {t.UndoMove(executed_move)} -> std::same_as<void>;
  // {t.AttachMoveCallback(callback)} -> std::same_as<void>;
  {t.IsDraw()} -> std::same_as<bool>; 
  {t.map()} -> std::same_as<gameboard::BoardMap_t&>;
};
