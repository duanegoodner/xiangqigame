#include <common.hpp>
#include <gtest/gtest.h>
#include <json_internal.hpp>
#include <nlohmann/json.hpp>
#include <piece_points_spec.hpp>
#include <utility_functs.hpp>

using namespace std;
using namespace json_internal;
using nloh_json = nlohmann::json;
using namespace piece_points_spec;

class TeamPointsTest : public ::testing::Test {
protected:
  string kICGARawPath = "/home/duane/workspace/project/src/cpp_modules/data/"
                        "ICGA_2004_raw.json";
  nloh_json nloh_raw_json_game = import_json<nloh_json>(kICGARawPath);
  nloh_json nloh_raw_json_red = nloh_raw_json_game.at("red");
};

TEST_F(TeamPointsTest, InitFromNlohmannJson) {
  TeamPoints red_team_points = TeamPoints<nloh_json>(nloh_raw_json_red);
  for (auto& [key, value] : nloh_raw_json_red.items()) {
    EXPECT_EQ(
      value,
      red_team_points.PiecePointsArrays()[key]
      );
  }
}

TEST_F(TeamPointsTest, NlohmannToJson) {
  auto nloh_raw_data = import_json<nloh_json>(kICGARawPath);
  auto red_team_points_json = nloh_raw_data.at("red");
  auto red_team_points_struct = TeamPoints<nloh_json>(red_team_points_json);
  auto result = red_team_points_struct.ToJson();
  for (auto& [key, value] : result.items()) {
    EXPECT_EQ(
      result.at(key),
      red_team_points_json.at(key)
    );
  }
}

TEST_F(TeamPointsTest, NolohmannToArray) {
  auto nloh_raw_data = import_json<nloh_json>(kICGARawPath);
  auto red_team_points_json = nloh_raw_data.at("red");
  auto red_team_points_struct = TeamPoints<nloh_json>(red_team_points_json);
  auto result = red_team_points_struct.ToArray();
  for (auto& [key, value] : red_team_points_json.items()) {
    EXPECT_EQ(
      red_team_points_json.at(key),
      result[kPieceTypeStringToEnum.at(key)]
    );
  }
}


class GamePointsTest : public ::testing::Test {
  protected:
  string kICGABPOPath = "/home/duane/workspace/project/src/cpp_modules/data/"
                        "ICGA_2004_bpo.json";

  string kICGARawPath = "/home/duane/workspace/project/src/cpp_modules/data/"
                        "ICGA_2004_raw.json";

  string kRawSchemaPath = "/home/duane/workspace/project/src/cpp_modules/data/"
                          "raw_points_schema.json";

  string kBPOSchemaPath = "/home/duane/workspace/project/src/cpp_modules/data/"
                          "bpo_schema.json";
  
};

TEST_F(GamePointsTest, InitFromJson) {

}
