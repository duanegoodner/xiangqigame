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

  TranspositionTableSearchResult SearchTranspositionTable(
      PieceColor color,
      int search_depth
  ) {
    return static_cast<ConcreteSpaceInfoProvider *>(this)
        ->ImplementSearchTranspositionTable(color, search_depth);
  }

  void RecordCurrentStateScore(
      PieceColor color,
      int search_depth,
      MinimaxResultType result_type,
      BestMoves &best_moves
  ) {
    return static_cast<ConcreteSpaceInfoProvider *>(this)
        ->ImplementRecordCurrentStateScore(
            color,
            search_depth,
            result_type,
            best_moves
        );
  }

  MoveCollection CalcFinalMovesOf(PieceColor color) {
    return static_cast<ConcreteSpaceInfoProvider *>(this)->ImplementCalcFinalMovesOf(
        color
    );
  };

  ExecutedMove ExecuteMove(Move move) {
    return static_cast<ConcreteSpaceInfoProvider *>(this)->ImplementExecuteMove(move);
  }

  void UndoMove(ExecutedMove executed_move) {
    static_cast<ConcreteSpaceInfoProvider *>(this)->ImplementUndoMove(executed_move);
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
