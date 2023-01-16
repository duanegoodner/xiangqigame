#include <gtest/gtest.h>
#include <piece_points.hpp>
#include <iostream>

using namespace piece_points;

class PiecePointsBuilderTest : public ::testing::Test {

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

TEST_F(PiecePointsBuilderTest, BuildGamePoints) {
  auto game_points_array = game_points_array_builder_.BuildGamePointsArray();

}

TEST_F(PiecePointsBuilderTest, ArrayToMap) {
  auto game_points_array = game_points_array_builder_.BuildGamePointsArray();
  auto game_points_map = game_points_array_to_map(game_points_array);
  std::cout << "done" << std::endl;
}

TEST_F(PiecePointsBuilderTest, EMapTo_SMap) {
  auto game_points_array = game_points_array_builder_.BuildGamePointsArray();
  auto game_points_emap = game_points_array_to_map(game_points_array);
  auto game_points_smap = game_points_emap_to_smap(game_points_emap);
  std::cout << "done" << std::endl;
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}