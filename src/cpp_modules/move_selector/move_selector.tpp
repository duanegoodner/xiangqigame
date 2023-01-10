#ifndef _MOVE_SELECTOR_TEMPLATES_
#define _MOVE_SELECTOR_TEMPLATES_

#include <move_selector_helpers.hpp>
#include <piece_points.hpp>

using namespace piece_points;

template <typename ConcreteGameBoard>
Move RandomMoveSelector<ConcreteGameBoard>::ImplementSelectMove(
    ConcreteGameBoard &game_board,
    PieceColor cur_player
) {
  auto cur_moves = game_board.CalcFinalMovesOf(cur_player);
  auto selected_move_index =
      utility_functs::random((size_t)0, cur_moves.moves.size() - 1);
  return cur_moves.moves[selected_move_index];
}

template <typename MinimaxEvaluator, typename ConcreteGameBoard>
MinimaxMoveSelectorInterface<MinimaxEvaluator, ConcreteGameBoard>::
    MinimaxMoveSelectorInterface()
    : evaluator_{MinimaxEvaluator(DEFAULT_GAME_POINTS)}
    , search_depth_{3}
    , node_counter_{0} {}

template <typename MinimaxEvaluator, typename ConcreteGameBoard>
MinimaxMoveSelectorInterface<MinimaxEvaluator, ConcreteGameBoard>::
    MinimaxMoveSelectorInterface(int search_depth)
    : evaluator_{MinimaxEvaluator(DEFAULT_GAME_POINTS)}
    , search_depth_{search_depth}
    , node_counter_{0} {}

template <typename MinimaxEvaluator, typename ConcreteGameBoard>
MinimaxMoveSelectorInterface<MinimaxEvaluator, ConcreteGameBoard>::
    MinimaxMoveSelectorInterface(MinimaxEvaluator evaluator, int search_depth)
    : evaluator_{evaluator}
    , search_depth_{search_depth}
    , node_counter_{0} {}

template <typename MinimaxEvaluator, typename ConcreteGameBoard>
void MinimaxMoveSelectorInterface<MinimaxEvaluator, ConcreteGameBoard>::
    ResetNodeCounter() {
  node_counter_ = 0;
}

template <typename MinimaxEvaluator, typename ConcreteGameBoard>
vector<RatedMove>
MinimaxMoveSelectorInterface<MinimaxEvaluator, ConcreteGameBoard>::
    GenerateRankedMoveList(
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

template <typename MinimaxEvaluator, typename ConcreteGameBoard>
BestMoves
MinimaxMoveSelectorInterface<MinimaxEvaluator, ConcreteGameBoard>::MinimaxRec(
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

template <typename MinimaxEvaluator, typename ConcreteGameBoard>
Move MinimaxMoveSelectorInterface<MinimaxEvaluator, ConcreteGameBoard>::
    ImplementSelectMove(ConcreteGameBoard &game_board, PieceColor cur_player) {
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

#endif