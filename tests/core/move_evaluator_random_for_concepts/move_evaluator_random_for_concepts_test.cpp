#include <concept_move_evaluator.hpp>
#include <game_board_for_concepts.hpp>
#include <gtest/gtest.h>
#include <move_data_structs.hpp>
#include <move_evaluator_random_for_concepts.hpp>

class RandomEvaluatorConceptTest : public ::testing::Test {
protected:
  static bool moveComparator(const gameboard::Move &lhs, const gameboard::Move &rhs) {
    if (lhs.start.rank != rhs.start.rank)
      return lhs.start.rank < rhs.start.rank;
    if (lhs.start.file != rhs.start.file)
      return lhs.start.file < rhs.start.file;
    if (lhs.end.rank != rhs.end.rank)
      return lhs.end.rank < rhs.end.rank;
    return lhs.end.file < rhs.end.file;
  }

  std::shared_ptr<gameboard::GameBoardForConcepts> game_board_ =
      gameboard::GameBoardForConcepts::Create();

  moveselection::RandomMoveEvaluatorFactory move_evaluator_factory_;
};

TEST_F(RandomEvaluatorConceptTest, CompliesWithMoveEvaluatorConcept) {
  static_assert(
      MoveEvaluatorConcept<moveselection::RandomMoveEvaluatorForConcepts>,
      "RandomMoveEvaluatorForConcepts must comply with MoveEvaluatorConcept."
  );
}

TEST_F(RandomEvaluatorConceptTest, BuildRandomMoveEvaluator) {
  auto red_evaluator = move_evaluator_factory_.Create(gameboard::PieceColor::kRed);
}

// Red selects starting move 10 times. If choice is random, we can be
// almost certain that the number of unique selected Moves will be > 5.
TEST_F(RandomEvaluatorConceptTest, TestStartingMoveSelection) {

  int num_first_move_selections = 10;
  std::set<Move, bool (*)(const Move &, const Move &)> move_set(moveComparator);

  auto red_evaluator = move_evaluator_factory_.Create(gameboard::PieceColor::kRed);

  auto allowed_moves = game_board_->CalcFinalMovesOf(gameboard::PieceColor::kRed);

  for (auto idx = 0; idx < num_first_move_selections; idx++) {
    auto red_selected_move = red_evaluator->SelectMove(allowed_moves);
    move_set.insert(red_selected_move);
  }
  EXPECT_TRUE(move_set.size() > 5);
}