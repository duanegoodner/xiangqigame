#include <common.hpp>
#include <filesystem>
#include <gtest/gtest.h>
#include <json_internal.hpp>
#include <nlohmann/json.hpp>
#include <piece_points_spec.hpp>
#include <string>
#include <typeinfo>
#include <utility_functs.hpp>

using namespace std;
using namespace json_internal;
using nloh_json = nlohmann::json;
using namespace piece_points_spec;



class TeamPointsTestNlohmann : public ::testing::Test {
protected:

  const string kICGARawPath = utility_functs::get_data_file_abs_path("ICGA_2004_raw.json");
                        
  const nloh_json nloh_raw_json_game = import_json<nloh_json>(kICGARawPath);
  nloh_json nloh_raw_json_red = nloh_raw_json_game.at("red");
};

TEST_F(TeamPointsTestNlohmann, InitFromJson) {
  TeamPoints red_team_points = TeamPoints<nloh_json>(nloh_raw_json_red);
}

TEST_F(TeamPointsTestNlohmann, PiecePointsArraysMatchJsonSource) {
  TeamPoints red_team_points = TeamPoints<nloh_json>(nloh_raw_json_red);
  for (auto &[key, value] : nloh_raw_json_red.items()) {
    EXPECT_EQ(value, red_team_points.PiecePointsArrays()[key]);
  }
}

TEST_F(TeamPointsTestNlohmann, ToJson) {
  TeamPoints red_team_points = TeamPoints<nloh_json>(nloh_raw_json_red);
  auto result = red_team_points.ToJson();
  for (auto &[key, value] : result.items()) {
    EXPECT_EQ(result.at(key), nloh_raw_json_red.at(key));
  }
}

TEST_F(TeamPointsTestNlohmann, ToArray) {
  auto red_team_points = TeamPoints<nloh_json>(nloh_raw_json_red);
  auto result = red_team_points.ToArray();
  for (auto &[key, value] : nloh_raw_json_red.items()) {
    EXPECT_EQ(
        nloh_raw_json_red.at(key),
        result[kPieceTypeStringToEnum.at(key)]
    );
  }
}

class GamePointsTestNlohmann : public ::testing::Test {
protected:
  const string kICGARawPath = utility_functs::get_data_file_abs_path("ICGA_2004_raw.json");
  const string kRawSchemaPath = utility_functs::get_data_file_abs_path("raw_points_schema.json");
  nloh_json nloh_raw_json_game = import_json<nloh_json>(kICGARawPath);
};

// TEST_F(GamePointsTestNlohmann, InitFromJson) {
//   auto game_points = GamePoints<nloh_json>(nloh_raw_json_game);
// }

TEST_F(GamePointsTestNlohmann, PiecePointsArraysMatchJsonSource) {
  auto game_points = GamePoints<nloh_json>(nloh_raw_json_game);
  for (auto &[color_key, color_value] : nloh_raw_json_game.items()) {
    for (auto &[piece_type_key, piece_type_value] : color_value.items()) {
      EXPECT_EQ(
          nloh_raw_json_game.at(color_key).at(piece_type_key),
          game_points.TeamPointsJsons().at(color_key).PiecePointsArrays().at(
              piece_type_key
          )
      );
    }
  }
}

TEST_F(GamePointsTestNlohmann, InitFromString) {
  auto game_points = GamePoints<nloh_json>(kICGARawPath);
}

TEST_F(GamePointsTestNlohmann, ToJson) {
  auto game_points = GamePoints<nloh_json>(nloh_raw_json_game);
  auto json_output = game_points.ToJson();
  EXPECT_EQ(nloh_raw_json_game, json_output);
}

TEST_F(GamePointsTestNlohmann, ToArray) {
  auto game_points = GamePoints<nloh_json>(nloh_raw_json_game);
  auto result = game_points.ToArray();
  EXPECT_EQ(typeid(result), typeid(GamePointsArray_t));
  EXPECT_EQ(typeid(result[0]), typeid(TeamPointsArray_t));
  EXPECT_EQ(typeid(result[0][0]), typeid(PiecePointsArray_t));

  for (auto [color_key, color_val] : nloh_raw_json_game.items()) {
    for (auto [piece_type_key, piece_type_val] : color_val.items()) {
      EXPECT_EQ(
          nloh_raw_json_game.at(color_key).at(piece_type_key),
          result[get_zcolor_index(kPieceColorStringToEnum.at(color_key))]
                [kPieceTypeStringToEnum.at(piece_type_key)]
      );
    }
  }
}

TEST_F(GamePointsTestNlohmann, ToFileOutputExists) {
  size_t random_int =
      utility_functs::random((size_t)0, (size_t)numeric_limits<size_t>::max);
  auto output_path = testing::TempDir() + "GTEST-" + to_string(random_int);

  GamePoints<nloh_json>(nloh_raw_json_game).ToFile(output_path);
  EXPECT_TRUE(filesystem::exists(output_path));
  auto file_removed = filesystem::remove(output_path);
  EXPECT_TRUE(file_removed);
}

// TEST_F(GamePointsTestNlohmann, ToFileOutputHasCorrectFormat) {
//   size_t random_int =
//       utility_functs::random((size_t)0, (size_t)numeric_limits<size_t>::max);
//   auto output_path = testing::TempDir() + "GTEST-" + to_string(random_int);

// }

TEST_F(GamePointsTestNlohmann, StructDataMatchesJsonData) {
  auto game_points_struct = GamePoints<nloh_json>(nloh_raw_json_game);
  auto result = game_points_struct_match_json<nloh_json>(
      game_points_struct,
      nloh_raw_json_game
  );
}
