#include <common.hpp>
#include <gtest/gtest.h>
#include <iostream>
#include <piece_points_spec.hpp>


using namespace piece_points_spec;


class PiecePointsSpecTest : public ::testing::Test {
    protected:
    const string points_spec_path =
      "/home/duane/workspace/project/src/cpp_modules/src/piece_points/"
      "ICGA_2004_bpo.json";
};

TEST_F(PiecePointsSpecTest, InitPointsSpecBPOExternal) {
    auto external_pts_spec = PointsSpecBPOExternal(points_spec_path);
    EXPECT_EQ(external_pts_spec.black_base.size(), kNumPieceTypeVals);
    EXPECT_EQ(external_pts_spec.red_base_offsets.size(), kNumPieceTypeVals);
    EXPECT_EQ(external_pts_spec.black_position.size(), kNumPieceTypeVals);
    EXPECT_EQ(external_pts_spec.red_position_offsets.size(), kNumPieceTypeVals);
    for (auto base_val : external_pts_spec.black_base) {
        EXPECT_TRUE((base_val.second != 0) || (base_val.first == "null"));
    }
    for (auto position_array : external_pts_spec.black_position) {
        EXPECT_EQ(position_array.second.size(), kNumRanks);
        for (auto rank : position_array.second) {
            EXPECT_EQ(rank.size(), kNumFiles);
        }
    }
    EXPECT_EQ(external_pts_spec.black_position["chariot"][0][0], -2);
    for (auto base_val : external_pts_spec.red_base_offsets) {
        EXPECT_EQ(base_val.second, 0);
    }
    for (auto position_array : external_pts_spec.red_position_offsets) {
        for (auto rank = 0; rank < kNumRanks; rank++) {
            for (auto file = 0; file < kNumFiles; file++) {
                EXPECT_EQ(position_array.second[rank][file], 0);
            }
        }
    }
}

TEST_F(PiecePointsSpecTest, PointsSpecBPOExternalToJson) {
    auto external_pts_spec = PointsSpecBPOExternal(points_spec_path);
    auto json_object = external_pts_spec.ToJson();
}

TEST_F(PiecePointsSpecTest, InitPointsSpecBPOInternal) {
    auto internal_pts_spec = PointsSpecBPOExternal(points_spec_path);
}

TEST_F(PiecePointsSpecTest, PointsSpecBOPExternalToGamePointsSMap) {
    auto external_pts_spec = PointsSpecBPOExternal(points_spec_path);
    auto game_points_smap = external_pts_spec.ToGamePointsSmap();
}


int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
