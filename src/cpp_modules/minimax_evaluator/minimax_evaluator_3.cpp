#include <limits>
#include "minimax_evaluator_3.hpp"

using namespace board_components;
using namespace piece_points;

BestMoves evaluate_winner(
    PieceColor cur_player,
    PieceColor initiating_player)
{
    auto empty_best_moves = MoveCollection();

    if (cur_player == initiating_player)
    {
        return BestMoves{
            numeric_limits<int>::max(),
            empty_best_moves};
    }
    else
    {
        return BestMoves{
            numeric_limits<int>::min(),
            empty_best_moves};
    }
}

PiecePointsEvaluator::PiecePointsEvaluator(GamePositionPoints_t game_position_points)
    : game_position_points_{game_position_points} {};

PiecePointsEvaluator::PiecePointsEvaluator()
    : game_position_points_{DEFAULT_GAME_POINTS} {};

Points_t PiecePointsEvaluator::GetValueOfPieceAtPosition(
    PieceColor color, PieceType piece_type, BoardSpace space)
{
    return game_position_points_[color][piece_type][space.rank][space.file];
}

Points_t PiecePointsEvaluator::GetPlayerTotal(PieceColor color, GameBoard &game_board)
{
    int pre_attack_total = 0;
    for (auto space : game_board.GetAllSpacesOccupiedBy(color))
    {
        auto piece_type = game_board.GetType(space);
        pre_attack_total += GetValueOfPieceAtPosition(color, piece_type, space);
    }
    return pre_attack_total;
}

BestMoves PiecePointsEvaluator::ImplementEvaluateLeaf(
    GameBoard &game_board,
    PieceColor cur_player,
    MoveCollection &cur_player_moves,
    PieceColor initiating_player)
{
    auto cur_player_points = GetPlayerTotal(cur_player, game_board);
    auto opponent_points = GetPlayerTotal(
        opponent_of(cur_player), game_board);

    auto empty_move_collection = MoveCollection();

    if (cur_player == initiating_player)
    {
        return BestMoves{
            (cur_player_points - opponent_points),
            empty_move_collection};
    }
    else
    {
        return BestMoves{
            (opponent_points - cur_player_points),
            empty_move_collection};
    }
}

RatedMove PiecePointsEvaluator::ImplementRateMove(
    Move move, GameBoard &game_board, PieceColor cur_player)
{
    auto piece_type = game_board.GetType(move.start);
    auto cur_player_position_array = game_position_points_[cur_player][piece_type];
    auto position_value_delta = (cur_player_position_array[move.end.rank][move.end.file] -
                                 cur_player_position_array[move.start.rank][move.start.file]);

    int capture_val;

    if (game_board.GetColor(move.end) == opponent_of(cur_player))
    {
        auto captured_piece_type = game_board.GetType(move.end);
        auto opponent_position_array = game_position_points_[opponent_of(cur_player)][captured_piece_type];
        capture_val = opponent_position_array[move.end.rank][move.end.file];
    }
    else
    {
        capture_val = 0;
    }

    return RatedMove{move, (position_value_delta + capture_val)};
}