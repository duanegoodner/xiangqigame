#ifndef _MOVE_SELECTOR_
#define _MOVE_SELECTOR_

#include <board_components.hpp>
// #include <game_board.hpp>
#include <piece_points.hpp>
#include <utility_functs.hpp>
#include <vector>

using namespace piece_points;

struct BestMoves {
  Points_t best_eval;
  MoveCollection best_moves;
};

struct RatedMove {
  Move move;
  Points_t rating;
};

inline BestMoves evaluate_win_leaf(
    PieceColor cur_player,
    PieceColor initiating_player
) {
  auto empty_best_moves = MoveCollection();

  if (cur_player == initiating_player) {
    return BestMoves{numeric_limits<Points_t>::max(), empty_best_moves};
  } else {
    return BestMoves{numeric_limits<Points_t>::min(), empty_best_moves};
  }
}

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
template <typename ConcreteGameBoard> class MoveTracker {
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
  ) {
    auto cur_moves = game_board.CalcFinalMovesOf(cur_player);
    auto selected_move_index =
        utility_functs::random((size_t)0, cur_moves.moves.size() - 1);
    return cur_moves.moves[selected_move_index];
  }
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

  MinimaxMoveSelectorInterface()
      : evaluator_{MinimaxEvaluator(DEFAULT_GAME_POINTS)}
      , search_depth_{3}
      , node_counter_{0} {}

  MinimaxMoveSelectorInterface(int search_depth)
      : evaluator_{MinimaxEvaluator(DEFAULT_GAME_POINTS)}
      , search_depth_{search_depth}
      , node_counter_{0} {}

  MinimaxMoveSelectorInterface(MinimaxEvaluator evaluator, int search_depth)
      : evaluator_{evaluator}
      , search_depth_{search_depth}
      , node_counter_{0} {}

  void ResetNodeCounter() { InternalImplementResetNodeCounter(); };

  vector<RatedMove> GenerateRankedMoveList(
      ConcreteGameBoard &game_board,
      PieceColor cur_player,
      MoveCollection &cur_player_moves
  ) {
    return InternalImplementGenerateRankedMoveList(
        game_board,
        cur_player,
        cur_player_moves
    );
  }

  BestMoves MinimaxRec(
      ConcreteGameBoard &game_board,
      int search_depth,
      int alpha,
      int beta,
      PieceColor cur_player,
      PieceColor initiating_player
  ) {
    return InternalImplementMinimaxRec(
        game_board,
        search_depth,
        alpha,
        beta,
        cur_player,
        initiating_player
    );
  }

  Move ImplementSelectMove(
      ConcreteGameBoard &game_board,
      PieceColor cur_player
  ) {
    return InternalImplementSelectMove(game_board, cur_player);
  }

private:
  void InternalImplementResetNodeCounter() { node_counter_ = 0; };

  vector<RatedMove> InternalImplementGenerateRankedMoveList(
      ConcreteGameBoard &game_board,
      PieceColor cur_player,
      MoveCollection &cur_player_moves
  ) {
    vector<RatedMove> rated_moves;
    for (auto cur_move : cur_player_moves.moves) {
      auto cur_rated_move =
          evaluator_.ImplementRateMove(cur_move, game_board, cur_player);
      rated_moves.emplace_back(cur_rated_move);
    }
    sort(
        rated_moves.begin(),
        rated_moves.end(),
        [](const RatedMove &move_a, const RatedMove &move_b) {
          return move_a.rating > move_b.rating;
        }
    );
    return rated_moves;
  }

  Move InternalImplementSelectMove(
      ConcreteGameBoard &game_board,
      PieceColor cur_player
  ) {
    ResetNodeCounter();
    auto minimax_result = MinimaxRec(
        game_board,
        search_depth_,
        numeric_limits<int>::min(),
        numeric_limits<int>::max(),
        cur_player,
        cur_player
    );
    auto selected_move_index = utility_functs::random(
        (size_t)0,
        minimax_result.best_moves.moves.size() - 1
    );
    return minimax_result.best_moves.moves[selected_move_index];
  }

  BestMoves InternalImplementMinimaxRec(
      ConcreteGameBoard &game_board,
      int search_depth,
      int alpha,
      int beta,
      PieceColor cur_player,
      PieceColor initiating_player
  ) {
    node_counter_ += 1;
    auto cur_moves = game_board.CalcFinalMovesOf(cur_player);
    if (cur_moves.moves.size() == 0) {
      return evaluate_win_leaf(cur_player, initiating_player);
    }
    if (search_depth == 0) {
      return evaluator_.ImplementEvaluateNonWinLeaf(
          game_board,
          cur_player,
          initiating_player
      );
    }
    if (cur_player == initiating_player) {
      auto max_eval = numeric_limits<int>::min();
      MoveCollection best_moves;
      auto ranked_moves =
          GenerateRankedMoveList(game_board, cur_player, cur_moves);
      for (auto rated_move : ranked_moves) {
        auto executed_move = game_board.ExecuteMove(rated_move.move);
        auto cur_eval = MinimaxRec(
                            game_board,
                            search_depth - 1,
                            alpha,
                            beta,
                            opponent_of(initiating_player),
                            initiating_player
        )
                            .best_eval;
        if (cur_eval == max_eval) {
          best_moves.Append(rated_move.move);
        } else if (cur_eval > max_eval) {
          max_eval = cur_eval;
          best_moves.moves.clear();
          best_moves.Append(rated_move.move);
        }
        game_board.UndoMove(executed_move);
        alpha = max(alpha, cur_eval);
        if (beta <= alpha) {
          break;
        }
      }
      return BestMoves{max_eval, best_moves};
    } else {
      auto min_eval = numeric_limits<int>::max();
      MoveCollection best_moves;
      auto ranked_moves =
          GenerateRankedMoveList(game_board, cur_player, cur_moves);
      for (auto rated_move : ranked_moves) {
        auto executed_move = game_board.ExecuteMove(rated_move.move);
        auto cur_eval = MinimaxRec(
                            game_board,
                            search_depth - 1,
                            alpha,
                            beta,
                            initiating_player,
                            initiating_player
        )
                            .best_eval;
        if (cur_eval == min_eval) {
          best_moves.Append(rated_move.move);
        } else if (cur_eval < min_eval) {
          {
            min_eval = cur_eval;
            best_moves.moves.clear();
            best_moves.Append(rated_move.move);
          }
        }
        game_board.UndoMove(executed_move);
        beta = min(beta, cur_eval);
        if (beta <= alpha) {
          break;
        }
      }
      return BestMoves{min_eval, best_moves};
    }
  }
};

#endif
