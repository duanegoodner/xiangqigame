#include "common.hpp"
#include <gtest/gtest.h>
#include <game_points.hpp>
#include <iostream>
#include <utility_functs.hpp>

using namespace piece_points;

class PiecePointsBuilderTest : public ::testing::Test {

protected:
  const string points_spec_path = utility_functs::get_data_file_abs_path("ICGA_2004_bpo.json");

  const PointsSpecBPOExternal external_pts_spec =
      PointsSpecBPOExternal(points_spec_path);
  const PointsSpecBPOInternal internal_pts_spec =
      PointsSpecBPOInternal(external_pts_spec);
  PiecePointsBuilder piece_points_builder_ =
      PiecePointsBuilder(internal_pts_spec);

  GamePointsArrayBuilder game_points_array_builder_ =
      GamePointsArrayBuilder(points_spec_path);
};

TEST_F(PiecePointsBuilderTest, BuildGamePoints) {
  auto game_points = piece_points_builder_.BuildGamePoints();
  auto game_points_array = game_points_array_builder_.BuildGamePointsArray();
  for (auto color : game_points) {
    for (auto piece : color.second) {
      for (auto rank = 0; rank < kNumRanks; rank++) {
        for (auto file = 0; file < kNumFiles; file++) {
          std::cout << color.first << ", " << piece.first << ", " << rank << ", " << file << std::endl;
          EXPECT_EQ(
            game_points[color.first][piece.first][rank][file],
            game_points_array[get_zcolor_index(color.first)][piece.first][rank][file]);
        }
      }
    }
  }
  std::cout << "done" << std::endl;

}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}