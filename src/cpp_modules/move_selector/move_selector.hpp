#ifndef _MOVE_SELECTOR_
#define _MOVE_SELECTOR_

#include <board_components.hpp>
#include <move_selector_helpers.hpp>
#include <utility_functs.hpp>
#include <vector>

// CRTP interface that MoveSelector requires of (Minimax)Evaluator
template <typename ImplementedEvaluator, typename ConcreteGameBoard>
class EvaluatorInterface {
public:
  RatedMove RateMove(
      Move move,
      ConcreteGameBoard &game_board,
      PieceColor cur_player
  ) {
    return static_cast<ImplementedEvaluator *>(this)
        ->ImplementRateMove(move, game_board, cur_player);
  }

  BestMoves EvaluateNonWinLeaf(
      ConcreteGameBoard &game_board,
      PieceColor cur_player,
      PieceColor initiating_player
  ) {
    return static_cast<EvaluatorInterface *>(this)
        ->ImplementEvaluateNonWinLeaf(
            game_board,
            cur_player,
            initiating_player
        );
  }
};

// CRTP interface that MoveSelector requires of GameBoard
template <typename ConcreteGameBoard>
class MoveTracker {
public:
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

template <typename ImplementedMoveSelector, typename ConcreteGameBoard>
class MoveSelectorInterface {
public:
  Move SelectMove(ConcreteGameBoard &game_board, PieceColor cur_player) {
    return static_cast<ImplementedMoveSelector *>(this)->ImplementSelectMove(
        game_board,
        cur_player
    );
  }
};

template <typename ConcreteGameBoard>
class RandomMoveSelector : public MoveSelectorInterface<
                               RandomMoveSelector<ConcreteGameBoard>,
                               ConcreteGameBoard> {
public:
  Move ImplementSelectMove(
      ConcreteGameBoard &game_board,
      PieceColor cur_player
  );
};

template <typename MinimaxEvaluator, typename ConcreteGameBoard>
class MinimaxMoveSelectorInterface
    : public MoveSelectorInterface<
          MinimaxMoveSelectorInterface<MinimaxEvaluator, ConcreteGameBoard>,
          ConcreteGameBoard> {
public:
  // These data members need to be public for CRTP to work
  MinimaxEvaluator evaluator_;
  int search_depth_;
  int node_counter_;

  MinimaxMoveSelectorInterface();
  MinimaxMoveSelectorInterface(int search_depth);
  MinimaxMoveSelectorInterface(MinimaxEvaluator evaluator, int search_depth);
  void ResetNodeCounter();
  vector<RatedMove> GenerateRankedMoveList(
      ConcreteGameBoard &game_board,
      PieceColor cur_player,
      MoveCollection &cur_player_moves
  );

  BestMoves MinimaxRec(
      ConcreteGameBoard &game_board,
      int search_depth,
      int alpha,
      int beta,
      PieceColor cur_player,
      PieceColor initiating_player
  );

  Move ImplementSelectMove(
      ConcreteGameBoard &game_board,
      PieceColor cur_player
  );
};

#include <move_selector.tpp>

#endif
