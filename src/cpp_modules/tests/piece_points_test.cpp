#include <gtest/gtest.h>
#include <piece_points.hpp>

using namespace piece_points;

class PiecePointsBuilderTest : public ::testing::Test {

protected:
const string points_spec_path = "/home/duane/workspace/project/src/cpp_modules/piece_points/"
                 "ICGA_2004_bpo.json";

  const PointsSpecBPOExternal external_pts_spec = PointsSpecBPOExternal(points_spec_path);
  const PointsSpecBPOInternal internal_pts_spec = PointsSpecBPOInternal(external_pts_spec);
  PiecePointsBuilder piece_points_builder_ = PiecePointsBuilder(internal_pts_spec);
  // PiecePointsBuilderTest()
  //     : piece_points_builder_{PiecePointsBuilder()} {};
};

TEST_F(PiecePointsBuilderTest, BuildGamePoints) {
  auto game_points = piece_points_builder_.BuildGamePositionPoints(
      // kBasePointsICGA2004,
      // kAllOffsetsICGA2004
  );
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}