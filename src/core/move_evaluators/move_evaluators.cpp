//! @file move_evaluators.cpp
//! Linkage point for .hpp and .tpp files where template-heavy concrete MoveEvaluators
//! are implemented.
#include <moveselection/move_evaluators.hpp>

namespace moveselection {
namespace minimaxutils {

bool ValidateMove(SearchSummary &search_summary, const MoveCollection &allowed_moves) {
  bool is_selected_move_allowed =
      allowed_moves.ContainsMove(search_summary.selected_move());
  if (!is_selected_move_allowed) {
    search_summary.set_returned_illegal_move(true);
  }
  return is_selected_move_allowed;
}

} // namespace minimaxutils
} // namespace moveselection