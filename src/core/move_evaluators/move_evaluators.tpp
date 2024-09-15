// Filename: game_board.tpp
// Author: Duane Goodner
// Created: 2022-12-27
// Last Modified: 2024-08-16

// Description:
// Implements PiecePointsEvaluator template class.

#ifndef _MINIMAX_EVALUATOR_TEMPLATE_
#define _MINIMAX_EVALUATOR_TEMPLATE_

#include <board_components.hpp>
#include <chrono>
#include <common.hpp>
#include <evaluator_details.hpp>
#include <iostream>
#include <limits>
using namespace board_components;

template <
    typename ConcreteSpaceInfoProvider,
    typename ConcreteBoardStateSummarizer,
    typename ConcretePieceValueProvider>
MinimaxMoveEvaluator<
    ConcreteSpaceInfoProvider,
    ConcreteBoardStateSummarizer,
    ConcretePieceValueProvider>::
    MinimaxMoveEvaluator(
        PieceColor evaluating_player,
        int starting_search_depth,
        ConcreteSpaceInfoProvider &game_board,
        ConcretePieceValueProvider game_position_points
    )
    : evaluating_player_{evaluating_player}
    , starting_search_depth_{starting_search_depth}
    , game_board_{game_board}
    , game_position_points_{game_position_points}
    , hash_calculator_{ConcreteBoardStateSummarizer{}}
    , num_move_selections_{0}
    , search_summaries_{} {
  game_board_.AttachMoveCallback(std::bind(
      &ConcreteBoardStateSummarizer::UpdateBoardState,
      &hash_calculator_,
      std::placeholders::_1
  ));
}

template <
    typename ConcreteSpaceInfoProvider,
    typename ConcreteBoardStateSummarizer,
    typename ConcretePieceValueProvider>
MinimaxMoveEvaluator<
    ConcreteSpaceInfoProvider,
    ConcreteBoardStateSummarizer,
    ConcretePieceValueProvider>::
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
    typename ConcreteBoardStateSummarizer,
    typename ConcretePieceValueProvider>
Move MinimaxMoveEvaluator<
    ConcreteSpaceInfoProvider,
    ConcreteBoardStateSummarizer,
    ConcretePieceValueProvider>::ImplementSelectMove() {

  auto &first_search_summary = search_summaries_.NewFirstSearch(starting_search_depth_);
  // SearchSummary first_search_summary{};
  // SearchSummary second_search_summary{};
  Move final_selected_move;

  auto first_selected_move = RunMinimax(first_search_summary);
  // first_search_summaries_.emplace_back(first_search_summary);

  // check if move selected by first search is allowed
  // (if we had a hash collision, we might select an illegal move)
  auto allowed_moves = game_board_.CalcFinalMovesOf(evaluating_player_);
  if (allowed_moves.ContainsMove(first_selected_move)) {
    final_selected_move = first_selected_move;
  } else {
    auto &second_search_summary =
        search_summaries_.NewExtraSearch(starting_search_depth_, num_move_selections_);
    auto second_selected_move = RunMinimax(second_search_summary, false);
    // second_search_summaries_[num_move_selections_] = second_search_summary;
    final_selected_move = second_selected_move;
  }
  num_move_selections_++;
  return final_selected_move;
}

template <
    typename ConcreteSpaceInfoProvider,
    typename ConcreteBoardStateSummarizer,
    typename ConcretePieceValueProvider>
