#ifndef _MINIMAX_EVALUATOR_3_
#define _MINIMAX_EVALUATOR_3_

#include <board_components.hpp>
#include <common.hpp>
#include <game_board.hpp>
#include <move_selector.hpp>

using namespace board_components;

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