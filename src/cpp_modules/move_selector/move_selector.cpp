#include <limits>
#include "move_selector.hpp"
#include "shared_components.hpp"

using namespace board_components;
using namespace std;

RandomMoveSelector::RandomMoveSelector()
{
    mt19937 engine(time(nullptr));
}

Move RandomMoveSelector::ImplementSelectMove(
    GameBoard game_board,
    PieceColor cur_player,
    vector<Move> cur_moves)
{
    uniform_int_distribution<int> dist(0, cur_moves.size() - 1);
    return cur_moves[dist(engine)];
    // return cur_moves[0];
}

template<typename E>
MinimaxMoveSelector<E>::MinimaxMoveSelector(
    E evaluator, int search_depth)
    : evaluator_{evaluator}, search_depth_{search_depth}, node_counter_{0} {};

template <typename E>
void MinimaxMoveSelector<E>::ResetNodeCounter()
{
    node_counter_ = 0;
}

template <typename E>
vector<RatedMove> MinimaxMoveSelector<E>::GenerateRankedMoveList(
    GameBoard game_board,
    PieceColor cur_player,
    vector<Move> cur_player_moves)
{
    vector<RatedMove> rated_moves;
    for (auto cur_move : cur_player_moves)
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
}

template <typename E>
BestMoves MinimaxMoveSelector<E>::MinimaxRec(
    GameBoard game_board,
    int search_depth,
    int alpha,
    int beta,
    PieceColor cur_player,
    PieceColor initiating_player)
{
    node_counter_ += 1;
    auto cur_moves = game_board.CalcFinalMovesOf(cur_player);
    if (cur_moves.size() == 0)
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
        vector<Move> best_moves;
        auto ranked_moves = GenerateRankedMoveList(
            game_board, cur_player, cur_moves);
        for (auto rated_move : ranked_moves)
        {
            auto executed_move = game_board.ExecuteMove(rated_move.move);
            auto cur_eval = this->MinimaxRec(
                                    game_board,
                                    search_depth - 1,
                                    alpha,
                                    beta,
                                    opponent_of(initiating_player),
                                    initiating_player)
                                .best_eval;
            if (cur_eval == max_eval)
            {
                best_moves.emplace_back(rated_move.move);
            }
            else if (cur_eval > max_eval)
            {
                max_eval = cur_eval;
                best_moves.clear();
                best_moves.emplace_back(rated_move.move);
            }
            game_board.UndoMove(executed_move);
            alpha = max(alpha, cur_eval);
            if (beta < alpha)
            {
                break;
            }
        }
        return BestMoves(max_eval, best_moves);
    }
    else
    {
        auto min_eval = numeric_limits<int>::max();
        vector<RatedMove> best_moves;
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
                best_moves.emplace_back(rated_move);
            }
            else if (cur_eval < min_eval)
            {
                {
                    min_eval = cur_eval;
                    best_moves.clear();
                    best_moves.emplace_back(rated_move);
                }
            }
            game_board.UndoMove(executed_move);
            beta = min(beta, cur_eval);
            if (beta <= alpha)
            {
                break;
            }
        }
        return BestMoves(min_eval, best_moves);
    }
}

template <typename E>
Move MinimaxMoveSelector<E>::ImplementSelectMove(
    GameBoard game_board, PieceColor cur_player, vector<Move> cur_moves) {
        ResetNodeCounter();
        auto minimax_result = MinimaxRec(
            game_board,
            search_depth_,
            numeric_limits<int>::min(),
            numeric_limits<int>::max(),
            cur_player,
            cur_player);
        
        uniform_int_distribution<int> dist(0, minimax_result.best_moves.size() - 1);
        return minimax_result.best_moves[dist(engine)];
    }