#include <gtest/gtest.h>
#include <piece_points_bpo.hpp>
#include <piece_position_points_for_concepts.hpp>
#include <piece_value_provider_new_concept.hpp>
#include <type_traits>
#include <utility_functs.hpp>

class PiecePositionPointsForConceptsTest : public ::testing::Test {
protected:
  const string points_spec_path =
      utility_functs::get_data_file_abs_path("ICGA_2004_bpo.json");
  const string raw_points_json_path =
      utility_functs::get_data_file_abs_path("ICGA_2004_raw.json");
  piecepoints::BPOPointsSKeys bpo_points_skeys{points_spec_path};
  piecepoints::BPOPointsEKeys bpo_points_ekeys = bpo_points_skeys.ToBPOPointsEKeys();
  piecepoints::GamePointsArray_t game_points_array =
      bpo_points_skeys.ToGamePointsArray();
};

TEST_F(PiecePositionPointsForConceptsTest, SatisfiesPieceValueProviderConcept) {
  static_assert(
      PieceValueProviderConcept<piecepoints::PiecePositionPointsForConcepts>,
      "PiecePositionPointsForConcepts must satisfy PieceValueProviderConcept"
  );
}

TEST_F(PiecePositionPointsForConceptsTest, InitFromGamePointsArray) {
  piecepoints::PiecePositionPointsForConcepts piece_points{game_points_array};
}

TEST_F(PiecePositionPointsForConceptsTest, InitFromBPOExternal) {
  piecepoints::PiecePositionPointsForConcepts piece_points{bpo_points_skeys};
}

TEST_F(PiecePositionPointsForConceptsTest, InitFromBPOEKeys) {
  piecepoints::PiecePositionPointsForConcepts piece_points{bpo_points_ekeys};
}

TEST_F(PiecePositionPointsForConceptsTest, SpotCheckBlackValues) {
  piecepoints::PiecePositionPointsForConcepts piece_points{game_points_array};
  gameboard::BoardSpace black_cannon_space{9, 4};
  gameboard::BoardSpace black_advisor_space{0, 0};
  gameboard::BoardSpace black_horse_space{0, 1};

  auto black_cannon_9_4 = piece_points.GetValueOfPieceAtPosition(
      gameboard::PieceColor::kBlk,
      gameboard::PieceType::kCan,
      black_cannon_space
  );
  auto black_advisor_0_0 = piece_points.GetValueOfPieceAtPosition(
      gameboard::PieceColor::kBlk,
      gameboard::PieceType::kAdv,
      black_advisor_space
  );
  auto black_horse_0_1 = piece_points.GetValueOfPieceAtPosition(
      gameboard::PieceColor::kBlk,
      gameboard::PieceType::kHor,
      black_horse_space
  );

  EXPECT_EQ(black_cannon_9_4, 285 - 12);
  EXPECT_EQ(black_advisor_0_0, 120 + 0);
  EXPECT_EQ(black_horse_0_1, 270 - 4);
}

TEST_F(PiecePositionPointsForConceptsTest, RedPointsFlippedWRTBlack) {
  for (auto piece_type_index = 0; piece_type_index < gameboard::kNumPieceTypeVals;
       piece_type_index++) {
    for (auto rank = 0; rank < gameboard::kNumRanks; rank++) {
      for (auto file = 0; file < gameboard::kNumFiles; file++) {
      }
    }
  }
}

TEST_F(PiecePositionPointsForConceptsTest, ConvertGamePointsArrayToSMap) {
  piecepoints::PiecePositionPointsForConcepts piece_position_points =
      piecepoints::PiecePositionPointsForConcepts(game_points_array);
  auto s_map = piece_position_points.PointsArrayToSmap();

  // confirm corresponding PiecePointsArray_t arrays in s_map and
  // game_points_array are equal
  for (auto color : s_map) {
    for (auto piece_type : color.second) {
      EXPECT_EQ(
          piece_type.second,
          game_points_array[GetZColorIndexOf(
              gameboard::kPieceColorStringToEnum.at(color.first)
          )][gameboard::kPieceTypeStringToEnum.at(piece_type.first)]
      );
    }
  }
}

TEST_F(PiecePositionPointsForConceptsTest, InitFromFile) {
  auto piece_points = piecepoints::PiecePositionPointsForConcepts(points_spec_path);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}