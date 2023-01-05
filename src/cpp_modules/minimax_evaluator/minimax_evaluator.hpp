#ifndef _MINIMAX_EVALUATOR_3_
#define _MINIMAX_EVALUATOR_3_

#include <board_components.hpp>
#include <game_board.hpp>
#include <move_selector.hpp>
// #include <piece_points.hpp>

using namespace board_components;
using namespace piece_points;


template <typename ImplementedEvaluator> class EvaluatorInterface {
public:
  RatedMove RateMove(Move move, GameBoard &game_board, PieceColor cur_player) {
    return static_cast<ImplementedEvaluator *>(this)
        ->ImplementRateMove(move, game_board, cur_player);
  }

  BestMoves EvaluateNonWinLeaf(
      GameBoard &game_board,
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

class PiecePointsEvaluator : public EvaluatorInterface<PiecePointsEvaluator> {
public:
  PiecePointsEvaluator(GamePositionPoints_t game_position_points_);
  PiecePointsEvaluator();

  BestMoves ImplementEvaluateNonWinLeaf(
      GameBoard &game_board,
      PieceColor cur_player,
      PieceColor initiating_player
  );

  RatedMove ImplementRateMove(
      Move move,
      GameBoard &game_board,
      PieceColor cur_player
  );

  Points_t GetValueOfPieceAtPosition(
      PieceColor color,
      PieceType piece_type,
      BoardSpace space
  );

  Points_t GetPlayerTotal(PieceColor color, GameBoard &game_board);

private:
  GamePositionPoints_t game_position_points_;
};

#endif