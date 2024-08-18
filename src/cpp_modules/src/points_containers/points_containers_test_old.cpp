#include <common.hpp>
#include <gtest/gtest.h>
#include <json_interface.hpp>
#include <json_io.hpp>
#include <points_containers.hpp>
#include <utility_functs.hpp>

using namespace std;
using namespace points_containers;

class GamePointsTest : public ::testing::Test {
protected:

  const string kICGARawPath = utility_functs::get_data_file_abs_path("ICGA_2004_raw.json");

  json_io::NlohmannJsonIO json;
  json_interface::JsonIO &json_interface = json;

  GamePointsSMap_t ImportGamePointsSMap() {
    GamePointsSMap_t game_points{};
    json_interface.Import(game_points, kICGARawPath);
    return game_points;
  }

  GamePointsSMap_t game_points_smap = ImportGamePointsSMap();
  TeamPointsSMap_t black_points_smap = game_points_smap.at("black");
  TeamPointsSMap_t red_points_smap = game_points_smap.at("red");
};

TEST_F(GamePointsTest, GamePointsInitFromSmap) {
  auto game_points_struct = points_containers::GamePoints(game_points_smap);
}

TEST_F(GamePointsTest, GamePointsInitFromTeamPoints) {
  auto black_team_points = TeamPoints(black_points_smap);
  auto red_team_points = TeamPoints(red_points_smap);
  auto game_points = GamePoints(black_team_points, red_team_points);
}

TEST_F(GamePointsTest, GamePointsToArray) {
  // auto game_points_map = json_interface.Import(kICGARawPath);
  auto game_points_struct = points_containers::GamePoints(game_points_smap);
  auto result = game_points_struct.ToArray();
}

TEST_F(GamePointsTest, GamePointsInitFromArray) {
  auto game_points_struct = points_containers::GamePoints(game_points_smap);
  auto game_points_array = game_points_struct.ToArray();
  auto reconstructed_game_points = GamePoints(game_points_array);
}

class BasePointsOffsetSpecTest : public ::testing::Test {
protected:
  string kICGABPOPath = utility_functs::get_data_file_abs_path("ICGA_2004_bpo.json"); 
  json_io::NlohmannJsonIO json;
  json_interface::JsonIO &json_interface = json;

  BPOSpecSMap_t ImportBPOSpecSMap() {
    BPOSpecSMap_t bpo_spec{};
    json_interface.Import(bpo_spec, kICGABPOPath);
    return bpo_spec;
  }

  BPOSpecSMap_t bpo_spec_map = ImportBPOSpecSMap();
};

TEST_F(BasePointsOffsetSpecTest, BPOSpecInitFromSmap) {
  auto bpo_spec = BasePointsOffsetSpec(bpo_spec_map);
}

TEST_F(BasePointsOffsetSpecTest, BPOToGamePoints) {
  auto bpo_spec = BasePointsOffsetSpec(bpo_spec_map);
  auto result = bpo_spec.ToGamePoints();
}

TEST_F(BasePointsOffsetSpecTest, BPOToMap) {
  auto bpo_spec = BasePointsOffsetSpec(bpo_spec_map);
  auto result = bpo_spec.ToMap();
}

// TEST_F(GamePointsTest, TeamPointsInitFromSmap) {
//   auto black_team_points = TeamPoints(black_points_smap);
//   auto red_team_points = TeamPoints(red_points_smap);
// }

// TEST_F(GamePointsTest, TeamPointsToArray) {
//   auto black_team_points = TeamPoints(black_points_smap);
//   auto black_array = black_team_points.ToArray();
//   auto red_team_points = TeamPoints(red_points_smap);
//   auto red_array = red_team_points.ToArray();
// }

// TEST_F(GamePointsTest, TeamPointsToMap) {
//   auto black_team_points = TeamPoints(black_points_smap);
//   auto black_map = black_team_points.ToMap();
//   auto red_team_points = TeamPoints(red_points_smap);
//   auto red_map = red_team_points.ToMap();
// }

// TEST_F(GamePointsTest, TeamPointsFromArray) {
//   auto black_team_points = TeamPoints(black_points_smap);
//   auto black_array = black_team_points.ToArray();
//   auto red_team_points = TeamPoints(red_points_smap);
//   auto red_array = red_team_points.ToArray();
//   auto black_team_points_reconstructed = TeamPoints(black_array);
//   auto red_team_points_reconstructed = TeamPoints(red_array);
// }
