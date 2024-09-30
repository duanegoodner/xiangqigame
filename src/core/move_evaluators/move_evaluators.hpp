// Filename: move_evaluators.hpp
// Author: Duane Goodner
// Created: 2022-12-17
// Last Modified: 2024-09-30

// Description:
// Defines PiecePointsEvaluator template class and interfaces that
// PiecePointsEvaluator requires NewGameBoard and GamePoints classes to comply
// with.

#ifndef _MINIMAX_EVALUATOR_
#define _MINIMAX_EVALUATOR_

#include <board_components.hpp>
#include <common.hpp>
#include <evaluator_details.hpp>
#include <functional>
#include <limits>
#include <utility_functs.hpp>

using namespace board_components;

// CRTP INTERFACE: Evaluator <- SpaceInfoProvider (concrete example =
// New GameBoard)
template <typename ConcreteSpaceInfoProvider>
class SpaceInfoProvider {
public:
  vector<BoardSpace> GetAllSpacesOccupiedBy(PieceColor color) {
    return static_cast<ConcreteSpaceInfoProvider *>(this)
        ->ImplementGetAllSpacesOccupiedBy(color);
  }

  PieceColor GetColor(BoardSpace space) {
    return static_cast<ConcreteSpaceInfoProvider *>(this)->ImplementGetColor(space);
  }

  PieceType GetType(BoardSpace space) {
    return static_cast<ConcreteSpaceInfoProvider *>(this)->ImplementGetType(space);
  }

  MoveCollection CalcFinalMovesOf(PieceColor color) {
    return static_cast<ConcreteSpaceInfoProvider *>(this)->ImplementCalcFinalMovesOf(
        color
    );
  };

  ExecutedMove ExecuteMove(Move move) {
    return static_cast<ConcreteSpaceInfoProvider *>(this)->ImplementExecuteMove(move);
  }

  void UndoMove(ExecutedMove executed_move) {
    static_cast<ConcreteSpaceInfoProvider *>(this)->ImplementUndoMove(executed_move);
  }

  void AttachMoveCallback(function<void(ExecutedMove)> callback) {
    static_cast<ConcreteSpaceInfoProvider *>(this)->ImplementAttachMoveCallback(callback
    );
  }
};

// CRTP Interface: Evaluator <- BoardStateSummarizer (e.g. HashCalculator)
template <typename ConcreteBoardStateSummarizer, typename KeyType>
class BoardStateSummarizer {
public:
  typedef KeyType ZobristKey_t;
  void FullBoardStateCalc(BoardMap_t &board_map) {
    static_cast<ConcreteBoardStateSummarizer *>(this)->ImplementFullBoardStateCalc(
        board_map
    );
  }

  void UpdateBoardState(const ExecutedMove &move) {
    return static_cast<ConcreteBoardStateSummarizer *>(this)->ImplementUpdateBoardState(
        move
    );
  }

  ZobristKey_t GetState() {
    return static_cast<ConcreteBoardStateSummarizer *>(this)->ImplementGetState();
  }

  void RecordTrData(
      int search_depth,
      MinimaxResultType result_type,
      BestMoves &best_moves
  ) {
    return static_cast<ConcreteBoardStateSummarizer *>(this)
        ->ImplementRecordTrData(search_depth, result_type, best_moves);
  }

  TranspositionTableSearchResult GetTrData(int search_depth) {
    return static_cast<ConcreteBoardStateSummarizer *>(this)->ImplementGetTrData(
        search_depth
    );
  }
};

// CRTP Interface: Evaluator <- GamePoints
template <typename ConcretePieceValueProvider>
class PieceValueProvider {
public:
  // typedef Points_t PieceValue_t;
  Points_t GetValueOfPieceAtPosition(
      PieceColor color,
      PieceType piece_type,
      BoardSpace space
  ) {
    return static_cast<ConcretePieceValueProvider *>(this)
        ->ImplementGetValueOfPieceAtPosition(color, piece_type, space);
  }
};

// CRTP Interface: AIPlayer <- MoveEvaluatorInterface
// Currently not using since AI Player is currently in Python side of app.
// If/when implement AI Player in C++, will move this interface definition to
// C++ Player header file.
template <typename ConcreteMoveEvaluator>
class MoveEvaluatorInterface {
public:
  Move SelectMove() {
    return static_cast<ConcreteMoveEvaluator *>(this)->ImplementSelectMove(
        // cur_player
    );
  }
};

// CLASS TEMPLATE: MinimaxMoveEvaluator
// IMPLEMENTS INTERFACE:
//    MoveEvaluatorInterface
// USES:
//    ConcreteSpaceInfoProvider (e.g. NewGameBoard) that implements
//    SpaceInfoProvider.
//    ConcretePieceValueProvider (e.g. PiecePoints) that
//    implements PieceValueProvider
// Uses minimax algorithm with alpha-beta pruning to select a move for
// evaluating_player_.
template <
    typename ConcreteSpaceInfoProvider,
    typename ConcreteBoardStateSummarizer,
    typename ConcretePieceValueProvider>
class MinimaxMoveEvaluator : public MoveEvaluatorInterface<MinimaxMoveEvaluator<
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
  inline SearchSummaries GetSearchSummaries() {
    return search_summaries_;
  }
  inline int StartingSearchDepth() {
    return starting_search_depth_;
  }

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
  SearchSummaries search_summaries_;
  

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
      SearchSummary& single_search_summary,
      bool use_transposition_table = true
  );
  Move RunMinimax(
      SearchSummary &single_search_summary,
      bool use_transposition_table = true
  );

};

// CLASS TEMPLATE: RandomMoveEvaluator
// IMPLEMENTS INTERFACE:
//    MoveEvaluatorInterface
// USES:
//    ConcreteSpaceInfoProvider (e.g. NewGameBoard) that implements
//    SpaceInfoProvider.
// Randomly chooses one of the legal moves available to evaluating_player_.
template <typename ConcreteSpaceInfoProvider>
class RandomMoveEvaluator
    : public MoveEvaluatorInterface<RandomMoveEvaluator<ConcreteSpaceInfoProvider>> {
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

#include <move_evaluators.tpp>

#endif /* MINIMAX_EVALUATOR */
