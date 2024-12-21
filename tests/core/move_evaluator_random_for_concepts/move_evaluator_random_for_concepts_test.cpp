#include <concepts/move_evaluator.hpp>
#include <gameboard/game_board_for_concepts.hpp>
#include <gtest/gtest.h>
#include <gameboard/move_data_structs.hpp>
#include <moveselection/move_evaluator_random_for_concepts.hpp>

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

  gameboard::MoveCollection dummy_move_collection_{};
  gameboard::Move move_a{{0, 0}, {0, 1}};
  gameboard::Move move_b{{1, 0}, {1, 1}};
  gameboard::Move move_c{{2, 0}, {2, 1}};

  std::set<gameboard::Move, bool (*)(const gameboard::Move &, const gameboard::Move &)>
      selected_move_set_{moveComparator};
};

TEST_F(RandomEvaluatorConceptTest, CompliesWithMoveEvaluatorConcept) {
  static_assert(
      MoveEvaluatorConcept<moveselection::RandomMoveEvaluatorForConcepts>,
      "RandomMoveEvaluatorForConcepts must comply with MoveEvaluatorConcept."
  );
}

TEST_F(RandomEvaluatorConceptTest, BuildFromStaticFactoryMethod) {
  auto red_evaluator =
      moveselection::RandomMoveEvaluatorForConcepts::Create(gameboard::PieceColor::kRed);
}

TEST_F(RandomEvaluatorConceptTest, BuildFromFactoryClass) {
  auto red_evaluator = move_evaluator_factory_.Create(gameboard::PieceColor::kRed);
}

TEST_F(RandomEvaluatorConceptTest, SelectsMoveIndexWithinAllowedRange) {

  size_t num_selections = 100;

  dummy_move_collection_.Append(move_a);
  dummy_move_collection_.Append(move_b);
  dummy_move_collection_.Append(move_c);

  auto red_evaluator =
      moveselection::RandomMoveEvaluatorForConcepts::Create(gameboard::PieceColor::kRed);

  for (auto idx = 0; idx < num_selections; ++idx) {
    auto selected_move = red_evaluator->SelectMove(dummy_move_collection_);
    EXPECT_TRUE(
        selected_move == move_a || selected_move == move_b || selected_move == move_c
    );
    selected_move_set_.insert(selected_move);
  }
  EXPECT_TRUE(selected_move_set_.size() == 3);
}

// Red selects starting move 10 times. If choice is random, we can be
// almost certain that the number of unique selected Moves will be > 5.
TEST_F(RandomEvaluatorConceptTest, TestStartingMoveSelection) {

  int num_first_move_selections = 10;

  auto red_evaluator = move_evaluator_factory_.Create(gameboard::PieceColor::kRed);

  auto allowed_moves = game_board_->CalcFinalMovesOf(gameboard::PieceColor::kRed);

  for (auto idx = 0; idx < num_first_move_selections; idx++) {
    auto red_selected_move = red_evaluator->SelectMove(allowed_moves);
    selected_move_set_.insert(red_selected_move);
  }
  EXPECT_TRUE(selected_move_set_.size() > 5);
}