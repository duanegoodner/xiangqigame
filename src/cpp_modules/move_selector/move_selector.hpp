#ifndef _MOVE_SELECTOR_
#define _MOVE_SELECTOR_


#include <utility>
#include <random>
#include <vector>
#include "game_board.hpp"
#include "shared_components.hpp"
#include "minimax_evaluator.hpp"

using namespace std;
using namespace board_components;

template <typename S>
class MoveSelector
{
public:
    template<typename G, typename P, typename PM>
    Move SelectMove(
        G &&game_board,
        P (&&cur_player),
        PM (&&cur_moves)) {
            return static_cast<S*>(this)->ImplementSelectMove(
                std::forward<G>(game_board),
                std::forward<P>(cur_player),
                std::forward<PM>(cur_moves));
        }
};


class RandomMoveSelector : public MoveSelector<RandomMoveSelector> {
    public:
    mt19937 engine;
    RandomMoveSelector();   
    
    Move ImplementSelectMove(
        GameBoard game_board,
        PieceColor cur_player,
        vector<Move> cur_moves);
};

template<typename E>
class MinimaxMoveSelector : public MoveSelector<MinimaxMoveSelector<E>> {
    public:
    E evaluator_;
    int search_depth_;
    int node_counter_;
    mt19937 engine;

    
    MinimaxMoveSelector(
        E evaluator, int search_depth);
    Move ImplementSelectMove(
        GameBoard game_board,
        PieceColor cur_player,
        vector<Move> cur_moves);
    void ResetNodeCounter();
    vector<RatedMove> GenerateRankedMoveList(
        GameBoard game_board,
        PieceColor cur_player,
        vector<Move> cur_player_moves);
    BestMoves MinimaxRec(
        GameBoard game_board,
        int search_depth,
        int alpha,
        int beta,
        PieceColor cur_player,
        PieceColor initiating_player);
};

#endif // _MOVE_SELECTOR_
