//! @file move_evaluators.hpp
//! Definitions of concrete classes that implement the MoveEvaluator interface.

#pragma once

#include <board_data_structs.hpp>
#include <board_state_summarizer_interface.hpp>
#include <evaluator_data_structs.hpp>
#include <functional>
#include <limits>
#include <move_evaluator_interface.hpp>
#include <piece_value_provider_interface.hpp>
#include <space_info_provider_interface.hpp>
#include <utility_functs.hpp>

using namespace gameboard;

namespace moveselection {

//! Implements MoveEvaluator interface, and selects move::Move based on Minimax
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
      int starting_search_depth,
      ConcreteSpaceInfoProvider &game_board,
      const ConcretePieceValueProvider &game_position_points,
      uint32_t zkey_seed
  );

  MinimaxMoveEvaluator(
      PieceColor evaluating_player,
      int starting_search_depth,
      ConcreteSpaceInfoProvider &game_board,
      const ConcretePieceValueProvider &game_position_points
  );

  MinimaxMoveEvaluator(
      PieceColor evaluating_player,
      int starting_search_depth,
      ConcreteSpaceInfoProvider &game_board,
      uint32_t zkey_seed
  );

  MinimaxMoveEvaluator(
      PieceColor evaluating_player,
      int starting_search_depth,
      ConcreteSpaceInfoProvider &game_board
  );

  Move ImplementSelectMove(MoveCollection &allowed_moves);
  Points_t GetPlayerTotal(PieceColor color);
  inline const moveselection::SearchSummaries &search_summaries() {
    return search_summaries_;
  }
  inline int StartingSearchDepth() { return starting_search_depth_; }

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
  PieceColor evaluating_player_;
  ConcretePieceValueProvider game_position_points_;
  ConcreteBoardStateSummarizer hash_calculator_;
  ConcreteSpaceInfoProvider &game_board_;
  int num_move_selections_;
  int starting_search_depth_;
  moveselection::SearchSummaries search_summaries_;

  EqualScoreMoves EvaluateNonWinLeaf(PieceColor cur_player);
  EqualScoreMoves EvaluateEndOfGameLeaf(
      PieceColor cur_player,
      MinimaxResultType &result_type
  );
  ScoredMove RateMove(Move move, PieceColor cur_player);
  Points_t GetValueOfPieceAtPosition(
      PieceColor color,
      PieceType piece_type,
      BoardSpace space
  );
  std::vector<ScoredMove> GenerateRankedMoveList(
      PieceColor cur_player,
      MoveCollection &cur_player_moves
  );
  EqualScoreMoves MinimaxRec(
      MoveCollection &allowed_moves,
      int remaining_search_depth,
      int alpha,
      int beta,
      PieceColor cur_player,
      SearchSummary &single_search_summary,
      bool use_transposition_table = true
  );
  bool ValidateMove(SearchSummary &search_summary, MoveCollection &allowed_moves);
  void RunMinimax(
      MoveCollection& allowed_moves,
      SearchSummary &single_search_summary,
      bool use_transposition_table = true
  );
  SearchSummary& RunFirstSearch(MoveCollection &allowed_moves);
  SearchSummary& RunSecondSearch(MoveCollection &allowed_moves);
  void IncrementNumMoveSelections() { num_move_selections_++; }

  // bool IsTrTableResultAcceptable(, MoveCollection &allowed_moves) {

  // }
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
  );
  Move ImplementSelectMove(MoveCollection &allowed_moves);

private:
  PieceColor evaluating_player_;
  ConcreteSpaceInfoProvider &game_board_;
};
} // namespace moveselection

#include <minimax_move_evaluator.tpp>
#include <random_move_evaluator.tpp>
