#include <gtest/gtest.h>
#include <piece_points.hpp>

using namespace piece_points;

class PiecePointsBuilderTest : public ::testing::Test {

protected:
  PiecePointsBuilder piece_points_builder_;
  PiecePointsBuilderTest()
      : piece_points_builder_{PiecePointsBuilder()} {};
};

TEST_F(PiecePointsBuilderTest, BuildGamePoints) {
  auto game_points = piece_points_builder_.BuildGamePositionPoints(
      kBasePointsICGA2004,
      kAllOffsetsICGA2004
  );
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}