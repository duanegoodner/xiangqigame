// Filename: minimax_evaluator.hpp
// Author: Duane Goodner
// Created: 2022-12-17
// Last Modified: 2024-08-16

// Description:
// Defines PiecePointsEvaluator template class and interfaces that
// PiecePointsEvaluator requires GameBoard and GamePoints classes to comply
// with.

#ifndef _MINIMAX_EVALUATOR_
#define _MINIMAX_EVALUATOR_

// #include "piece_points.hpp"
#include <board_components.hpp>
#include <common.hpp>
#include <move_selector.hpp>

using namespace board_components;

// CRTP INTERFACE: Evaluator <- SpaceInfoProvider (concrete example = GameBoard)
template <typename ConcreteSpaceInfoProvider>
class SpaceInfoProvider {
public:
  vector<BoardSpace> GetAllSpacesOccupiedBy(PieceColor color) {
    return static_cast<ConcreteSpaceInfoProvider *>(this)
        ->ImplementGetAllSpacesOccupiedBy(color);
  }

  PieceColor GetColor(BoardSpace space) {
    return static_cast<ConcreteSpaceInfoProvider *>(this)->ImplementGetColor(space);
  }

  PieceType GetType(BoardSpace space) {
    return static_cast<ConcreteSpaceInfoProvider *>(this)->ImplementGetType(space);
  }
};

// CRTP Interface: Evaluator <- GamePoints
template <typename ConcretePieceValueProvider>
class PieceValueProvider {
public:
  Points_t GetValueOfPieceAtPosition(
      PieceColor color,
      PieceType piece_type,
      BoardSpace space
  ) {
    return static_cast<ConcretePieceValueProvider *>(this)
        ->ImplementGetValueOfPieceAtPosition(color, piece_type, space);
  }
};

/*
Template for a class that has ConcreteSpaceInfoProvider, is constructed using
ConcretePieceValueProvider, and implements the Evaluator interface specified by
MoveSelector
 */

template <typename ConcreteSpaceInfoProvider, typename ConcretePieceValueProvider>
class PiecePointsEvaluator
    : public Evaluator<
          PiecePointsEvaluator<ConcreteSpaceInfoProvider, ConcretePieceValueProvider>,
          ConcreteSpaceInfoProvider> {
public:
  PiecePointsEvaluator(ConcretePieceValueProvider game_position_points_);
  PiecePointsEvaluator();

  BestMoves ImplementEvaluateNonWinLeaf(
      ConcreteSpaceInfoProvider &game_board,
      PieceColor cur_player,
      PieceColor initiating_player
  );

  RatedMove ImplementRateMove(
      Move move,
      ConcreteSpaceInfoProvider &game_board,
      PieceColor cur_player
  );

  Points_t GetValueOfPieceAtPosition(
      PieceColor color,
      PieceType piece_type,
      BoardSpace space
  );

  Points_t GetPlayerTotal(PieceColor color, ConcreteSpaceInfoProvider &game_board);

private:
  ConcretePieceValueProvider game_position_points_;
};

#include <minimax_evaluator.tpp>

#endif /* MINIMAX_EVALUATOR */
