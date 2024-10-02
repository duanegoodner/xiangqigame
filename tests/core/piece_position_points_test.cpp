#include "utility_functs.hpp"
#include <common.hpp>
#include <gtest/gtest.h>
#include <iostream>
#include <piece_points.hpp>
#include <piece_points_spec.hpp>
#include <utility_functs.hpp>

using namespace piece_points;

class GamePointsArrayBuilderTest : public ::testing::Test {

protected:
  const string points_spec_path =
      utility_functs::get_data_file_abs_path("ICGA_2004_bpo.json");

  const PointsSpecBPOExternal external_pts_spec =
      PointsSpecBPOExternal(points_spec_path);
  const PointsSpecBPOInternal internal_pts_spec =
      PointsSpecBPOInternal(external_pts_spec);
  GamePointsArray_t game_points_array =
      GamePointsArrayBuilder(points_spec_path).BuildGamePointsArray();
};

TEST_F(GamePointsArrayBuilderTest, CorrectArraySize) {
  EXPECT_EQ(
      game_points_array.size(),
      kNumPieceColorVals - 1
  ); // no sub-array for null color
  EXPECT_EQ(game_points_array[0].size(), kNumPieceTypeVals);
  EXPECT_EQ(game_points_array[0][0].size(), kNumRanks);
  EXPECT_EQ(game_points_array[0][0][0].size(), kNumFiles);
}

TEST_F(GamePointsArrayBuilderTest, SpotCheckCorrectValues) {
  auto black_cannon_9_4 = game_points_array[get_zcolor_index(PieceColor::kBlk)]
                                           [PieceType::kCan][9][4];
  auto black_advisor_0_0 =
      game_points_array[get_zcolor_index(PieceColor::kBlk)][PieceType::kAdv][0]
                       [0];

  auto black_horse_0_1 = game_points_array[get_zcolor_index(PieceColor::kBlk)]
                                          [PieceType::kHor][0][1];

  EXPECT_EQ(black_cannon_9_4, 285 - 12);
  EXPECT_EQ(black_advisor_0_0, 120 + 0);
  EXPECT_EQ(black_horse_0_1, 270 - 4);
}

TEST_F(GamePointsArrayBuilderTest, RedPointsArraysProperlyFlipped) {
  for (auto piece_index = 0; piece_index < kNumPieceTypeVals; piece_index++) {
    auto black_piece_array =
        game_points_array[get_zcolor_index(PieceColor::kBlk)][piece_index];
    auto flipped_black_piece_array =
        utility_functs::vertical_flip_array(black_piece_array);
    auto red_piece_array =
        game_points_array[get_zcolor_index(PieceColor::kRed)][piece_index];
    EXPECT_TRUE(flipped_black_piece_array == red_piece_array);
  }
}

class PiecePositionPointsTest : public ::testing::Test {

protected:
  const string points_spec_path =
      utility_functs::get_data_file_abs_path("ICGA_2004_bpo.json");
  const string raw_points_json_path =
      utility_functs::get_data_file_abs_path("ICGA_2004_raw.json");
  const PointsSpecBPOExternal external_pts_spec =
      PointsSpecBPOExternal(points_spec_path);
  const PointsSpecBPOInternal internal_pts_spec =
      PointsSpecBPOInternal(external_pts_spec);
  GamePointsArray_t game_points_array =
      GamePointsArrayBuilder(points_spec_path).BuildGamePointsArray();
};

TEST_F(PiecePositionPointsTest, InitFromGamePointsArray) {
  PiecePositionPoints piece_points = PiecePositionPoints(game_points_array);
}

TEST_F(PiecePositionPointsTest, InitFromBPOExternal) {
  PiecePositionPoints piece_points = PiecePositionPoints(external_pts_spec);
}

TEST_F(PiecePositionPointsTest, InitFromBPOInternal) {
  PiecePositionPoints piece_points = PiecePositionPoints(internal_pts_spec);
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

TEST_F(PiecePositionPointsTest, game_points_array_to_smap) {
  auto s_map = game_points_array_to_smap(game_points_array);

  // confirm corresponding PiecePointsArray_t arrays in s_map and
  // game_points_array are equal
  for (auto color : s_map) {
    for (auto piece_type : color.second) {
      EXPECT_EQ(
          piece_type.second,
          game_points_array[get_zcolor_index(
              kPieceColorStringToEnum.at(color.first)
          )][kPieceTypeStringToEnum.at(piece_type.first)]
      );
    }
  }
}

TEST_F(PiecePositionPointsTest, PiecePositionPointsToJson) {
  PiecePositionPoints piece_points = PiecePositionPoints(game_points_array);
  auto json_object = piece_points.ToJson();
}

TEST_F(PiecePositionPointsTest, import_json) {
  auto imported_raw_json = utility_functs::import_json(raw_points_json_path);
  auto s_map = raw_points_to_smap(imported_raw_json);
  auto piece_ponts = PiecePositionPoints(s_map);
  std::cout << "done" << std::endl;
}

TEST_F(PiecePositionPointsTest, InitFromFile) {
  auto piece_points = PiecePositionPoints(raw_points_json_path);
}

// TEST(ArrayBuilder_2Test, InitWithPath) {
//   json json_object = utility_functs::import_json(kICGARawPath);
//   auto array_builder_from_raw = GamePointsArrayBuilder_2(json_object);
// }

// TEST(FormatIdentifierTest, InitWithJsonObject) {
//   json json_object = utility_functs::import_json(kICGARawPath);
//   auto format_identifier = FormatIdentifier(json_object);
// }

// TEST(ImportSchemasTest, ImportDefaultSchemaPaths) {
//   auto schemas = import_schemas(DEFAULT_POINTS_SCHEMA_PATHS);
//   std::cout << "done" << std::endl;
// }

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}