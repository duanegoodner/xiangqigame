#ifndef _MOVE_SELECTOR_HELPERS_
#define _MOVE_SELECTOR_HELPERS_

#include <common.hpp>
#include <board_components.hpp>

using namespace board_components;

// struct BestMoves {
//   Points_t best_eval;
//   MoveCollection best_moves;
// };

// struct RatedMove {
//   Move move;
//   Points_t rating;
// };

// inline BestMoves evaluate_win_leaf(
//     PieceColor cur_player,
//     PieceColor initiating_player
// ) {
//   auto empty_best_moves = MoveCollection();

//   if (cur_player == initiating_player) {
//     return BestMoves{numeric_limits<Points_t>::min(), empty_best_moves};
//   } else {
//     return BestMoves{numeric_limits<Points_t>::max(), empty_best_moves};
//   }
// }

#endif