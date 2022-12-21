#include "board_components.hpp"
#include "game_board.hpp"
#include "minimax_evaluator_2.hpp"

using namespace minimax_evaluator;

namespace move_selector
{
    template <typename ImplementedMoveSelector>
    class MoveSelectorInterface
    {
    public:
        Move SelectMove(
            GameBoard &game_board,
            PieceColor cur_player,
            MoveCollection &cur_moves)
        {
            return static_cast<ImplementedMoveSelector *>(this)->ImplementSelectMove(
                game_board, cur_player, cur_moves);
        }
    };

    class RandomMoveSelector : public MoveSelectorInterface<RandomMoveSelector>
    {
        public:
        Move ImplementSelectMove(
            GameBoard &game_board,
            PieceColor cur_player,
            MoveCollection &cur_moves
        );
    };

    template <typename MinimaxEvaluator>
    class MinimaxMoveSelectorInterface : public MoveSelectorInterface<MinimaxMoveSelectorInterface<MinimaxEvaluator>>
    {
    public:
        MinimaxEvaluator evaluator_;
        int search_depth_;
        int node_counter_;

        MinimaxMoveSelectorInterface<MinimaxEvaluator>(
            MinimaxEvaluator evaluator, int search_depth);
        MinimaxMoveSelectorInterface<MinimaxEvaluator>(int search_depth);        
        void ResetNodeCounter();
        Move ImplementSelectMove(
            GameBoard &game_board,
            PieceColor cur_player,
            MoveCollection &cur_moves);
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
   
}
