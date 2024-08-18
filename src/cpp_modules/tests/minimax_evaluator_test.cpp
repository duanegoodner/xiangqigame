#include <game_board.hpp>
#include <gtest/gtest.h>
#include <hash_calculator.hpp>
#include <minimax_evaluator.hpp>
#include <piece_points.hpp>
#include <utility_functs.hpp>

using namespace piece_points;

class PiecePointsEvaluatorTest : public ::testing::Test {

protected:
  // PiecePointsBuilder piece_points_buider_;
  const string points_spec_path = utility_functs::get_data_file_abs_path("ICGA_2004_bpo.json");

  const PointsSpecBPOExternal external_pts_spec =
      PointsSpecBPOExternal(points_spec_path);
  const PointsSpecBPOInternal internal_pts_spec =
      PointsSpecBPOInternal(external_pts_spec);
//   PiecePointsBuilder piece_points_builder_ =
//       PiecePointsBuilder(internal_pts_spec);
  GamePointsArrayBuilder game_points_array_builder_ =
      GamePointsArrayBuilder(internal_pts_spec);
  GameBoard<HashCalculator> game_board_;
  // PiecePointsEvaluatorTest()
  //     : piece_points_buider_{PiecePointsBuilder()}
  //     , game_board_{GameBoard<HashCalculator>()} {};
};

TEST_F(PiecePointsEvaluatorTest, EvaluateMove) {
  // auto game_position_points =
  // piece_points_builder_.BuildGamePoints();
  auto game_position_points =
      PiecePoints(game_points_array_builder_.BuildGamePointsArray());

  auto piece_points_evaluator =
      PiecePointsEvaluator<GameBoard<HashCalculator>, PiecePoints>(game_position_points);
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
