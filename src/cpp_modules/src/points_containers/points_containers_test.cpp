#include <common.hpp>
#include <gtest/gtest.h>
#include <json_interface.hpp>
#include <json_io.hpp>
#include <points_containers.hpp>

using namespace std;
using namespace points_containers;

namespace points_containers_test {

// 
GamePointsSMap_t import_game_points_smap(string file_path) {
  json_io::NlohmannJsonIO json;
  json_interface::JsonIO & json_interface = json;
  GamePointsSMap_t game_points{};
  json_interface.Import(game_points, file_path);
  return game_points;
}


class PieceContainersTest : public ::testing::Test {
protected:
  string kICGARawPath = "/home/duane/workspace/project/src/cpp_modules/data/"
                        "ICGA_2004_raw.json";
  string kICGABPOPath = "/home/duane/workspace/project/src/cpp_modules/data/"
                        "ICGA_2004_bpo.json";
  json_io::NlohmannJsonIO json;
  json_interface::JsonIO &json_interface = json;
  GamePointsSMap_t game_points_smap = import_game_points_smap(kICGARawPath);
  TeamPointsSMap_t black_points_smap = game_points_smap.at("black");
  TeamPointsSMap_t red_points_smap = game_points_smap.at("red");
};

TEST_F(PieceContainersTest, TeamPointsInitFromSmap) {
  auto black_team_points = TeamPoints(black_points_smap);
  auto red_team_points = TeamPoints(red_points_smap);
}

TEST_F(PieceContainersTest, TeamPointsToArray) {
  auto black_team_points = TeamPoints(black_points_smap);
  auto black_array = black_team_points.ToArray();
  auto red_team_points = TeamPoints(red_points_smap);
  auto red_array = red_team_points.ToArray();
}

TEST_F(PieceContainersTest, GamePointsInitFromSmap) {
  auto game_points_struct = points_containers::GamePoints(game_points_smap);
}

TEST_F(PieceContainersTest, GamePointsInitFromTeamPoints) {
  auto black_team_points = TeamPoints(black_points_smap);
  auto red_team_points = TeamPoints(red_points_smap);
  auto game_points = GamePoints(black_team_points, red_team_points);
}

TEST_F(PieceContainersTest, GamePointsToArray) {
  // auto game_points_map = json_interface.Import(kICGARawPath);
  auto game_points_struct = points_containers::GamePoints(game_points_smap);
  auto result = game_points_struct.ToArray();
}

// TEST_F(PieceContainersTest, PieceBasePointsInitFromSmap) {
//   auto bpo_spec_map = json_interface.Import(kICGABPOPath);
//   BasePointsSMap_t black_base = bpo_spec_map.at("black_base");
//   auto black_base_points_struct = PieceBasePoints(black_base);
//   BasePointsSMap_t red_base_offsets = bpo_spec_map.at("red_base_offsets");
//   auto red_base_offsets_struct = PieceBasePoints(red_base_offsets);
// }

// TEST_F(PieceContainersTest, TeamPointsFromBPOPositionVals) {
//   auto bpo_spec_map = json_interface.Import(kICGABPOPath);
//   TeamPoints black_position = TeamPoints()
// }

} // end NAMESPACE points_containers_test