std::vector<RatedMove> MinimaxMoveEvaluator<
    ConcreteSpaceInfoProvider,
    ConcreteBoardStateSummarizer,
    ConcretePieceValueProvider>::
    GenerateRankedMoveList(PieceColor cur_player, MoveCollection &cur_player_moves) {
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
    typename ConcreteBoardStateSummarizer,
    typename ConcretePieceValueProvider>
BestMoves MinimaxMoveEvaluator<
    ConcreteSpaceInfoProvider,
    ConcreteBoardStateSummarizer,
    ConcretePieceValueProvider>::EvaluateNonWinLeaf(PieceColor cur_player) {
  auto cur_player_points = GetPlayerTotal(cur_player);
  auto opponent_points = GetPlayerTotal(opponent_of(cur_player));

  auto empty_move_collection = MoveCollection();

  if (cur_player == evaluating_player_) {
    return BestMoves{(cur_player_points - opponent_points), empty_move_collection};
  } else {
    return BestMoves{(opponent_points - cur_player_points), empty_move_collection};
  }
}

template <
    typename ConcreteSpaceInfoProvider,
    typename ConcreteBoardStateSummarizer,
    typename ConcretePieceValueProvider>
BestMoves MinimaxMoveEvaluator<
    ConcreteSpaceInfoProvider,
    ConcreteBoardStateSummarizer,
    ConcretePieceValueProvider>::
    EvaluateEndOfGameLeaf(PieceColor cur_player, MinimaxResultType &result_type) {
  auto empty_best_moves = MoveCollection();

  if (cur_player == evaluating_player_) {
    result_type = MinimaxResultType::kEvaluatorLoses;
    return BestMoves{numeric_limits<Points_t>::min(), empty_best_moves};
  } else {
    result_type = MinimaxResultType::kEvaluatorWins;
    return BestMoves{numeric_limits<Points_t>::max(), empty_best_moves};
  }
}

template <
    typename ConcreteSpaceInfoProvider,
    typename ConcreteBoardStateSummarizer,
    typename ConcretePieceValueProvider>
RatedMove MinimaxMoveEvaluator<
    ConcreteSpaceInfoProvider,
    ConcreteBoardStateSummarizer,
    ConcretePieceValueProvider>::RateMove(Move move, PieceColor cur_player) {
  auto piece_type = game_board_.GetType(move.start);

  auto end_points =
      game_position_points_.GetValueOfPieceAtPosition(cur_player, piece_type, move.end);
  auto start_points = game_position_points_
                          .GetValueOfPieceAtPosition(cur_player, piece_type, move.start);
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
    typename ConcreteBoardStateSummarizer,
    typename ConcretePieceValueProvider>
Points_t MinimaxMoveEvaluator<
    ConcreteSpaceInfoProvider,
    ConcreteBoardStateSummarizer,
    ConcretePieceValueProvider>::
    GetValueOfPieceAtPosition(PieceColor color, PieceType piece_type, BoardSpace space) {
  return game_position_points_.GetValueOfPieceAtPosition(color, piece_type, space);
}

template <
    typename ConcreteSpaceInfoProvider,
    typename ConcreteBoardStateSummarizer,
    typename ConcretePieceValueProvider>
Points_t MinimaxMoveEvaluator<
    ConcreteSpaceInfoProvider,
    ConcreteBoardStateSummarizer,
    ConcretePieceValueProvider>::GetPlayerTotal(PieceColor color) {
  Points_t pre_attack_total = 0;
  for (auto space : game_board_.GetAllSpacesOccupiedBy(color)) {
    auto piece_type = game_board_.GetType(space);
    pre_attack_total +=
        game_position_points_.GetValueOfPieceAtPosition(color, piece_type, space);
  }
  return pre_attack_total;
}

template <
    typename ConcreteSpaceInfoProvider,
    typename ConcreteBoardStateSummarizer,
    typename ConcretePieceValueProvider>
BestMoves MinimaxMoveEvaluator<
    ConcreteSpaceInfoProvider,
    ConcreteBoardStateSummarizer,
    ConcretePieceValueProvider>::
    MinimaxRec(
        int cur_search_depth,
        int alpha,
        int beta,
        PieceColor cur_player,
        SearchSummary &search_summary,
        bool use_transposition_table
    ) {
  search_summary.num_nodes++;
  MinimaxResultType result_type{};

  // First we check if result for current board state is in transposition table (unless
  // this is a second search in which case we don't use transposition table)
  if (use_transposition_table) {
    auto state_score_search_result = hash_calculator_.GetTrData(cur_search_depth);
    if (state_score_search_result.found) {
      
      // TODO split into 3 cases: standard, evaluator loses, and evaluator wins
      // Can just look at result type of table entry
      auto existing_result_type = state_score_search_result.table_entry.result_type;
      if (existing_result_type == MinimaxResultType::kEvaluatorLoses) {
        result_type = MinimaxResultType::kTrTableHitEvaluatorLoses;
      } else if (existing_result_type == MinimaxResultType::kTrTableHitEvaluatorWins) {
        result_type = MinimaxResultType::kTrTableHitEvaluatorWins;
      } else {
        result_type = MinimaxResultType::kTrTableHitStandard;
      }
      auto result = state_score_search_result.table_entry.best_moves;
      search_summary.Update(result_type, cur_search_depth, result);
      return result;
    }
  }

  // Get all legal moves
  auto cur_moves = game_board_.CalcFinalMovesOf(cur_player);

  // If no legal moves, node is an end-of-game leaf
  if (cur_moves.moves.size() == 0) {
    auto result = EvaluateEndOfGameLeaf(cur_player, result_type);
    hash_calculator_.RecordTrData(cur_search_depth, result_type, result);
    search_summary.Update(result_type, cur_search_depth, result);
    // search_summary.result_counts[result_type]++;
    return result;
  }
  // If search depth is zero, node is a normal leaf (end of our search depth)
  if (cur_search_depth == 0) {
    result_type = MinimaxResultType::kStandardLeaf;
    auto result = EvaluateNonWinLeaf(cur_player);
    hash_calculator_.RecordTrData(cur_search_depth, result_type, result);
    search_summary.Update(result_type, cur_search_depth, result);
    // search_summary.result_counts[result_type]++;
    return result;
  }

  if (cur_player == evaluating_player_) {
    // evaluation of each legal move when it's evaluating player's turn
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
                          search_summary,
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
    //  if we get here and result type is still unknown, then we fully evaluated the
    //  node. Note that "full evaluation" just means not end of search. Could be the
    // root node or could be interior node. Recommend finding a way to store search depth
    // in SearchResult
    if (result_type == MinimaxResultType::kUnknown) {
      result_type = MinimaxResultType::kFullyEvaluatedNode;
    }
    auto result = BestMoves{max_eval, best_moves};
    hash_calculator_.RecordTrData(cur_search_depth, result_type, result);
    search_summary.Update(result_type, cur_search_depth, result);
    // search_summary.result_counts[result_type]++;
    return BestMoves{max_eval, best_moves};

  } else {
    // evaluation of each legal move when it's evaluating player's opponent's turn
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
                          search_summary,
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
    if (result_type == MinimaxResultType::kUnknown) {
      result_type = MinimaxResultType::kFullyEvaluatedNode;
    }
    auto result = BestMoves{min_eval, best_moves};
    hash_calculator_.RecordTrData(cur_search_depth, result_type, result);
    search_summary.Update(result_type, cur_search_depth, result);
    // search_summary.result_counts[result_type]++;
    return result;
  }
}

template <
    typename ConcreteSpaceInfoProvider,
    typename ConcreteBoardStateSummarizer,
    typename ConcretePieceValueProvider>
Move MinimaxMoveEvaluator<
    ConcreteSpaceInfoProvider,
    ConcreteBoardStateSummarizer,
    ConcretePieceValueProvider>::
    RunMinimax(SearchSummary &search_summary, bool use_transposition_table) {

  auto search_start = std::chrono::high_resolution_clock::now();
  auto minimax_result = MinimaxRec(
      starting_search_depth_,
      numeric_limits<int>::min(),
      numeric_limits<int>::max(),
      evaluating_player_,
      search_summary,
      use_transposition_table
  );
  auto search_end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::nano> search_time = search_end - search_start;
  search_summary.SetTime(search_time);
  // search_summary.time = search_time;
  auto selected_move_index =
      utility_functs::random((size_t)0, minimax_result.best_moves.moves.size() - 1);
  auto selected_move = minimax_result.best_moves.moves[selected_move_index];
  search_summary.SetSelectedMove(selected_move);

  return selected_move;
}

#endif /* MINIMAX_EVALUATOR */
