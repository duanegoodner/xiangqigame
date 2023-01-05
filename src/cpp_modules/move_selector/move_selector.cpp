#include <board_components.hpp>
#include <iostream>
#include <limits>
#include <move_selector.hpp>
#include <piece_points.hpp>

using namespace board_components;
using namespace std;
using namespace piece_points;

Move RandomMoveSelector::ImplementSelectMove(
    GameBoard &game_board,
    PieceColor cur_player
    // MoveCollection &cur_moves
) {
  auto cur_moves = game_board.CalcFinalMovesOf(cur_player);
  auto selected_move_index =
      utility_functs::random((size_t)0, cur_moves.moves.size() - 1);
  return cur_moves.moves[selected_move_index];
}

// PiecePointsMoveSelector::PiecePointsMoveSelector() {
//   this->evaluator_ = PiecePointsEvaluator(DEFAULT_GAME_POINTS);
//     this->search_depth_ = 3;
//     this->node_counter_ = 0;
// }

// PiecePointsMoveSelector::PiecePointsMoveSelector(int search_depth) {
//     this->evaluator_ = PiecePointsEvaluator(DEFAULT_GAME_POINTS);
//     this->search_depth_ = search_depth;
//     this->node_counter_ = 0;
//   }

// PiecePointsMoveSelector::PiecePointsMoveSelector(PiecePointsEvaluator evaluator, int search_depth) {
//     this->evaluator_ = evaluator;
//     this->search_depth_ = search_depth;
//     this->node_counter_ = 0;
//   }