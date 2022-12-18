#include "piece_points.hpp"

#include <gtest/gtest.h>

using namespace piece_points;

class PiecePointsBuilderTest : public ::testing::Test {
    // TeamBasePoints_t black_base_pts = kBasePointsICGA2004;
    // TeamPositionPoints_t black_offsets = kAllOffsetsICGA2004;

    protected:
    PiecePointsBuilder piece_points_builder_;
    PiecePointsBuilderTest()
        : piece_points_builder_{PiecePointsBuilder()} {};
};

TEST_F(PiecePointsBuilderTest, BuildGamePoints) {
    auto game_points = piece_points_builder_.BuildGamePositionPoints(
        kBasePointsICGA2004, kAllOffsetsICGA2004
    );
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}