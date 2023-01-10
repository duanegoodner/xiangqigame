#ifndef _MINIMAX_EVALUATOR_3_
#define _MINIMAX_EVALUATOR_3_

#include <board_components.hpp>
#include <common.hpp>
#include <move_selector.hpp>

using namespace board_components;

template <typename ConcreteGameBoard>
class SpaceInfoProvider {
public:
  
  vector<BoardSpace> GetAllSpacesOccupiedBy(PieceColor color) {
    return static_cast<ConcreteGameBoard *>(this)
        ->ImplementGetAllSpacesOccupiedBy(color);
  }

  PieceColor GetColor(BoardSpace space) {
    return static_cast<ConcreteGameBoard *>(this)->ImplementGetColor(space);
  }

  PieceType GetType(BoardSpace space) {
    return static_cast<ConcreteGameBoard *>(this)->ImplementGetType(space);
  }
};

template <typename ConcreteGameBoard>
class PiecePointsEvaluator : public Evaluator<
                                 PiecePointsEvaluator<ConcreteGameBoard>,
                                 ConcreteGameBoard> {
public:
  PiecePointsEvaluator(GamePositionPoints_t game_position_points_);
  PiecePointsEvaluator();

  BestMoves ImplementEvaluateNonWinLeaf(
      ConcreteGameBoard &game_board,
      PieceColor cur_player,
      PieceColor initiating_player
  );

  RatedMove ImplementRateMove(
      Move move,
      ConcreteGameBoard &game_board,
      PieceColor cur_player
  );

  Points_t GetValueOfPieceAtPosition(
      PieceColor color,
      PieceType piece_type,
      BoardSpace space
  );

  Points_t GetPlayerTotal(PieceColor color, ConcreteGameBoard &game_board);

private:
  GamePositionPoints_t game_position_points_;
};

#include <minimax_evaluator.tpp>

#endif