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
template <typename ConcreteEvaluator, typename ConcreteGameBoard>
class Evaluator {
public:
  typedef ConcreteGameBoard game_board_type;

  RatedMove RateMove(
      Move move,
      ConcreteGameBoard &game_board,
      PieceColor cur_player
  ) {
    return static_cast<ConcreteEvaluator *>(this)
        ->ImplementRateMove(move, game_board, cur_player);
  }

  BestMoves EvaluateNonWinLeaf(
      ConcreteGameBoard &game_board,
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

struct StateScoreSearchResult {
  zkey_t state;
  bool found;
  BestMoves best_moves;
};

// CRTP Interface: MoveSelector <- GameBoard
template <typename ConcreteGameBoard, typename ConcreteHashCalculator>
class MoveTracker {
public:

  // zkey_t GetState() {
  //   return static_cast<ConcreteGameBoard *>(this)->ImplementGetState();
  // }

  StateScoreSearchResult FindCurrentStateScore(PieceColor color) {
    return static_cast<ConcreteGameBoard *>(this)->ImplementFindCurrentStateScore(color);
  }

  void RecordCurrentStateScore(PieceColor color, BestMoves& best_moves) {
    return static_cast<ConcreteGameBoard *>(this)->ImplementRecordCurrentStateScore(color, best_moves);
  }
  
  MoveCollection CalcFinalMovesOf(PieceColor color) {
    return static_cast<ConcreteGameBoard *>(this)->ImplementCalcFinalMovesOf(
        color
    );
  };

  ExecutedMove ExecuteMove(Move move) {
    return static_cast<ConcreteGameBoard *>(this)->ImplementExecuteMove(move);
  }

  void UndoMove(ExecutedMove executed_move) {
    static_cast<ConcreteGameBoard *>(this)->ImplementUndoMove(executed_move);
  }
};

// CRTP Interface: AIPlayer <- MoveSelector
// Currently not using since AI Player is currently in Python side of app.
// If/when implement AI Player in C++, will move this interface definition to
// C++ Player header file.
template <typename ConcreteMoveSelector, typename ConcreteGameBoard>
class MoveSelector {
public:
  typedef ConcreteGameBoard game_board_type;

  Move SelectMove(ConcreteGameBoard &game_board, PieceColor cur_player) {
    return static_cast<ConcreteMoveSelector *>(this)->ImplementSelectMove(
        game_board,
        cur_player
    );
  }
};

// Implementation: MoveSelector that selects a random (legal) move
template <typename ConcreteGameBoard>
class RandomMoveSelector : public MoveSelector<
                               RandomMoveSelector<ConcreteGameBoard>,
                               ConcreteGameBoard> {
public:
  Move ImplementSelectMove(
      ConcreteGameBoard &game_board,
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

  BestMoves MinimaxRec(
      typename ConcreteEvaluator::game_board_type &game_board,
      int search_depth,
      int alpha,
      int beta,
      PieceColor cur_player,
      PieceColor initiating_player
  );

  Move ImplementSelectMove(
      typename ConcreteEvaluator::game_board_type &game_board,
      PieceColor cur_player
  );
};

#include <move_selector.tpp>

#endif
