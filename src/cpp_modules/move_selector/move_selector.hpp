#include <vector>
#include "game_board.hpp"
#include "shared_components.hpp"
#include "minimax_evaluator.hpp"

using namespace std;
using namespace board_components;

template <typename T>
class MoveSelector
{
public:
    Move SelectMove(
        const BoardMap_t& board_map,
        PieceColor cur_player,
        vector<Move> cur_moves) {
            static_cast<T*>(this)->DoSelectMove(
                board_map, cur_player, cur_moves);
        }
};


class RandomMoveSelector : public MoveSelector<RandomMoveSelector> {
    public:
    Move DoSelectMove(
        const BoardMap_t& board_map,
        PieceColor cur_player,
        vector<Move> cur_moves);
};

class MinimaxMoveSelector : public MoveSelector<MinimaxMoveSelector> {
    public:
    MinimaxMoveSelector(
        MinimaxEvaluator evaluator, int search_depth
    );
    Move DoSelectMove(
        const BoardMap_t& board_map,
        PieceColor cur_player,
        vector<Move> cur_moves
    );
};


