#include <vector>
#include "board_components.hpp"
#include "game_board.hpp"
#include "minimax_evaluator.hpp"
#include "piece_points.hpp"
#include "utility_functs.hpp"

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
        MoveCollection &cur_moves);
};

template <typename MinimaxEvaluator>
class MinimaxMoveSelectorInterface : public MoveSelectorInterface<MinimaxMoveSelectorInterface<MinimaxEvaluator>>
{
public:
    // These data members need to be public for CRTP to work
    MinimaxEvaluator evaluator_;
    int search_depth_;
    int node_counter_;

    void ResetNodeCounter()
    {
        InternalImplementResetNodeCounter();
    };
    
    vector<RatedMove> GenerateRankedMoveList(
        GameBoard &game_board,
        PieceColor cur_player,
        MoveCollection &cur_player_moves)
    {
        return InternalImplementGenerateRankedMoveList(
            game_board, cur_player, cur_player_moves);
    }

    BestMoves MinimaxRec(
        GameBoard &game_board,
        int search_depth,
        int alpha,
        int beta,
        PieceColor cur_player,
        PieceColor initiating_player)
    {
        return InternalImplementMinimaxRec(
            game_board, search_depth, alpha, beta, cur_player, initiating_player);
    }

    Move ImplementSelectMove(
        GameBoard &game_board, PieceColor cur_player, MoveCollection &cur_moves)
    {
        return InternalImplementSelectMove(game_board, cur_player, cur_moves);
    }

    private:

    void InternalImplementResetNodeCounter()
    {
        node_counter_ = 0;
    };
    
    vector<RatedMove> InternalImplementGenerateRankedMoveList(
        GameBoard &game_board,
        PieceColor cur_player,
        MoveCollection &cur_player_moves)
    {
        vector<RatedMove> rated_moves;
        for (auto cur_move : cur_player_moves.moves)
        {
            auto cur_rated_move = evaluator_.ImplementRateMove(
                cur_move, game_board, cur_player);
            rated_moves.emplace_back(cur_rated_move);
        }
        sort(rated_moves.begin(), rated_moves.end(),
             [](const RatedMove &move_a, const RatedMove &move_b)
             {
                 return move_a.rating > move_b.rating;
             });
        return rated_moves;
    }
    
    Move InternalImplementSelectMove(
        GameBoard &game_board,
        PieceColor cur_player,
        MoveCollection &cur_moves
    ) {
        ResetNodeCounter();
        auto minimax_result = MinimaxRec(
            game_board,
            search_depth_,
            numeric_limits<int>::min(),
            numeric_limits<int>::max(),
            cur_player,
            cur_player);
        auto selected_move_index = utility_functs::random(
            (size_t)0, minimax_result.best_moves.moves.size() - 1);
        uniform_int_distribution<int> dist(0, minimax_result.best_moves.moves.size() - 1);
        return minimax_result.best_moves.moves[selected_move_index];
    }

    BestMoves InternalImplementMinimaxRec(
        GameBoard &game_board,
        int search_depth,
        int alpha,
        int beta,
        PieceColor cur_player,
        PieceColor initiating_player)
    {
        node_counter_ += 1;
        auto cur_moves = game_board.CalcFinalMovesOf(cur_player);
        if (cur_moves.moves.size() == 0)
        {
            return evaluate_winner(cur_player, initiating_player);
        }
        if (search_depth == 0)
        {
            return evaluator_.ImplementEvaluateLeaf(
                game_board, cur_player, cur_moves, initiating_player);
        }
        if (cur_player == initiating_player)
        {
            auto max_eval = numeric_limits<int>::min();
            MoveCollection best_moves;
            auto ranked_moves = GenerateRankedMoveList(
                game_board, cur_player, cur_moves);
            for (auto rated_move : ranked_moves)
            {
                auto executed_move = game_board.ExecuteMove(rated_move.move);
                auto cur_eval = MinimaxRec(
                                    game_board,
                                    search_depth - 1,
                                    alpha,
                                    beta,
                                    opponent_of(initiating_player),
                                    initiating_player)
                                    .best_eval;
                if (cur_eval == max_eval)
                {
                    best_moves.Append(rated_move.move);
                }
                else if (cur_eval > max_eval)
                {
                    max_eval = cur_eval;
                    best_moves.moves.clear();
                    best_moves.Append(rated_move.move);
                }
                game_board.UndoMove(executed_move);
                alpha = max(alpha, cur_eval);
                if (beta <= alpha)
                {
                    break;
                }
            }
            return BestMoves{max_eval, best_moves};
        }
        else
        {
            auto min_eval = numeric_limits<int>::max();
            MoveCollection best_moves;
            auto ranked_moves = GenerateRankedMoveList(
                game_board, cur_player, cur_moves);
            for (auto rated_move : ranked_moves)
            {
                auto executed_move = game_board.ExecuteMove(rated_move.move);
                auto cur_eval = MinimaxRec(
                                    game_board,
                                    search_depth - 1,
                                    alpha,
                                    beta,
                                    initiating_player,
                                    initiating_player)
                                    .best_eval;
                if (cur_eval == min_eval)
                {
                    best_moves.Append(rated_move.move);
                }
                else if (cur_eval < min_eval)
                {
                    {
                        min_eval = cur_eval;
                        best_moves.moves.clear();
                        best_moves.Append(rated_move.move);
                    }
                }
                game_board.UndoMove(executed_move);
                beta = min(beta, cur_eval);
                if (beta <= alpha)
                {
                    break;
                }
            }
            return BestMoves{min_eval, best_moves};
        }
    }
};

class PiecePointsMoveSelector : public MinimaxMoveSelectorInterface<PiecePointsEvaluator> {
    public:
    PiecePointsMoveSelector() {
        this->evaluator_ = PiecePointsEvaluator(DEFAULT_GAME_POINTS);
        this->search_depth_ = 3;
        this->node_counter_ = 0;
    }
    PiecePointsMoveSelector(int search_depth) {
        this->evaluator_ = PiecePointsEvaluator(DEFAULT_GAME_POINTS);
        this->search_depth_ = search_depth;
        this->node_counter_ = 0;
    }
    PiecePointsMoveSelector(
        PiecePointsEvaluator evaluator,
        int search_depth) {
            this->evaluator_ = evaluator;
            this->search_depth_ = search_depth;
            this->node_counter_ = 0;
        }
};
