
#include <limits>
#include <game_board.hpp>
#include <minimax_evaluator.hpp>

using namespace board_components;
using namespace piece_points;

template <typename ConcreteGameBoard>
PiecePointsEvaluator<ConcreteGameBoard>::PiecePointsEvaluator(
    GamePositionPoints_t game_position_points
)
    : game_position_points_{game_position_points} {};

template <typename ConcreteGameBoard>
PiecePointsEvaluator<ConcreteGameBoard>::PiecePointsEvaluator()
    : game_position_points_{DEFAULT_GAME_POINTS} {};

template <typename ConcreteGameBoard>
Points_t PiecePointsEvaluator<ConcreteGameBoard>::GetValueOfPieceAtPosition(
    PieceColor color,
    PieceType piece_type,
    BoardSpace space
) {
  return game_position_points_[color][piece_type][space.rank][space.file];
}

template <typename ConcreteGameBoard>
Points_t PiecePointsEvaluator<ConcreteGameBoard>::GetPlayerTotal(
    PieceColor color,
    ConcreteGameBoard &game_board
) {
  Points_t pre_attack_total = 0;
  for (auto space : game_board.GetAllSpacesOccupiedBy(color)) {
    auto piece_type = game_board.GetType(space);
    pre_attack_total += GetValueOfPieceAtPosition(color, piece_type, space);
  }
  return pre_attack_total;
}

template <typename ConcreteGameBoard>
BestMoves PiecePointsEvaluator<ConcreteGameBoard>::ImplementEvaluateNonWinLeaf(
    ConcreteGameBoard &game_board,
    PieceColor cur_player,
    // MoveCollection &cur_player_moves,
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

template <typename ConcreteGameBoard>
RatedMove PiecePointsEvaluator<ConcreteGameBoard>::ImplementRateMove(
    Move move,
    ConcreteGameBoard &game_board,
    PieceColor cur_player
) {
  auto piece_type = game_board.GetType(move.start);
  auto cur_player_position_array =
      game_position_points_[cur_player][piece_type];
  auto position_value_delta =
      (cur_player_position_array[move.end.rank][move.end.file] -
       cur_player_position_array[move.start.rank][move.start.file]);

  Points_t capture_val;

  if (game_board.GetColor(move.end) == opponent_of(cur_player)) {
    auto captured_piece_type = game_board.GetType(move.end);
    auto opponent_position_array =
        game_position_points_[opponent_of(cur_player)][captured_piece_type];
    capture_val = opponent_position_array[move.end.rank][move.end.file];
  } else {
    capture_val = 0;
  }

  return RatedMove{move, (position_value_delta + capture_val)};
}

template class PiecePointsEvaluator<GameBoard>;