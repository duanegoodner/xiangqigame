#include <common.hpp>
#include <gtest/gtest.h>
#include <iostream>
#include <piece_points.hpp>
#include <piece_points_spec.hpp>

using namespace piece_points;

class GamePointsArrayBuilderTest : public ::testing::Test {

protected:
  const string points_spec_path =
      "/home/duane/workspace/project/src/cpp_modules/piece_points/"
      "ICGA_2004_bpo.json";

  const PointsSpecBPOExternal external_pts_spec =
      PointsSpecBPOExternal(points_spec_path);
  const PointsSpecBPOInternal internal_pts_spec =
      PointsSpecBPOInternal(external_pts_spec);

  GamePointsArrayBuilder game_points_array_builder_ =
      GamePointsArrayBuilder(points_spec_path);
};

TEST_F(GamePointsArrayBuilderTest, BuildGamePoints) {
  auto game_points_array = game_points_array_builder_.BuildGamePointsArray();
  for (auto piece_index = 0; piece_index < kNumPieceTypeVals; piece_index++) {
    auto black_piece_array =
        game_points_array[get_zcolor_index(PieceColor::kBlk)];
    // auto flipped
  }

  // for (auto color : game_points) {
  //   for (auto piece : color.second) {
  //     for (auto rank = 0; rank < kNumRanks; rank++) {
  //       for (auto file = 0; file < kNumFiles; file++) {
  //         std::cout << color.first << ", " << piece.first << ", " << rank <<
  //         ", " << file << std::endl; EXPECT_EQ(
  //           game_points[color.first][piece.first][rank][file],
  //           game_points_array[get_zcolor_index(color.first)][piece.first][rank][file]);
  //       }
  //     }
  //   }
  // }
  // std::cout << "done" << std::endl;
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}