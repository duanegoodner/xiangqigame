#include "piece_points.hpp"

#include <gtest/gtest.h>

class PiecePointsTest : public ::testing::Test {

    TeamBasePoints_t base_pts = kBasePointsICGA2004;
    TeamPositionPoints_t offsets = kAllOffsetsICGA2004;

    protected:
    PiecePoints piece_points_;
    PiecePointsTest()
        : piece_points_{PiecePoints(base_pts, offsets)} {};
};

TEST_F(PiecePointsTest, CreatePiecePoints) {
    EXPECT_EQ(
        piece_points_
        .black_piece_points_[PieceType::kGen][0][0], 6000);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}