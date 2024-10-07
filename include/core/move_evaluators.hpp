// Filename: move_evaluators.hpp
// Author: Duane Goodner
// Created: 2022-12-17
// Last Modified: 2024-09-30

// Description:
// Defines PiecePointsEvaluator template class and interfaces that
// PiecePointsEvaluator requires GameBoard and GamePoints classes to comply
// with.

#pragma once

#include <board_data_structs.hpp>
#include <board_state_summarizer_interface.hpp>
#include <common.hpp>
#include <evaluator_details.hpp>
#include <functional>
#include <limits>
#include <piece_value_provider_interface.hpp>
#include <move_evaluator_interface.hpp>
#include <space_info_provider_interface.hpp>
#include <utility_functs.hpp>

using namespace gameboard;
using namespace moves;

namespace moveselection {

// CLASS TEMPLATE: MinimaxMoveEvaluator
// IMPLEMENTS INTERFACE:
//    MoveEvaluator
// USES:
//    ConcreteSpaceInfoProvider (e.g. GameBoard) that implements
//    SpaceInfoProvider.
//    ConcretePieceValueProvider (e.g. PiecePoints) that
//    implements PieceValueProvider
// Uses minimax algorithm with alpha-beta pruning to select a move for
// evaluating_player_.
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
      ConcretePieceValueProvider game_position_points
  );

  MinimaxMoveEvaluator(
      PieceColor evaluating_player,
      int starting_search_depth,
      ConcreteSpaceInfoProvider &game_board
  );

  Move ImplementSelectMove();
  Points_t GetPlayerTotal(PieceColor color);
  inline moveselection::SearchSummaries GetSearchSummaries() { return search_summaries_; }
  inline int StartingSearchDepth() { return starting_search_depth_; }

  inline size_t KeySizeBits() {
    return 8 * sizeof(typename ConcreteBoardStateSummarizer::ZobristKey_t);
  }

private:
  PieceColor evaluating_player_;
  ConcretePieceValueProvider game_position_points_;
  ConcreteBoardStateSummarizer hash_calculator_;
  ConcreteSpaceInfoProvider &game_board_;
  int num_move_selections_;
  int starting_search_depth_;
  moveselection::SearchSummaries search_summaries_;

  BestMoves EvaluateNonWinLeaf(PieceColor cur_player);
  BestMoves EvaluateEndOfGameLeaf(PieceColor cur_player, MinimaxResultType &result_type);
  RatedMove RateMove(Move move, PieceColor cur_player);
  Points_t GetValueOfPieceAtPosition(
      PieceColor color,
      PieceType piece_type,
      BoardSpace space
  );
  std::vector<RatedMove> GenerateRankedMoveList(
      PieceColor cur_player,
      MoveCollection &cur_player_moves
  );
  BestMoves MinimaxRec(
      int remaining_search_depth,
      int alpha,
      int beta,
      PieceColor cur_player,
      SearchSummary &single_search_summary,
      bool use_transposition_table = true
  );
  Move RunMinimax(
      SearchSummary &single_search_summary,
      bool use_transposition_table = true
  );
};

// CLASS TEMPLATE: RandomMoveEvaluator
// IMPLEMENTS INTERFACE:
//    MoveEvaluator
// USES:
//    ConcreteSpaceInfoProvider (e.g. GameBoard) that implements
//    SpaceInfoProvider.
// Randomly chooses one of the legal moves available to evaluating_player_.
template <typename ConcreteSpaceInfoProvider>
class RandomMoveEvaluator
    : public MoveEvaluator<RandomMoveEvaluator<ConcreteSpaceInfoProvider>> {
public:
  RandomMoveEvaluator(
      PieceColor evaluating_player,
      ConcreteSpaceInfoProvider &game_board
  )
      : evaluating_player_{evaluating_player}
      , game_board_{game_board} {};
  Move ImplementSelectMove() {
    auto allowed_moves = game_board_.CalcFinalMovesOf(evaluating_player_);
    auto selected_move_index =
        utility_functs::random((size_t)0, allowed_moves.moves.size() - 1);
    return allowed_moves.moves[selected_move_index];
  };

private:
  PieceColor evaluating_player_;
  ConcreteSpaceInfoProvider &game_board_;
};
} // namespace moveselection

#include <move_evaluators.tpp>
