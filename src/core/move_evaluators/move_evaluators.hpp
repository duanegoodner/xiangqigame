// Filename: move_evaluators.hpp
// Author: Duane Goodner
// Created: 2022-12-17
// Last Modified: 2024-08-16

// Description:
// Defines PiecePointsEvaluator template class and interfaces that
// PiecePointsEvaluator requires GameBoard and GamePoints classes to comply
// with.

#ifndef _MINIMAX_EVALUATOR_
#define _MINIMAX_EVALUATOR_

#include <board_components.hpp>
#include <common.hpp>
#include <evaluator_details.hpp>
#include <utility_functs.hpp>

using namespace board_components;

// CRTP INTERFACE: Evaluator <- SpaceInfoProvider (concrete example =
// GameBoard)
template <typename ConcreteSpaceInfoProvider>
class SpaceInfoProvider {
public:
  vector<BoardSpace> GetAllSpacesOccupiedBy(PieceColor color) {
    return static_cast<ConcreteSpaceInfoProvider *>(this)
        ->ImplementGetAllSpacesOccupiedBy(color);
  }

  PieceColor GetColor(BoardSpace space) {
    return static_cast<ConcreteSpaceInfoProvider *>(this)->ImplementGetColor(
        space
    );
  }

  PieceType GetType(BoardSpace space) {
    return static_cast<ConcreteSpaceInfoProvider *>(this)->ImplementGetType(
        space
    );
  }

  TranspositionTableSearchResult GetEvalResult(
      PieceColor color,
      int search_depth
  ) {
    return static_cast<ConcreteSpaceInfoProvider *>(this)
        ->ImplementGetEvalResult(color, search_depth);
  }

  void RecordEvalResult(
      PieceColor color,
      int search_depth,
      MinimaxResultType result_type,
      BestMoves &best_moves
  ) {
    return static_cast<ConcreteSpaceInfoProvider *>(this)
        ->ImplementRecordEvalResult(
            color,
            search_depth,
            result_type,
            best_moves
        );
  }

  MoveCollection CalcFinalMovesOf(PieceColor color) {
    return static_cast<ConcreteSpaceInfoProvider *>(this)
        ->ImplementCalcFinalMovesOf(color);
  };

  ExecutedMove ExecuteMove(Move move) {
    return static_cast<ConcreteSpaceInfoProvider *>(this)
        ->ImplementExecuteMove(move);
  }

  void UndoMove(ExecutedMove executed_move) {
    static_cast<ConcreteSpaceInfoProvider *>(this)->ImplementUndoMove(
        executed_move
    );
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

// CRTP Interface: AIPlayer <- MoveEvaluatorInterface
// Currently not using since AI Player is currently in Python side of app.
// If/when implement AI Player in C++, will move this interface definition to
// C++ Player header file.
template <typename ConcreteMoveEvaluator>
class MoveEvaluatorInterface {
public:
  Move SelectMove() {
    return static_cast<ConcreteMoveEvaluator *>(this)->ImplementSelectMove(
        // cur_player
    );
  }
};

// CLASS TEMPLATE: MinimaxMoveEvaluator
// IMPLEMENTS INTERFACE:
//    MoveEvaluatorInterface
// USES:
//    ConcreteSpaceInfoProvider (e.g. GameBoard) that implements
//    SpaceInfoProvider.
//    ConcretePieceValueProvider (e.g. PiecePoints) that
//    implements PieceValueProvider
// Uses minimax algorithm with alpha-beta pruning to select a move for
// evaluating_player_.
template <
    typename ConcreteSpaceInfoProvider,
    typename ConcretePieceValueProvider>
class MinimaxMoveEvaluator
    : public MoveEvaluatorInterface<MinimaxMoveEvaluator<
          ConcreteSpaceInfoProvider,
          ConcretePieceValueProvider>> {

public:
  MinimaxMoveEvaluator(
      PieceColor evaluating_player,
      int starting_search_depth,
      ConcreteSpaceInfoProvider &game_board,
      ConcretePieceValueProvider game_position_points
  );

  MinimaxMoveEvaluator(
      PieceColor evaluating_player,
      int starting_search_depth,
      ConcreteSpaceInfoProvider &game_board
  );

  Move ImplementSelectMove();
  Points_t GetPlayerTotal(PieceColor color);

private:
  PieceColor evaluating_player_;
  ConcretePieceValueProvider game_position_points_;
  ConcreteSpaceInfoProvider &game_board_;
  BestMoves EvaluateNonWinLeaf(PieceColor cur_player);
  BestMoves EvaluateEndOfGameLeaf(PieceColor cur_player);
  RatedMove RateMove(Move move, PieceColor cur_player);
  Points_t GetValueOfPieceAtPosition(
      PieceColor color,
      PieceType piece_type,
      BoardSpace space
  );
  int starting_search_depth_;
  int node_counter_;
  void ResetNodeCounter() { node_counter_ = 0; }
  std::vector<RatedMove> GenerateRankedMoveList(
      PieceColor cur_player,
      MoveCollection &cur_player_moves
  );
  BestMoves MinimaxRec(
      int cur_search_depth,
      int alpha,
      int beta,
      PieceColor cur_player,
      bool use_transposition_table = true
  );
  Move RunMinimax(bool use_transposition_table = true);
};

// CLASS TEMPLATE: RandomMoveEvaluator
// IMPLEMENTS INTERFACE:
//    MoveEvaluatorInterface
// USES:
//    ConcreteSpaceInfoProvider (e.g. GameBoard) that implements
//    SpaceInfoProvider.
// Randomly chooses one of the legal moves available to evaluating_player_.
template <typename ConcreteSpaceInfoProvider>
class RandomMoveEvaluator
    : public MoveEvaluatorInterface<
          RandomMoveEvaluator<ConcreteSpaceInfoProvider>> {
public:
  RandomMoveEvaluator(
      PieceColor evaluating_player,
      ConcreteSpaceInfoProvider &game_board
  )
      : evaluating_player_{evaluating_player}
      , game_board_{game_board} {};
  Move ImplementSelectMove() {
    auto allowed_moves = game_board_.CalcFinalMovesOf(evaluating_player_);
    auto selected_move_index =
        utility_functs::random((size_t)0, allowed_moves.moves.size() - 1);
    return allowed_moves.moves[selected_move_index];
  };

private:
  PieceColor evaluating_player_;
  ConcreteSpaceInfoProvider &game_board_;
};

#include <move_evaluators.tpp>

#endif /* MINIMAX_EVALUATOR */
