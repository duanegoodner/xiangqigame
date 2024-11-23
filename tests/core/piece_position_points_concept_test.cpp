#include <gtest/gtest.h>
#include <piece_points_bpo.hpp>
#include <piece_position_points_for_concepts.hpp>
#include <piece_value_provider_new_concept.hpp>
#include <shared_ptr_builder_tests.hpp>
#include <type_traits>
#include <utility_functs.hpp>

// Run our standard test suite for a Builder that has .build() -> shared_ptr
using PiecePositionPointsBuilderTestTypes = ::testing::Types<BuilderObjectPair<
    piecepoints::PiecePositionPointsBuilder,
    piecepoints::PiecePositionPointsForConcepts>>;
INSTANTIATE_TYPED_TEST_SUITE_P(
    PiecePositionPointsBuilderTestInstance,
    SharedPtrBuilderTest,
    PiecePositionPointsBuilderTestTypes
);

//! Tests specific to a GameBoardBuilder (beyone those in the generic Builder suite)
class PiecePositionPointsBuilderTest : public ::testing::Test {
protected:
  piecepoints::PiecePositionPointsBuilder builder_;
};

TEST_F(PiecePositionPointsBuilderTest, CreatesValidPiecePositionPoints) {
  auto piece_position_points = builder_.build();
  auto full_points_array = piece_position_points->points_array;
  static_assert(
      std::is_same<decltype(full_points_array), piecepoints::GamePointsArray_t>::value,
      "The .points array should be the same as GamePointsArray_t."
  );
  auto team_points_array = full_points_array[0];
  static_assert(
      std::is_same<decltype(team_points_array), piecepoints::TeamPointsArray_t>::value,
      "A single element of .points array should be the same as TeamPointsArray_t"
  );
  auto single_piece_array = team_points_array[0];
  EXPECT_EQ(single_piece_array.size(), gameboard::kNumRanks)
      << "Number of elements in one row of single piece array should match number of "
         "ranks on board.";
  EXPECT_EQ(
      single_piece_array[0].size(),
      gameboard::kNumFiles
  ) << "Size of final dimension of points array should match number of files on board.";
}

class PiecePositionPointsForConceptsTest : public ::testing::Test {
protected:
  piecepoints::PiecePositionPointsBuilder builder_;
  std::shared_ptr<piecepoints::PiecePositionPointsForConcepts> piece_position_points_ =
      builder_.build();

  // const string points_spec_path =
  //     utility_functs::get_data_file_abs_path("ICGA_2004_bpo.json");
  // const string raw_points_json_path =
  //     utility_functs::get_data_file_abs_path("ICGA_2004_raw.json");
  // piecepoints::BPOPointsSKeys bpo_points_skeys{points_spec_path};
  // piecepoints::BPOPointsEKeys bpo_points_ekeys = bpo_points_skeys.ToBPOPointsEKeys();
  // piecepoints::GamePointsArray_t game_points_array =
  //     bpo_points_skeys.ToGamePointsArray();
};

TEST_F(PiecePositionPointsForConceptsTest, SatisfiesPieceValueProviderConcept) {
  static_assert(
      PieceValueProviderConcept<piecepoints::PiecePositionPointsForConcepts>,
      "PiecePositionPointsForConcepts must satisfy PieceValueProviderConcept"
  );
}

TEST_F(PiecePositionPointsForConceptsTest, SpotCheckBlackValues) {
  // piecepoints::PiecePositionPointsForConcepts piece_points{game_points_array};
  gameboard::BoardSpace black_cannon_space{9, 4};
  gameboard::BoardSpace black_advisor_space{0, 0};
  gameboard::BoardSpace black_horse_space{0, 1};

  auto black_cannon_9_4 = piece_position_points_->GetValueOfPieceAtPosition(
      gameboard::PieceColor::kBlk,
      gameboard::PieceType::kCan,
      black_cannon_space
  );
  auto black_advisor_0_0 = piece_position_points_->GetValueOfPieceAtPosition(
      gameboard::PieceColor::kBlk,
      gameboard::PieceType::kAdv,
      black_advisor_space
  );
  auto black_horse_0_1 = piece_position_points_->GetValueOfPieceAtPosition(
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
  auto s_map = piece_position_points_->PointsArrayToSmap();

  // confirm corresponding PiecePointsArray_t arrays in s_map and
  // game_points_array are equal
  for (auto color : s_map) {
    for (auto piece_type : color.second) {
      EXPECT_EQ(
          piece_type.second,
          piece_position_points_->points_array[GetZColorIndexOf(
              gameboard::kPieceColorStringToEnum.at(color.first)
          )][gameboard::kPieceTypeStringToEnum.at(piece_type.first)]
      );
    }
  }
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}