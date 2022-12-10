#include <utility>
#include <vector>
#include "piece_points.hpp"
#include "shared_components.hpp"



using namespace std;
using namespace board_components;


struct BestMoves {
    int best_eval;
    vector<Move> best_moves;
};

struct RatedMove {
    Move move;
    int rating;
};

BestMoves evaluate_winner(
    PieceColor cur_player,
    PieceColor initiating_player);

template <typename D>
class MinimaxEvaluator {
    public:

    template<typename G, typename P, typename V>    
    BestMoves EvaluateLeaf(
        G &&game_board,
        P &&cur_player,
        V &&cur_player_moves,
        P &&initiating_player) {
            static_cast<D*>(this)->ImplementEvaluateLeaf(
                std::forward<G>(game_board),
                std::forward<P>(cur_player),
                std::forward<V>(cur_player_moves),
                std::forward<P>(initiating_player));
        };

    template<typename M, typename G, typename P>
    RatedMove RateMove(
        M &&move,
        G &&game_board,
        P &&cur_player) {
            static_cast<D*>(this)->ImplementRateMove(
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
    PiecePointsEvaluator(PiecePoints piece_points);

    BestMoves ImplementEvaluateLeaf(
        GameBoard game_board,
        PieceColor cur_player,
        vector<Move> cur_player_moves,
        PieceColor initiating_player);
    
    RatedMove ImplementRatedMove(
        Move move,
        GameBoard game_board,
        PieceColor cur_player);
    
    int GetValueOfPieceAtPosition(
        PieceColor color,
        PieceType piece_type,
        BoardSpace space);
    
    int GetPlayerTotal(PieceColor color, GameBoard game_board);

    private:
    PiecePoints piece_points_;


};

