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
    MinimaxMoveEvaluator(
        PieceColor evaluating_player,
        int starting_search_depth,
        ConcreteSpaceInfoProvider &game_board,
        ConcretePieceValueProvider game_position_points
    )
    : evaluating_player_{evaluating_player}
    , starting_search_depth_{starting_search_depth}
    , game_board_{game_board}
    , game_position_points_{game_position_points} {}

template <
    typename ConcreteSpaceInfoProvider,
    typename ConcretePieceValueProvider>
MinimaxMoveEvaluator<ConcreteSpaceInfoProvider, ConcretePieceValueProvider>::
    MinimaxMoveEvaluator(
        PieceColor evaluating_player,
        int starting_search_depth,
        ConcreteSpaceInfoProvider &game_board
    )
    : MinimaxMoveEvaluator(
          evaluating_player,
          starting_search_depth,
          game_board,
          ConcretePieceValueProvider()
      ) {}

template <
    typename ConcreteSpaceInfoProvider,
    typename ConcretePieceValueProvider>
Move MinimaxMoveEvaluator<
    ConcreteSpaceInfoProvider,
    ConcretePieceValueProvider>::ImplementSelectMove() {

  auto first_selected_move = RunMinimax();

  auto allowed_moves = game_board_.CalcFinalMovesOf(evaluating_player_);
  if (allowed_moves.ContainsMove(first_selected_move)) {
    return first_selected_move;
  } else {
    auto second_selected_move = RunMinimax(false);

    auto second_allowed_moves =
        game_board_.CalcFinalMovesOf(evaluating_player_);
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
std::vector<RatedMove>
MinimaxMoveEvaluator<ConcreteSpaceInfoProvider, ConcretePieceValueProvider>::
    GenerateRankedMoveList(
        PieceColor cur_player,
        MoveCollection &cur_player_moves
    ) {
  vector<RatedMove> rated_moves;
  for (auto cur_move : cur_player_moves.moves) {
    auto cur_rated_move = RateMove(cur_move, cur_player);
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
BestMoves MinimaxMoveEvaluator<
    ConcreteSpaceInfoProvider,
    ConcretePieceValueProvider>::EvaluateNonWinLeaf(PieceColor cur_player) {
  auto cur_player_points = GetPlayerTotal(cur_player);
  auto opponent_points = GetPlayerTotal(opponent_of(cur_player));

  auto empty_move_collection = MoveCollection();

  if (cur_player == evaluating_player_) {
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
BestMoves MinimaxMoveEvaluator<
    ConcreteSpaceInfoProvider,
    ConcretePieceValueProvider>::EvaluateEndOfGameLeaf(PieceColor cur_player) {
  auto empty_best_moves = MoveCollection();

  if (cur_player == evaluating_player_) {
    return BestMoves{numeric_limits<Points_t>::min(), empty_best_moves};
  } else {
    return BestMoves{numeric_limits<Points_t>::max(), empty_best_moves};
  }
}

template <
    typename ConcreteSpaceInfoProvider,
    typename ConcretePieceValueProvider>
RatedMove MinimaxMoveEvaluator<
    ConcreteSpaceInfoProvider,
    ConcretePieceValueProvider>::RateMove(Move move, PieceColor cur_player) {
  auto piece_type = game_board_.GetType(move.start);

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

  if (game_board_.GetColor(move.end) == opponent_of(cur_player)) {
    auto captured_piece_type = game_board_.GetType(move.end);
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
Points_t MinimaxMoveEvaluator<
    ConcreteSpaceInfoProvider,
    ConcretePieceValueProvider>::GetPlayerTotal(PieceColor color) {
  Points_t pre_attack_total = 0;
  for (auto space : game_board_.GetAllSpacesOccupiedBy(color)) {
    auto piece_type = game_board_.GetType(space);
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
        int cur_search_depth,
        int alpha,
        int beta,
        PieceColor cur_player,
        bool use_transposition_table
    ) {
  node_counter_ += 1;
  MinimaxResultType result_type{};

  auto state_score_search_result = game_board_.GetEvalResult(
      evaluating_player_,
      cur_search_depth
  );
  if (state_score_search_result.found && use_transposition_table) {
    return state_score_search_result.table_entry.best_moves;
  }

  auto cur_moves = game_board_.CalcFinalMovesOf(cur_player);
  if (cur_moves.moves.size() == 0) {
    result_type = MinimaxResultType::kEndGameLeaf;
    auto result = EvaluateEndOfGameLeaf(cur_player);
    game_board_.RecordEvalResult(
        evaluating_player_,
        cur_search_depth,
        result_type,
        result
    );
    return result;
  }
  if (cur_search_depth == 0) {
    result_type = MinimaxResultType::kStandardLeaf;
    auto result = EvaluateNonWinLeaf(cur_player);
    game_board_.RecordEvalResult(
        evaluating_player_,
        cur_search_depth,
        result_type,
        result
    );
    return result;
  }
  if (cur_player == evaluating_player_) {
    auto max_eval = numeric_limits<int>::min();
    MoveCollection best_moves;
    auto ranked_moves = GenerateRankedMoveList(cur_player, cur_moves);
    for (auto rated_move : ranked_moves) {
      auto executed_move = game_board_.ExecuteMove(rated_move.move);
      auto cur_eval = MinimaxRec(
                          cur_search_depth - 1,
                          alpha,
                          beta,
                          opponent_of(evaluating_player_),
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
      game_board_.UndoMove(executed_move);
      alpha = max(alpha, cur_eval);
      if (beta <= alpha) {
        result_type = MinimaxResultType::kAlphaPrune;
        break;
      }
    }
    auto result = BestMoves{max_eval, best_moves};
    game_board_.RecordEvalResult(
        evaluating_player_,
        cur_search_depth,
        result_type,
        result
    );
    return BestMoves{max_eval, best_moves};
  } else {
    auto min_eval = numeric_limits<int>::max();
    MoveCollection best_moves;
    auto ranked_moves = GenerateRankedMoveList(cur_player, cur_moves);
    for (auto rated_move : ranked_moves) {
      auto executed_move = game_board_.ExecuteMove(rated_move.move);
      auto cur_eval = MinimaxRec(
                          cur_search_depth - 1,
                          alpha,
                          beta,
                          evaluating_player_,
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
      game_board_.UndoMove(executed_move);
      beta = min(beta, cur_eval);
      if (beta <= alpha) {
        result_type = MinimaxResultType::kBetaPrune;
        break;
      }
    }
    result_type = MinimaxResultType::kFullyEvaluatedNode;
    auto result = BestMoves{min_eval, best_moves};
    game_board_.RecordEvalResult(
        evaluating_player_,
        cur_search_depth,
        result_type,
        result
    );
    return result;
  }
}

template <
    typename ConcreteSpaceInfoProvider,
    typename ConcretePieceValueProvider>
Move MinimaxMoveEvaluator<
    ConcreteSpaceInfoProvider,
    ConcretePieceValueProvider>::
    RunMinimax(
        // int search_depth,
        // int alpha,
        // int beta,
        bool use_transposition_table
    ) {
  ResetNodeCounter();
  auto minimax_result = MinimaxRec(
      starting_search_depth_,
      numeric_limits<int>::min(),
      numeric_limits<int>::max(),
      evaluating_player_,
      use_transposition_table
  );
  auto selected_move_index = utility_functs::random(
      (size_t)0,
      minimax_result.best_moves.moves.size() - 1
  );

  return minimax_result.best_moves.moves[selected_move_index];
}

#endif /* MINIMAX_EVALUATOR */
