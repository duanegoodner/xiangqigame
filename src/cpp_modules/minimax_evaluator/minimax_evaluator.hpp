#ifndef _MINIMAX_EVALUATOR_
#define _MINIMAX_EVALUATOR_

#include <utility>
#include <vector>
#include "game_board.hpp"
#include "piece_points.hpp"
#include "board_components.hpp"


using namespace std;
using namespace board_components;
using namespace piece_points;

struct BestMoves {
    int best_eval;
    MoveCollection best_moves;
};

struct RatedMove {
    Move move;
    int rating;
};

BestMoves evaluate_winner(
    PieceColor cur_player,
    PieceColor initiating_player);

template <typename E>
class MinimaxEvaluator {
    public:

    // template<typename G, typename P, typename PM>    
    BestMoves EvaluateLeaf(
        GameBoard &game_board,
        PieceColor cur_player,
        MoveCollection &cur_player_moves,
        PieceColor initiating_player) {
            return static_cast<E*>(this)->ImplementEvaluateLeaf(
                game_board,
                cur_player,
                cur_player_moves,
                initiating_player);
        };

    // template<typename M, typename G, typename P>
    RatedMove RateMove(
        Move move,
        GameBoard &game_board,
        PieceColor cur_player) {
            return static_cast<E*>(this)->ImplementRateMove(
                move, game_board, cur_player);
        };
    // GenerateRankedMoveList not included. Will make it
    // a method of MinimaxMoveSelector (this way, avoid mixing
    // interface and implementation)
};

class PiecePointsEvaluator : public MinimaxEvaluator<PiecePointsEvaluator> {
    public:
    PiecePointsEvaluator(GamePositionPoints_t game_position_points);

    BestMoves ImplementEvaluateLeaf(
        GameBoard &game_board,
        PieceColor cur_player,
        MoveCollection &cur_player_moves,
        PieceColor initiating_player);
    
    RatedMove ImplementRateMove(
        Move move,
        GameBoard &game_board,
        PieceColor cur_player);
    
    int GetValueOfPieceAtPosition(
        PieceColor color,
        PieceType piece_type,
        BoardSpace space);
    
    int GetPlayerTotal(PieceColor color, GameBoard &game_board);

    private:
    GamePositionPoints_t game_position_points_;
};

#endif // _MINIMAX_EVALUATOR_

