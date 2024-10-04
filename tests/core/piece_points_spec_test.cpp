#include <common.hpp>
#include <gtest/gtest.h>
#include <iostream>
#include <piece_points_spec.hpp>
#include <utility_functs.hpp>

using namespace piece_points;

class PiecePointsSpecTest : public ::testing::Test {
protected:
  const string points_spec_path =
      utility_functs::get_data_file_abs_path("ICGA_2004_bpo.json");
};

TEST_F(PiecePointsSpecTest, InitBPOPointsSKeys) {
  auto external_pts_spec = BPOPointsSKeys(points_spec_path);
  EXPECT_EQ(external_pts_spec.black_base_.size(), kNumPieceTypeVals);
  EXPECT_EQ(external_pts_spec.red_base_offsets_.size(), kNumPieceTypeVals);
  EXPECT_EQ(external_pts_spec.black_position_.size(), kNumPieceTypeVals);
  EXPECT_EQ(external_pts_spec.red_position_offsets_.size(), kNumPieceTypeVals);
  for (auto base_val : external_pts_spec.black_base_) {
    EXPECT_TRUE((base_val.second != 0) || (base_val.first == "null"));
  }
  for (auto position_array : external_pts_spec.black_position_) {
    EXPECT_EQ(position_array.second.size(), kNumRanks);
    for (auto rank : position_array.second) {
      EXPECT_EQ(rank.size(), kNumFiles);
    }
  }
  EXPECT_EQ(external_pts_spec.black_position_["chariot"][0][0], -2);
  for (auto base_val : external_pts_spec.red_base_offsets_) {
    EXPECT_EQ(base_val.second, 0);
  }
  for (auto position_array : external_pts_spec.red_position_offsets_) {
    for (auto rank = 0; rank < kNumRanks; rank++) {
      for (auto file = 0; file < kNumFiles; file++) {
        EXPECT_EQ(position_array.second[rank][file], 0);
      }
    }
  }
}

TEST_F(PiecePointsSpecTest, InitBPOPointsEKeys) {
  auto internal_pts_spec = BPOPointsSKeys(points_spec_path);
}

TEST_F(PiecePointsSpecTest, PointsSpecBOPExternalToGamePointsSMap) {
  auto external_pts_spec = BPOPointsSKeys(points_spec_path);
  auto game_points_smap = external_pts_spec.ToGamePointsSmap();
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
