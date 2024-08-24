// Filename: move_selector.tpp
// Author: Duane Goodner
// Created: 2022-12-10
// Last Modified: 2024-08-16

// Description:
// Implements methods of Concrete MoveSelector classes. These are still
// template methods because MoveSelector can have any valid implementation of
// GameBoard and Evaluator classes.

#ifndef _MOVE_SELECTOR_TEMPLATES_
#define _MOVE_SELECTOR_TEMPLATES_

#include <iostream>
#include <move_selector_details.hpp>

template <typename ConcreteSpaceInfoProvider>
Move RandomMoveSelector<ConcreteSpaceInfoProvider>::ImplementSelectMove(
    ConcreteSpaceInfoProvider &game_board,
    PieceColor cur_player
) {
  auto cur_moves = game_board.CalcFinalMovesOf(cur_player);
  auto selected_move_index =
      utility_functs::random((size_t)0, cur_moves.moves.size() - 1);
  return cur_moves.moves[selected_move_index];
}

template <typename MinimaxEvaluator>
MinimaxMoveSelector<MinimaxEvaluator>::MinimaxMoveSelector()
    // : evaluator_{MinimaxEvaluator(DEFAULT_GAME_POINTS_ARRAY)}
    : evaluator_{MinimaxEvaluator()}
    , search_depth_{3}
    , node_counter_{0} {}

template <typename MinimaxEvaluator>
MinimaxMoveSelector<MinimaxEvaluator>::MinimaxMoveSelector(int search_depth)
    // : evaluator_{MinimaxEvaluator(DEFAULT_GAME_POINTS_ARRAY)}
    : evaluator_{MinimaxEvaluator()}
    , search_depth_{search_depth}
    , node_counter_{0} {}

template <typename MinimaxEvaluator>
MinimaxMoveSelector<MinimaxEvaluator>::MinimaxMoveSelector(
    MinimaxEvaluator evaluator,
    int search_depth
)
    : evaluator_{evaluator}
    , search_depth_{search_depth}
    , node_counter_{0} {}

template <typename MinimaxEvaluator>
void MinimaxMoveSelector<MinimaxEvaluator>::ResetNodeCounter() {
  node_counter_ = 0;
}

template <typename MinimaxEvaluator>
vector<RatedMove>
MinimaxMoveSelector<MinimaxEvaluator>::GenerateRankedMoveList(
    MinimaxEvaluator::game_board_type &game_board,
    PieceColor cur_player,
    MoveCollection &cur_player_moves
) {
  vector<RatedMove> rated_moves;
  for (auto cur_move : cur_player_moves.moves) {
    auto cur_rated_move =
        evaluator_.RateMove(cur_move, game_board, cur_player);
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

template <typename MinimaxEvaluator>
BestMoves MinimaxMoveSelector<MinimaxEvaluator>::MinimaxRec(
    MinimaxEvaluator::game_board_type &game_board,
    int search_depth,
    int alpha,
    int beta,
    PieceColor cur_player,
    PieceColor initiating_player,
    bool use_transposition_table
) {
  node_counter_ += 1;
  MinimaxResultType result_type{};

  auto state_score_search_result =
        game_board.SearchTranspositionTable(initiating_player, search_depth);
  if (state_score_search_result.found && use_transposition_table) {
      return state_score_search_result.table_entry.best_moves;
    }

  auto cur_moves = game_board.CalcFinalMovesOf(cur_player);
  if (cur_moves.moves.size() == 0) {
    result_type = MinimaxResultType::kEndGameLeaf;
    auto result = evaluate_win_leaf(cur_player, initiating_player);
    game_board.RecordCurrentStateScore(
        initiating_player,
        search_depth,
        result_type,
        result
    );
    return result;
  }
  if (search_depth == 0) {
    result_type = MinimaxResultType::kStandardLeaf;
    auto result = evaluator_.EvaluateNonWinLeaf(
        game_board,
        cur_player,
        initiating_player
    );
    game_board.RecordCurrentStateScore(
        initiating_player,
        search_depth,
        result_type,
        result
    );
    return result;
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
                          initiating_player,
                          use_transposition_table
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
        result_type = MinimaxResultType::kAlphaPrune;
        break;
      }
    }
    auto result = BestMoves{max_eval, best_moves};
    game_board.RecordCurrentStateScore(
        initiating_player,
        search_depth,
        result_type,
        result
    );
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
                          initiating_player,
                          use_transposition_table
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
        result_type = MinimaxResultType::kBetaPrune;
        break;
      }
    }
    result_type = MinimaxResultType::kFullyEvaluatedNode;
    auto result = BestMoves{min_eval, best_moves};
    game_board.RecordCurrentStateScore(
        initiating_player,
        search_depth,
        result_type,
        result
    );
    return result;
  }
}

template <typename MinimaxEvaluator>
Move MinimaxMoveSelector<MinimaxEvaluator>::RunMinimax(
    MinimaxEvaluator::game_board_type &game_board,
    int search_depth,
    int alpha,
    int beta,
    PieceColor cur_player,
    PieceColor initiating_player,
    bool use_transposition_table
) {
  ResetNodeCounter();
  auto minimax_result = MinimaxRec(
      game_board,
      search_depth_,
      numeric_limits<int>::min(),
      numeric_limits<int>::max(),
      cur_player,
      cur_player,
      use_transposition_table
  );
  auto selected_move_index = utility_functs::random(
      (size_t)0,
      minimax_result.best_moves.moves.size() - 1
  );

  return minimax_result.best_moves.moves[selected_move_index];
}

template <typename MinimaxEvaluator>
Move MinimaxMoveSelector<MinimaxEvaluator>::ImplementSelectMove(
    MinimaxEvaluator::game_board_type &game_board,
    PieceColor cur_player
) {

  auto first_selected_move = RunMinimax(
      game_board,
      search_depth_,
      numeric_limits<int>::min(),
      numeric_limits<int>::max(),
      cur_player,
      cur_player
  );

  auto allowed_moves = game_board.CalcFinalMovesOf(cur_player);
  if (allowed_moves.ContainsMove(first_selected_move)) {
    return first_selected_move;
  } else {
    auto second_selected_move = RunMinimax(
        game_board,
        search_depth_,
        numeric_limits<int>::min(),
        numeric_limits<int>::max(),
        cur_player,
        cur_player,
        false
    );

    auto second_allowed_moves = game_board.CalcFinalMovesOf(cur_player);
    if (not second_allowed_moves.ContainsMove(second_selected_move)) {
      std::cout << "Returned bad move\nstart = "
                << second_selected_move.start.rank << ", "
                << second_selected_move.start.file
                << "\nend = " << second_selected_move.end.rank << ", "
                << second_selected_move.end.file << std::endl;
      exit(1);
    }

    return second_selected_move;
  }
}

#endif