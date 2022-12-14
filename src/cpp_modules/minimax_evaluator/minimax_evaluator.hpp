#ifndef _MINIMAX_EVALUATOR_
#define _MINIMAX_EVALUATOR_

#include <utility>
#include <vector>
#include "game_board.hpp"
#include "piece_points_builder.hpp"
#include "board_components.hpp"


using namespace std;
using namespace board_components;


struct BestMoves {
    int best_eval;
    MoveCollection &best_moves;
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

    template<typename G, typename P, typename PM>    
    BestMoves EvaluateLeaf(
        G &&game_board,
        P &&cur_player,
        PM &&cur_player_moves,
        P &&initiating_player) {
            return static_cast<E*>(this)->ImplementEvaluateLeaf(
                std::forward<G>(game_board),
                std::forward<P>(cur_player),
                std::forward<PM>(cur_player_moves),
                std::forward<P>(initiating_player));
        };

    template<typename M, typename G, typename P>
    RatedMove RateMove(
        M &&move,
        G &&game_board,
        P &&cur_player) {
            return static_cast<E*>(this)->ImplementRateMove(
                std::forward<M>(move), 
                std::forward<G>(game_board),
                std::forward<P>(cur_player));
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

