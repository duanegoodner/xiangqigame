#ifndef _MINIMAX_EVALUATOR_3_
#define _MINIMAX_EVAULATOR_3_

#include "board_components.hpp"
#include "game_board.hpp"
#include "piece_points.hpp"

using namespace board_components;
using namespace piece_points;
    
    struct BestMoves
    {
        Points_t best_eval;
        MoveCollection best_moves;
    };

    struct RatedMove
    {
        Move move;
        Points_t rating;
    };

    BestMoves evaluate_winner(
        PieceColor cur_player,
        PieceColor initiating_player);

    template <typename ImplementedEvaluator>
    class EvaluatorInterface
    {
    public:
        RatedMove RateMove(
            Move move,
            GameBoard &game_board,
            PieceColor cur_player)
        {
            return static_cast<ImplementedEvaluator *>(this)->ImplementRateMove(
                move, game_board, cur_player);
        }

        BestMoves EvaluateLeaf(
            GameBoard &game_board,
            PieceColor cur_player,
            MoveCollection &cur_player_moves,
            PieceColor initiating_player)
        {
            return static_cast<EvaluatorInterface *>(this)->ImplementEvaluateLeaf(
                game_board,
                cur_player,
                cur_player_moves,
                initiating_player);
        }
    };


    class PiecePointsEvaluator : public EvaluatorInterface<PiecePointsEvaluator>
    {
    public:
        PiecePointsEvaluator(GamePositionPoints_t game_position_points_);
        PiecePointsEvaluator();


        BestMoves ImplementEvaluateLeaf(
            GameBoard &game_board,
            PieceColor cur_player,
            MoveCollection &cur_player_moves,
            PieceColor initiating_player);

        RatedMove ImplementRateMove(
            Move move,
            GameBoard &game_board,
            PieceColor cur_player);

        Points_t GetValueOfPieceAtPosition(
            PieceColor color,
            PieceType piece_type,
            BoardSpace space);

        Points_t GetPlayerTotal(PieceColor color, GameBoard &game_board);

    private:
        GamePositionPoints_t game_position_points_;
    };

#endif