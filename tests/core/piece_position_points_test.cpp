#include <gtest/gtest.h>
#include <iostream>
#include <piecepoints/base_position_offset.hpp>
#include <piecepoints/piece_position_points.hpp>
#include <utility_functs.hpp>

using namespace piecepoints;

class PiecePositionPointsTest : public ::testing::Test {

protected:
  const string points_spec_path =
      utility_functs::get_data_file_abs_path("ICGA_2004_bpo.json");
  const string raw_points_json_path =
      utility_functs::get_data_file_abs_path("ICGA_2004_raw.json");
  BPOPointsSKeys bpo_points_skeys{points_spec_path};
  BPOPointsEKeys bpo_points_ekeys = bpo_points_skeys.ToBPOPointsEKeys();
  GamePointsArray_t game_points_array = bpo_points_skeys.ToGamePointsArray();
};

TEST_F(PiecePositionPointsTest, InitFromGamePointsArray) {
  PiecePositionPoints piece_points = PiecePositionPoints(game_points_array);
}

TEST_F(PiecePositionPointsTest, InitFromBPOExternal) {
  PiecePositionPoints piece_points = PiecePositionPoints(bpo_points_skeys);
}

TEST_F(PiecePositionPointsTest, InitFromBPOEKeys) {
  PiecePositionPoints piece_points = PiecePositionPoints(bpo_points_ekeys);
}

TEST_F(PiecePositionPointsTest, SpotCheckBlackValues) {
  PiecePositionPoints piece_points = PiecePositionPoints(game_points_array);
  auto black_cannon_9_4 = piece_points.ImplementGetValueOfPieceAtPosition(
      PieceColor::kBlk,
      PieceType::kCan,
      BoardSpace{9, 4}
  );
  auto black_advisor_0_0 = piece_points.ImplementGetValueOfPieceAtPosition(
      PieceColor::kBlk,
      PieceType::kAdv,
      BoardSpace{0, 0}
  );
  auto black_horse_0_1 = piece_points.ImplementGetValueOfPieceAtPosition(
      PieceColor::kBlk,
      PieceType::kHor,
      BoardSpace{0, 1}
  );

  EXPECT_EQ(black_cannon_9_4, 285 - 12);
  EXPECT_EQ(black_advisor_0_0, 120 + 0);
  EXPECT_EQ(black_horse_0_1, 270 - 4);
}

TEST_F(PiecePositionPointsTest, RedPointsFlippedWRTBlack) {
  for (auto piece_type_index = 0; piece_type_index < kNumPieceTypeVals;
       piece_type_index++) {
    for (auto rank = 0; rank < kNumRanks; rank++) {
      for (auto file = 0; file < kNumFiles; file++) {
      }
    }
  }
}

TEST_F(PiecePositionPointsTest, ConvertGamePointsArrayToSMap) {
  PiecePositionPoints piece_position_points = PiecePositionPoints(game_points_array);
  auto s_map = piece_position_points.PointsArrayToSmap();

  // confirm corresponding PiecePointsArray_t arrays in s_map and
  // game_points_array are equal
  for (auto color : s_map) {
    for (auto piece_type : color.second) {
      EXPECT_EQ(
          piece_type.second,
          game_points_array[GetZColorIndexOf(kPieceColorStringToEnum.at(color.first))]
                           [kPieceTypeStringToEnum.at(piece_type.first)]
      );
    }
  }
}

TEST_F(PiecePositionPointsTest, InitFromFile) {
  auto piece_points = PiecePositionPoints(points_spec_path);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}