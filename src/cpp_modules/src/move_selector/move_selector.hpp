// Filename: move_selector.hpp
// Author: Duane Goodner
// Created: 2022-12-10
// Last Modified: 2024-08-16

// Description:
// Defines MoveSelector Interface and concrete template classes, as well as
// interfaces that MoveSelector expects compliance with. AIPlayer that de-facto
// defines MoveSelector is currently on Python side of app.

#ifndef _MOVE_SELECTOR_
#define _MOVE_SELECTOR_

#include <board_components.hpp>
#include <common.hpp>
#include <move_selector_details.hpp>
#include <utility_functs.hpp>
#include <vector>

// CRTP Interface: MoveSelector <- (Minimax)Evaluator
// Note: Some implementations of MoveSelector (e.g. RandomMoveSelector) do not
// have an Evaluator.
template <typename ConcreteEvaluator, typename ConcreteSpaceInfoProvider>
class Evaluator {
public:
  typedef ConcreteSpaceInfoProvider game_board_type;

  RatedMove RateMove(
      Move move,
      ConcreteSpaceInfoProvider &game_board,
      PieceColor cur_player
  ) {
    return static_cast<ConcreteEvaluator *>(this)
        ->ImplementRateMove(move, game_board, cur_player);
  }

  BestMoves EvaluateNonWinLeaf(
      ConcreteSpaceInfoProvider &game_board,
      PieceColor cur_player,
      PieceColor initiating_player
  ) {
    return static_cast<ConcreteEvaluator *>(this)->ImplementEvaluateNonWinLeaf(
        game_board,
        cur_player,
        initiating_player
    );
  }
};

enum MinimaxResultType : int {
  kUnknown = 0,
  kFullyEvaluatedNode = 1,
  kStandardLeaf = 2,
  kEndGameLeaf = 3,
  kAlphaPrune = 4,
  kBetaPrune = 5
};

struct TranspositionTableEntry {
  zkey_t state;
  int search_depth;
  MinimaxResultType result_type;
  BestMoves best_moves;

  Points_t Score() { return best_moves.best_eval; }
};

struct TranspositionTableSearchResult {
  TranspositionTableEntry table_entry;
  bool found;
};

// CRTP Interface: MoveSelector <- GameBoard
template <typename ConcreteSpaceInfoProvider, typename ConcreteBoardStateSummarizer>
class MoveTracker {
public:

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

// CRTP Interface: AIPlayer <- MoveSelector
// Currently not using since AI Player is currently in Python side of app.
// If/when implement AI Player in C++, will move this interface definition to
// C++ Player header file.
template <typename ConcreteMoveSelector, typename ConcreteSpaceInfoProvider>
class MoveSelector {
public:
  typedef ConcreteSpaceInfoProvider game_board_type;

  Move SelectMove(ConcreteSpaceInfoProvider &game_board, PieceColor cur_player) {
    return static_cast<ConcreteMoveSelector *>(this)->ImplementSelectMove(
        game_board,
        cur_player
    );
  }
};

// Implementation: MoveSelector that selects a random (legal) move
template <typename ConcreteSpaceInfoProvider>
class RandomMoveSelector : public MoveSelector<
                               RandomMoveSelector<ConcreteSpaceInfoProvider>,
                               ConcreteSpaceInfoProvider> {
public:
  Move ImplementSelectMove(
      ConcreteSpaceInfoProvider &game_board,
      PieceColor cur_player
  );
};

// Implementation: MoveSelector that uses Minimax to select move.
template <typename ConcreteEvaluator>
class MinimaxMoveSelector : public MoveSelector<
                                MinimaxMoveSelector<ConcreteEvaluator>,
                                typename ConcreteEvaluator::game_board_type> {
public:
  // These data members need to be public for CRTP to work
  ConcreteEvaluator evaluator_;
  int search_depth_;
  int node_counter_;

  MinimaxMoveSelector();
  MinimaxMoveSelector(int search_depth);
  MinimaxMoveSelector(ConcreteEvaluator evaluator, int search_depth);
  void ResetNodeCounter();
  vector<RatedMove> GenerateRankedMoveList(
      typename ConcreteEvaluator::game_board_type &game_board,
      PieceColor cur_player,
      MoveCollection &cur_player_moves
  );
  Move ImplementSelectMove(
      typename ConcreteEvaluator::game_board_type &game_board,
      PieceColor cur_player
  );

private:
  BestMoves MinimaxRec(typename ConcreteEvaluator::game_board_type &game_board,
                       int search_depth,
                       int alpha,
                       int beta,
                       PieceColor cur_player,
                       PieceColor initiating_player,
                       bool use_transposition_table = true);
  
  Move RunMinimax(typename ConcreteEvaluator::game_board_type &game_board,
                       int search_depth,
                       int alpha,
                       int beta,
                       PieceColor cur_player,
                       PieceColor initiating_player,
                       bool use_transposition_table = true);
};



#include <move_selector.tpp>

#endif
