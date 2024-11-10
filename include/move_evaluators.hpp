//! @file move_evaluators.hpp
//! Definitions of concrete classes that implement the MoveEvaluator interface.

#pragma once

#include <array>
#include <board_data_structs.hpp>
#include <board_state_summarizer_interface.hpp>
#include <evaluator_data_structs.hpp>
#include <functional>
#include <limits>
#include <move_evaluator_interface.hpp>
#include <piece_value_provider_interface.hpp>
#include <space_info_provider_interface.hpp>
#include <unordered_map>
#include <utility_functs.hpp>

using namespace gameboard;

namespace moveselection {

//! Implements MoveEvaluator interface, and selects move::Move using Minimax
//! algorithm; uses SpaceInfoProvider, BoardStateSummarizer, and PieceValueProvider
//! interfaces.
template <
    typename ConcreteSpaceInfoProvider,
    typename ConcreteBoardStateSummarizer,
    typename ConcretePieceValueProvider>
class MinimaxMoveEvaluator : public MoveEvaluator<MinimaxMoveEvaluator<
                                 ConcreteSpaceInfoProvider,
                                 ConcreteBoardStateSummarizer,
                                 ConcretePieceValueProvider>> {

public:
  MinimaxMoveEvaluator(
      PieceColor evaluating_player,
      DepthType starting_search_depth,
      ConcreteSpaceInfoProvider &game_board,
      const ConcretePieceValueProvider &game_position_points
  )
      : MinimaxMoveEvaluator(
            evaluating_player,
            starting_search_depth,
            game_board,
            game_position_points,
            std::random_device{}()
        ) {}

  MinimaxMoveEvaluator(
      PieceColor evaluating_player,
      DepthType starting_search_depth,
      ConcreteSpaceInfoProvider &game_board,
      uint32_t zkey_seed
  )
      : MinimaxMoveEvaluator(
            evaluating_player,
            starting_search_depth,
            game_board,
            ConcretePieceValueProvider(),
            zkey_seed
        ) {}

  MinimaxMoveEvaluator(
      PieceColor evaluating_player,
      DepthType starting_search_depth,
      ConcreteSpaceInfoProvider &game_board
  )
      : MinimaxMoveEvaluator(
            evaluating_player,
            starting_search_depth,
            game_board,
            ConcretePieceValueProvider(),
            std::random_device{}()
        ) {}

  Move ImplementSelectMove(MoveCollection &allowed_moves) {

    Move final_selected_move;

    auto &first_search_summary = RunFirstSearch(allowed_moves);

    if (ValidateMove(first_search_summary, allowed_moves)) {
      final_selected_move = first_search_summary.selected_move();
    } else {
      auto &second_search_summary = RunSecondSearch(allowed_moves);
      final_selected_move = second_search_summary.selected_move();
    }

    IncrementNumMoveSelections();
    return final_selected_move;
  }

  inline const moveselection::SearchSummaries &search_summaries() {
    return search_summaries_;
  }

  inline DepthType StartingSearchDepth() { return starting_search_depth_; }

  inline size_t KeySizeBits() {
    return 8 * sizeof(typename ConcreteBoardStateSummarizer::ZobristKey_t);
  }

  const ConcreteBoardStateSummarizer &hash_calculator() const {
    return hash_calculator_;
  }

  const uint32_t zkeys_seed() { return hash_calculator_.zkeys_seed(); }

  const std::string board_state_hex_str() {
    return hash_calculator_.board_state_hex_str();
  }

private:
  explicit MinimaxMoveEvaluator(
      PieceColor evaluating_player,
      DepthType starting_search_depth,
      ConcreteSpaceInfoProvider &game_board,
      const ConcretePieceValueProvider &game_position_points,
      uint32_t zkey_seed
  )
      : evaluating_player_{evaluating_player}
      , starting_search_depth_{starting_search_depth}
      , game_board_{game_board}
      , game_position_points_{game_position_points}
      , hash_calculator_{ConcreteBoardStateSummarizer{zkey_seed}}
      , num_move_selections_{0}
      , search_summaries_{} {
    game_board_.AttachMoveCallback(std::bind(
        &ConcreteBoardStateSummarizer::UpdateBoardState,
        &hash_calculator_,
        std::placeholders::_1
    ));
    hash_calculator_.FullBoardStateCalc(game_board_.map());
  }

  PieceColor evaluating_player_;
  ConcretePieceValueProvider game_position_points_;
  ConcreteBoardStateSummarizer hash_calculator_;
  ConcreteSpaceInfoProvider &game_board_;
  MoveCountType num_move_selections_;
  DepthType starting_search_depth_;
  moveselection::SearchSummaries search_summaries_;

  Points_t GetPlayerTotal(PieceColor color) {
    Points_t pre_attack_total = 0;
    for (auto space : game_board_.GetAllSpacesOccupiedBy(color)) {
      auto piece_type = game_board_.GetType(space);
      pre_attack_total +=
          game_position_points_.GetValueOfPieceAtPosition(color, piece_type, space);
    }
    return pre_attack_total;
  }

  ScoredMove RateMove(Move move, PieceColor cur_player) {
    auto piece_type = game_board_.GetType(move.start);

    auto end_points = game_position_points_
                          .GetValueOfPieceAtPosition(cur_player, piece_type, move.end);
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

    return ScoredMove{move, (position_value_delta + capture_val)};
  }

  Points_t GetValueOfPieceAtPosition(
      PieceColor color,
      PieceType piece_type,
      BoardSpace space
  ) {
    return game_position_points_.GetValueOfPieceAtPosition(color, piece_type, space);
  }

  std::vector<ScoredMove> GenerateRankedMoveList(
      PieceColor cur_player,
      MoveCollection &cur_player_moves
  ) {
    vector<ScoredMove> rated_moves;
    for (auto cur_move : cur_player_moves.moves) {
      auto cur_rated_move = RateMove(cur_move, cur_player);
      rated_moves.emplace_back(cur_rated_move);
    }
    sort(
        rated_moves.begin(),
        rated_moves.end(),
        [](const ScoredMove &move_a, const ScoredMove &move_b) {
          return move_a.score > move_b.score;
        }
    );
    return rated_moves;
  }

  bool ValidateMove(SearchSummary &search_summary, MoveCollection &allowed_moves) {
    bool is_selected_move_allowed =
        allowed_moves.ContainsMove(search_summary.selected_move());
    if (!is_selected_move_allowed) {
      search_summary.set_returned_illegal_move(true);
    }
    return is_selected_move_allowed;
  }

  SearchSummary &RunFirstSearch(MoveCollection &allowed_moves) {
    auto &first_search_summary = search_summaries_.NewFirstSearch(
        starting_search_depth_,
        hash_calculator_.GetTrTableSize()
    );
    RunMinimax(allowed_moves, first_search_summary);

    return first_search_summary;
  };

  SearchSummary &RunSecondSearch(MoveCollection &allowed_moves) {
    auto &second_search_summary = search_summaries_.NewExtraSearch(
        starting_search_depth_,
        num_move_selections_,
        hash_calculator_.GetTrTableSize()
    );
    RunMinimax(allowed_moves, second_search_summary, false);

    return second_search_summary;
  }

  inline void IncrementNumMoveSelections() {
    num_move_selections_++;
    hash_calculator_.UpdateMoveCounter();
  }

  EqualScoreMoves HandleTrTableHit(
      SearchSummary &search_summary,
      MinimaxResultType &result_type,
      TranspositionTableSearchResult &tr_table_search_result,
      DepthType remaining_search_depth
  ) {
    result_type = MinimaxResultType::kTrTableHit;
    search_summary.RecordTrTableHit(tr_table_search_result, remaining_search_depth);
    return tr_table_search_result.score_and_moves();
  }

  EqualScoreMoves EvaluateEndOfGameLeaf(
      PieceColor cur_player,
      MinimaxResultType &result_type
  ) {
    auto empty_best_moves = MoveCollection();

    if (game_board_.IsDraw()) {
      result_type = MinimaxResultType::kDraw;
      return EqualScoreMoves{0, empty_best_moves};
    }

    if (cur_player == evaluating_player_) {
      result_type = MinimaxResultType::kEvaluatorLoses;
      return EqualScoreMoves{numeric_limits<Points_t>::min(), empty_best_moves};
    } else {
      result_type = MinimaxResultType::kEvaluatorWins;
      return EqualScoreMoves{numeric_limits<Points_t>::max(), empty_best_moves};
    }
  }

  EqualScoreMoves HandleEndOfGame(
      PieceColor cur_player,
      SearchSummary &search_summary,
      MinimaxResultType &result_type,
      DepthType remaining_search_depth
  ) {
    auto result = EvaluateEndOfGameLeaf(cur_player, result_type);
    hash_calculator_
        .RecordTrData(remaining_search_depth, result_type, result, num_move_selections_);
    search_summary.RecordNodeInfo(result_type, remaining_search_depth, result);
    return result;
  }

  EqualScoreMoves EvaluateNonWinLeaf(
      PieceColor cur_player,
      MinimaxResultType &result_type
  ) {
    result_type = MinimaxResultType::kStandardLeaf;

    auto cur_player_points = GetPlayerTotal(cur_player);
    auto opponent_points = GetPlayerTotal(opponent_of(cur_player));

    auto empty_move_collection = MoveCollection();

    if (cur_player == evaluating_player_) {
      return EqualScoreMoves{
          (cur_player_points - opponent_points),
          empty_move_collection
      };
    } else {
      return EqualScoreMoves{
          (opponent_points - cur_player_points),
          empty_move_collection
      };
    }
  };

  EqualScoreMoves HandleLeaf(
      PieceColor cur_player,
      SearchSummary &search_summary,
      MinimaxResultType &result_type,
      DepthType remaining_search_depth
  ) {
    auto result = EvaluateNonWinLeaf(cur_player, result_type);
    hash_calculator_
        .RecordTrData(remaining_search_depth, result_type, result, num_move_selections_);
    search_summary.RecordNodeInfo(result_type, remaining_search_depth, result);

    return result;
  }

  inline bool IsImprovement(
      Points_t cur_eval,
      Points_t previous_best_eval,
      PieceColor cur_player
  ) {
    if (cur_player == evaluating_player_) {
      return cur_eval > previous_best_eval;
    } else {
      return cur_eval < previous_best_eval;
    }
  }

  void UpdateBestMoves(
      PieceColor cur_player,
      Move move,
      MoveCollection &best_moves,
      Points_t cur_eval,
      Points_t &previous_best_eval
  ) {
    if (cur_eval == previous_best_eval) {
      best_moves.Append(move);
    } else if (IsImprovement(cur_eval, previous_best_eval, cur_player)) {
      previous_best_eval = cur_eval;
      best_moves.moves.clear();
      best_moves.Append(move);
    }
  }

  EqualScoreMoves FinalizeNodeResult(
      MinimaxResultType &result_type,
      Points_t best_eval,
      MoveCollection best_moves,
      DepthType remaining_search_depth,
      SearchSummary &search_summary
  ) {

    // If result_type is still unknown by the time FinalizeNodeResult is called, then we
    // have evaluated all moves from (i.e. children of) the node. Can achieve this for
    // root node or interior node, but not for a leaf node.
    if (result_type == MinimaxResultType::kUnknown) {
      result_type = MinimaxResultType::kFullyEvaluatedNode;
    }
    EqualScoreMoves result{best_eval, best_moves};
    hash_calculator_
        .RecordTrData(remaining_search_depth, result_type, result, num_move_selections_);
    search_summary.RecordNodeInfo(result_type, remaining_search_depth, result);
    return result;
  }

  inline void UpdateAlpha(Points_t &alpha, Points_t cur_eval) {
    alpha = max(alpha, cur_eval);
  }

  inline void UpdateBeta(Points_t &beta, Points_t cur_eval) {
    beta = min(beta, cur_eval);
  }

  inline bool IsPrunableForEvaluator(
      Points_t &alpha,
      Points_t &beta,
      MinimaxResultType &result_type
  ) {
    bool is_prunable = (beta <= alpha);
    if (is_prunable) {
      result_type = MinimaxResultType::kAlphaPrune;
    }
    return is_prunable;
  }

  inline bool IsPrunableForEvaluatorOpponent(
      Points_t &alpha,
      Points_t &beta,
      MinimaxResultType &result_type
  ) {
    bool is_prunable = (beta <= alpha);
    if (is_prunable) {
      result_type = MinimaxResultType::kBetaPrune;
    }
    return is_prunable;
  }

  inline bool IsPrunable(
      Points_t &alpha,
      Points_t &beta,
      MinimaxResultType &result_type,
      PieceColor cur_player
  ) {
    if (cur_player == evaluating_player_) {
      return IsPrunableForEvaluator(alpha, beta, result_type);
    } else {
      return IsPrunableForEvaluatorOpponent(alpha, beta, result_type);
    }
  }

  Points_t RecursivelyVisitNodes(
      Move move,
      PieceColor cur_player,
      MoveCollection &allowed_moves,
      DepthType remaining_search_depth,
      Points_t alpha,
      Points_t beta,
      SearchSummary &search_summary,
      bool use_transposition_table
  ) {
    auto executed_move = game_board_.ExecuteMove(move);
    auto new_allowed_moves = game_board_.CalcFinalMovesOf(opponent_of(cur_player));
    auto cur_eval = MinimaxRec(
                        new_allowed_moves,
                        remaining_search_depth - 1,
                        alpha,
                        beta,
                        opponent_of(cur_player),
                        search_summary,
                        use_transposition_table
    )
                        .shared_score;
    game_board_.UndoMove(executed_move);
    return cur_eval;
  }

  inline Points_t InitializedBestEval(PieceColor cur_player) {
    if (cur_player == evaluating_player_) {
      return numeric_limits<Points_t>::min();
    } else {
      return numeric_limits<Points_t>::max();
    }
  }

  inline void UpdatePruningParam(
      Points_t &alpha,
      Points_t &beta,
      Points_t cur_eval,
      PieceColor cur_player
  ) {
    if (cur_player == evaluating_player_) {
      UpdateAlpha(alpha, cur_eval);
    } else {
      UpdateBeta(beta, cur_eval);
    }
  }

  EqualScoreMoves HandleInternalNode(
      PieceColor cur_player,
      MoveCollection &allowed_moves,
      DepthType &remaining_search_depth,
      Points_t &alpha,
      Points_t &beta,
      MinimaxResultType result_type,
      SearchSummary &search_summary,
      bool use_transposition_table
  ) {
    auto max_eval = InitializedBestEval(cur_player);
    MoveCollection best_moves;
    auto ranked_moves = GenerateRankedMoveList(cur_player, allowed_moves);

    for (auto rated_move : ranked_moves) {
      auto cur_eval = RecursivelyVisitNodes(
          rated_move.move,
          cur_player,
          allowed_moves,
          remaining_search_depth,
          alpha,
          beta,
          search_summary,
          use_transposition_table
      );

      UpdateBestMoves(cur_player, rated_move.move, best_moves, cur_eval, max_eval);

      UpdatePruningParam(alpha, beta, cur_eval, cur_player);

      if (IsPrunable(alpha, beta, result_type, cur_player)) {
        break;
      }
    }

    return FinalizeNodeResult(
        result_type,
        max_eval,
        best_moves,
        remaining_search_depth,
        search_summary
    );
  }

  EqualScoreMoves MinimaxRec(
      MoveCollection &allowed_moves,
      DepthType remaining_search_depth,
      Points_t alpha,
      Points_t beta,
      PieceColor cur_player,
      SearchSummary &search_summary,
      bool use_transposition_table
  ) {
    MinimaxResultType result_type{};

    // Check if result for current board state is in transposition table
    // (unless this is a second search in which case we don't use transposition table)
    if (use_transposition_table) {
      auto tr_table_search_result =
          hash_calculator_.GetTrData(remaining_search_depth, num_move_selections_);
      if (tr_table_search_result.found() &&
          tr_table_search_result.IsConsistentWith(allowed_moves)) {
        return HandleTrTableHit(
            search_summary,
            result_type,
            tr_table_search_result,
            remaining_search_depth
        );
      }
    }

    // If no legal moves, node is an end-of-game leaf
    if (allowed_moves.moves.size() == 0) {
      return HandleEndOfGame(
          cur_player,
          search_summary,
          result_type,
          remaining_search_depth
      );
    }

    // If remaining search depth is zero, treat node as "normal" leaf (not end of game,
    // just end of search)
    if (remaining_search_depth == 0) {
      return HandleLeaf(cur_player, search_summary, result_type, remaining_search_depth);
    }

    // If not end-of-game, nor end-of-search, treat node as internal node.
    return HandleInternalNode(
        cur_player,
        allowed_moves,
        remaining_search_depth,
        alpha,
        beta,
        result_type,
        search_summary,
        use_transposition_table
    );
  }

  void RunMinimax(
      MoveCollection &allowed_moves,
      SearchSummary &search_summary,
      bool use_transposition_table = true
  ) {

    auto search_start = std::chrono::high_resolution_clock::now();
    auto minimax_result = MinimaxRec(
        allowed_moves,
        starting_search_depth_,
        numeric_limits<Points_t>::min(),
        numeric_limits<Points_t>::max(),
        evaluating_player_,
        search_summary,
        use_transposition_table
    );
    auto search_end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::nano> search_time = search_end - search_start;
    search_summary.set_time(search_time);
    auto selected_move_index = utility_functs::random(
        (size_t)0,
        minimax_result.move_collection().moves.size() - 1
    );
    auto selected_move = minimax_result.move_collection().moves[selected_move_index];
    search_summary.SetSelectedMove(selected_move);
    auto tr_table_size = hash_calculator_.GetTrTableSize();
    search_summary.set_tr_table_size_final(tr_table_size);

    // return selected_move;
  }
};

//! Implements gameboard::MoveEvaluator interface. Randomly chooses one of legal moves
//! available to moveselection::RandomMoveEvaluator.evaluating_player_.
template <typename ConcreteSpaceInfoProvider>
class RandomMoveEvaluator
    : public MoveEvaluator<RandomMoveEvaluator<ConcreteSpaceInfoProvider>> {
public:
  RandomMoveEvaluator(
      PieceColor evaluating_player,
      ConcreteSpaceInfoProvider &game_board
  )
      : evaluating_player_{evaluating_player}
      , game_board_{game_board} {}

  Move ImplementSelectMove(MoveCollection &allowed_moves) {
    auto selected_move_index =
        utility_functs::random((size_t)0, allowed_moves.moves.size() - 1);
    return allowed_moves.moves[selected_move_index];
  }

private:
  PieceColor evaluating_player_;
  ConcreteSpaceInfoProvider &game_board_;
};
} // namespace moveselection
