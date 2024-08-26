// Filename: game_board.tpp
// Author: Duane Goodner
// Created: 2022-12-27
// Last Modified: 2024-08-16

// Description:
// Implements PiecePointsEvaluator template class.

#ifndef _MINIMAX_EVALUATOR_TEMPLATE_
#define _MINIMAX_EVALUATOR_TEMPLATE_

#include <board_components.hpp>
#include <common.hpp>
#include <evaluator_details.hpp>
#include <iostream>
#include <limits>
using namespace board_components;

template <
    typename ConcreteSpaceInfoProvider,
    typename ConcretePieceValueProvider>
MinimaxMoveEvaluator<ConcreteSpaceInfoProvider, ConcretePieceValueProvider>::
    MinimaxMoveEvaluator(ConcretePieceValueProvider game_position_points)
    : game_position_points_{game_position_points} {}

template <
    typename ConcreteSpaceInfoProvider,
    typename ConcretePieceValueProvider>
MinimaxMoveEvaluator<ConcreteSpaceInfoProvider, ConcretePieceValueProvider>::
    MinimaxMoveEvaluator()
    : game_position_points_{ConcretePieceValueProvider()} {}

template <
    typename ConcreteSpaceInfoProvider,
    typename ConcretePieceValueProvider>
Move MinimaxMoveEvaluator<
    ConcreteSpaceInfoProvider,
    ConcretePieceValueProvider>::
    ImplementSelectMove(
        ConcreteSpaceInfoProvider &game_board,
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

template <
    typename ConcreteSpaceInfoProvider,
    typename ConcretePieceValueProvider>
std::vector<RatedMove> MinimaxMoveEvaluator<ConcreteSpaceInfoProvider, ConcretePieceValueProvider>::
    GenerateRandkedMoveList(
      ConcreteSpaceInfoProvider &game_board,
      PieceColor cur_player,
      MoveCollection &cur_player_moves
  ) {
  vector<RatedMove> rated_moves;
  for (auto cur_move : cur_player_moves.moves) {
    auto cur_rated_move =
        RateMove(cur_move, game_board, cur_player);
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

template <
    typename ConcreteSpaceInfoProvider,
    typename ConcretePieceValueProvider>
BestMoves
MinimaxMoveEvaluator<ConcreteSpaceInfoProvider, ConcretePieceValueProvider>::
    EvaluateNonWinLeaf(
        ConcreteSpaceInfoProvider &game_board,
        PieceColor cur_player,
        PieceColor initiating_player
    ) {
  auto cur_player_points = GetPlayerTotal(cur_player, game_board);
  auto opponent_points = GetPlayerTotal(opponent_of(cur_player), game_board);

  auto empty_move_collection = MoveCollection();

  if (cur_player == initiating_player) {
    return BestMoves{
        (cur_player_points - opponent_points),
        empty_move_collection
    };
  } else {
    return BestMoves{
        (opponent_points - cur_player_points),
        empty_move_collection
    };
  }
}

template <
    typename ConcreteSpaceInfoProvider,
    typename ConcretePieceValueProvider>
RatedMove
MinimaxMoveEvaluator<ConcreteSpaceInfoProvider, ConcretePieceValueProvider>::
    RateMove(
        Move move,
        ConcreteSpaceInfoProvider &game_board,
        PieceColor cur_player
    ) {
  auto piece_type = game_board.GetType(move.start);

  auto end_points = game_position_points_.GetValueOfPieceAtPosition(
      cur_player,
      piece_type,
      move.end
  );
  auto start_points = game_position_points_.GetValueOfPieceAtPosition(
      cur_player,
      piece_type,
      move.start
  );
  auto position_value_delta = end_points - start_points;

  Points_t capture_val;

  if (game_board.GetColor(move.end) == opponent_of(cur_player)) {
    auto captured_piece_type = game_board.GetType(move.end);
    capture_val = GetValueOfPieceAtPosition(
        opponent_of(cur_player),
        captured_piece_type,
        move.end
    );
  } else {
    capture_val = 0;
  }

  return RatedMove{move, (position_value_delta + capture_val)};
}

template <
    typename ConcreteSpaceInfoProvider,
    typename ConcretePieceValueProvider>
Points_t
MinimaxMoveEvaluator<ConcreteSpaceInfoProvider, ConcretePieceValueProvider>::
    GetValueOfPieceAtPosition(
        PieceColor color,
        PieceType piece_type,
        BoardSpace space
    ) {
  return game_position_points_
      .GetValueOfPieceAtPosition(color, piece_type, space);
}

template <
    typename ConcreteSpaceInfoProvider,
    typename ConcretePieceValueProvider>
Points_t
MinimaxMoveEvaluator<ConcreteSpaceInfoProvider, ConcretePieceValueProvider>::
    GetPlayerTotal(PieceColor color, ConcreteSpaceInfoProvider &game_board) {
  Points_t pre_attack_total = 0;
  for (auto space : game_board.GetAllSpacesOccupiedBy(color)) {
    auto piece_type = game_board.GetType(space);
    pre_attack_total += game_position_points_.GetValueOfPieceAtPosition(
        color,
        piece_type,
        space
    );
  }
  return pre_attack_total;
}

template <
    typename ConcreteSpaceInfoProvider,
    typename ConcretePieceValueProvider>
BestMoves
MinimaxMoveEvaluator<ConcreteSpaceInfoProvider, ConcretePieceValueProvider>::
    MinimaxRec(
        ConcreteSpaceInfoProvider &game_board,
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
    auto result =
        EvaluateNonWinLeaf(game_board, cur_player, initiating_player);
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

template <
    typename ConcreteSpaceInfoProvider,
    typename ConcretePieceValueProvider>
Move
MinimaxMoveEvaluator<ConcreteSpaceInfoProvider, ConcretePieceValueProvider>::
    RunMinimax(
        ConcreteSpaceInfoProvider &game_board,
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

#endif /* MINIMAX_EVALUATOR */
