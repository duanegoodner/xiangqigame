//! @file random_move_evaluator.tpp
//! Implementation of moveselection::RandomMoveEvaluator methods. 

#pragma once

#include <board_data_structs.hpp>
#include <game_piece.hpp>
#include <utility_functs.hpp>

using namespace gameboard;

namespace moveselection {

template <typename ConcreteSpaceInfoProvider>
RandomMoveEvaluator<ConcreteSpaceInfoProvider>::RandomMoveEvaluator(
    PieceColor evaluating_player,
    ConcreteSpaceInfoProvider &game_board
)
    : evaluating_player_{evaluating_player}
    , game_board_{game_board} {};

template <typename ConcreteSpaceInfoProvider>
Move RandomMoveEvaluator<ConcreteSpaceInfoProvider>::ImplementSelectMove() {
  auto allowed_moves = game_board_.CalcFinalMovesOf(evaluating_player_);
  auto selected_move_index =
      utility_functs::random((size_t)0, allowed_moves.moves.size() - 1);
  return allowed_moves.moves[selected_move_index];
};

}
