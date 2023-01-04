#include <gtest/gtest.h>
#include <minimax_evaluator.hpp>
#include <piece_points.hpp>

using namespace piece_points;

class PiecePointsEvaluatorTest : public ::testing::Test {

protected:
  PiecePointsBuilder piece_points_buider_;
  GameBoard game_board_;
  PiecePointsEvaluatorTest()
      : piece_points_buider_{PiecePointsBuilder()}
      , game_board_{GameBoard()} {};
};

TEST_F(PiecePointsEvaluatorTest, EvaluateMove) {
  auto game_position_points = piece_points_buider_.BuildGamePositionPoints(
      kBasePointsICGA2004,
      kAllOffsetsICGA2004
  );
  auto piece_points_evaluator = PiecePointsEvaluator(game_position_points);
  auto black_points_total =
      piece_points_evaluator.GetPlayerTotal(PieceColor::kBlk, game_board_);
  auto red_points_total =
      piece_points_evaluator.GetPlayerTotal(PieceColor::kRed, game_board_);

  auto red_moves = game_board_.CalcFinalMovesOf(PieceColor::kRed);
  auto executed_move = game_board_.ExecuteMove(red_moves.moves[0]);

  auto red_new_points_total =
      piece_points_evaluator.GetPlayerTotal(PieceColor::kRed, game_board_);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
