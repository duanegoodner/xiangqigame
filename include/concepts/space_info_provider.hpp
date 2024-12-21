#pragma once

#include <gameboard/board_data_structs.hpp>
#include <gameboard/game_piece.hpp>
#include <gameboard/move_data_structs.hpp>
#include <concepts>


template <typename T>
concept SpaceInfoProviderConcept = requires(
    T t,
    gameboard::PieceColor color,
    gameboard::BoardSpace &space,
    gameboard::Move &move,
    gameboard::ExecutedMove &executed_move,
    const gameboard::BoardMapInt_t &starting_board
    ) {
  {
    t.GetAllSpacesOccupiedBy(color)
  } -> std::same_as<std::vector<gameboard::BoardSpace>>;
  {t.GetColor(space)} -> std::same_as<gameboard::PieceColor>;
  {t.GetType(space)} -> std::same_as<gameboard::PieceType>;
  {t.CalcFinalMovesOf(color)} -> std::same_as<gameboard::MoveCollection>;
  {t.ExecuteMove(move)} -> std::same_as<gameboard::ExecutedMove>;
  {t.UndoMove(executed_move)} -> std::same_as<void>;
  {t.IsDraw()} -> std::same_as<bool>; 
  {t.map()} -> std::same_as<const gameboard::BoardMap_t&>;
  {t.Create(starting_board)} -> std::same_as<std::shared_ptr<T>>;
};
