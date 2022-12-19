#ifndef _MOVE_SELECTOR_
#define _MOVE_SELECTOR_


#include <utility>
#include <random>
#include <vector>
#include "game_board.hpp"
#include "board_components.hpp"
#include "minimax_evaluator.hpp"

using namespace std;
using namespace board_components;

template <class S>
class MoveSelector
{
public:
    // template<typename G, typename P, typename PM>
    Move SelectMove(
        GameBoard &game_board,
        PieceColor cur_player,
        MoveCollection &cur_moves) {
            return static_cast<S*>(this)->ImplementSelectMove(
                game_board, cur_player, cur_moves);
        }
};


class RandomMoveSelector : public MoveSelector<RandomMoveSelector> {
    
    public:
    Move ImplementSelectMove(
        GameBoard &game_board,
        PieceColor cur_player,
        MoveCollection &cur_moves);
};

template <class E>
class MinimaxMoveSelector : public MoveSelector<MinimaxMoveSelector<E>> {
    public:
    E evaluator_;
    int search_depth_;
    int node_counter_;
    // mt19937 engine;

    
    MinimaxMoveSelector(
        E evaluator, int search_depth);
    MinimaxMoveSelector(int search_depth);
    Move ImplementSelectMove(
        GameBoard &game_board,
        PieceColor cur_player,
        MoveCollection &cur_moves);
    void ResetNodeCounter();
    vector<RatedMove> GenerateRankedMoveList(
        GameBoard &game_board,
        PieceColor cur_player,
        MoveCollection &cur_player_moves);
    BestMoves MinimaxRec(
        GameBoard &game_board,
        int search_depth,
        int alpha,
        int beta,
        PieceColor cur_player,
        PieceColor initiating_player);
};

class PiecePointsMinimaxMoveSelector : public MinimaxMoveSelector<PiecePointsEvaluator> {


};

#endif // _MOVE_SELECTOR_
