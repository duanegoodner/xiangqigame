#include <game.hpp>
#include <game_board_for_concepts.hpp>
#include <gtest/gtest.h>
#include <memory>
#include <move_evaluator_minimax_for_concepts.hpp>
#include <piece_position_points_for_concepts.hpp>
#include <vector>
#include <zobrist_for_concepts.hpp>

class GameTestSuiteTwoAIBase {
public:
  virtual ~GameTestSuiteTwoAIBase() = default;
};

template <
    typename RedKeyType,
    typename BlackKeyType,
    size_t RedNumConfKeys,
    size_t BlackNumConfKeys>
class GameTestSuiteTwoAI : public GameTestSuiteTwoAIBase {
  using BlackZCalcType = boardstate::ZobristCalculatorForConcepts<BlackKeyType>;
  using RedZCalcType = boardstate::ZobristCalculatorForConcepts<RedKeyType>;

  using BlackZComponentType =
      boardstate::ZobristComponentForConcepts<BlackZCalcType, BlackNumConfKeys>;
  using RedZComponentType =
      boardstate::ZobristComponentForConcepts<RedZCalcType, RedNumConfKeys>;

  using BlackZCoordinatorType =
      boardstate::ZobristCoordinatorForConcepts<BlackZComponentType>;
  using RedZCoordinatorType =
      boardstate::ZobristCoordinatorForConcepts<RedZComponentType>;

  using BlackEvaluatorType = moveselection::MinimaxMoveEvaluatorForConcepts<
      BlackZCoordinatorType,
      gameboard::GameBoardForConcepts,
      piecepoints::PiecePositionPointsForConcepts>;
  using RedEvaluatorType = moveselection::MinimaxMoveEvaluatorForConcepts<
      RedZCoordinatorType,
      gameboard::GameBoardForConcepts,
      piecepoints::PiecePositionPointsForConcepts>;

  using GameType =
      game::Game<gameboard::GameBoardForConcepts, RedEvaluatorType, BlackEvaluatorType>;
};

class GameTestTwoAI : public ::testing::Test {
protected:
  std::vector<std::shared_ptr<GameTestSuiteTwoAIBase>> test_suites_;

  GameTestTwoAI() {
    test_suites_.emplace_back(
        std::make_shared<GameTestSuiteTwoAI<uint64_t, uint64_t, 1, 1>>()
    );
  }
};

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}