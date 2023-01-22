#ifndef _MINIMAX_EVALUATOR_TEMPLATE_
#define _MINIMAX_EVALUATOR_TEMPLATE_

#include "common.hpp"
#include <board_components.hpp>
#include <limits>
// #include <piece_points.hpp>

using namespace board_components;

template <typename ConcreteGameBoard, typename ConcreteGamePoints>
PiecePointsEvaluator<ConcreteGameBoard, ConcreteGamePoints>::
    PiecePointsEvaluator(ConcreteGamePoints game_position_points)
    : game_position_points_{game_position_points} {};

// TODO fix code smell. Constructor used for game_position_points_ currently
// not part of interface,
template <typename ConcreteGameBoard, typename ConcreteGamePoints>
PiecePointsEvaluator<ConcreteGameBoard, ConcreteGamePoints>::
    PiecePointsEvaluator()
    // : game_position_points_{ConcreteGamePoints(DEFAULT_GAME_POINTS_ARRAY)} {};
    : game_position_points_{ConcreteGamePoints()} {};

template <typename ConcreteGameBoard, typename ConcreteGamePoints>
Points_t PiecePointsEvaluator<ConcreteGameBoard, ConcreteGamePoints>::
    GetValueOfPieceAtPosition(
        PieceColor color,
        PieceType piece_type,
        BoardSpace space
    ) {
  return game_position_points_
      .GetValueOfPieceAtPosition(color, piece_type, space);
}

template <typename ConcreteGameBoard, typename ConcreteGamePoints>
Points_t PiecePointsEvaluator<ConcreteGameBoard, ConcreteGamePoints>::
    GetPlayerTotal(PieceColor color, ConcreteGameBoard &game_board) {
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

template <typename ConcreteGameBoard, typename ConcreteGamePoints>
BestMoves PiecePointsEvaluator<ConcreteGameBoard, ConcreteGamePoints>::
    ImplementEvaluateNonWinLeaf(
        ConcreteGameBoard &game_board,
        PieceColor cur_player,
        PieceColor initiating_player
    ) {
  auto cur_player_points = GetPlayerTotal(cur_player, game_board);
  auto opponent_points = GetPlayerTotal(opponent_of(cur_player), game_board);

  auto empty_move_collection = MoveCollection();

  if (cur_player == initiating_player) {
    return BestMoves{
        (cur_player_points - opponent_points),
        empty_move_collection};
  } else {
    return BestMoves{
        (opponent_points - cur_player_points),
        empty_move_collection};
  }
}

template <typename ConcreteGameBoard, typename ConcreteGamePoints>
RatedMove
PiecePointsEvaluator<ConcreteGameBoard, ConcreteGamePoints>::ImplementRateMove(
    Move move,
    ConcreteGameBoard &game_board,
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

#endif /* MINIMAX_EVALUATOR */
