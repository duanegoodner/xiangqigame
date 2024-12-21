
#include <moveselection/move_evaluator_minimax_for_concepts.hpp>

namespace moveselection {
namespace minimaxutils_forconcepts {
bool ValidateMove(SearchSummary &search_summary, const MoveCollection &allowed_moves) {
  bool is_selected_move_allowed =
      allowed_moves.ContainsMove(search_summary.selected_move());
  if (!is_selected_move_allowed) {
    search_summary.set_returned_illegal_move(true);
  }
  return is_selected_move_allowed;
}
} // namespace minimaxutils_forconcepts
} // namespace moveselection