#include <iostream>
#include <limits>
#include <move_selector.hpp>
#include <piece_points.hpp>
#include <board_components.hpp>


using namespace board_components;
using namespace std;
using namespace piece_points;

Move RandomMoveSelector::ImplementSelectMove(
    GameBoard &game_board,
    PieceColor cur_player
    // MoveCollection &cur_moves
    )
{
    auto cur_moves = game_board.CalcFinalMovesOf(cur_player);
    auto selected_move_index = utility_functs::random(
        (size_t)0, cur_moves.moves.size() - 1);
    return cur_moves.moves[selected_move_index];
}