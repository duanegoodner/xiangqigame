#include <common.hpp>
#include <gtest/gtest.h>
#include <json_interface.hpp>
#include <json_io.hpp>
#include <points_containers.hpp>

using namespace std;

class GamePointsTest : public ::testing::Test {
protected:
  string kICGARawPath = "/home/duane/workspace/project/src/cpp_modules/data/"
                        "ICGA_2004_raw.json";
};

TEST_F(GamePointsTest, InitFromSmap) {
  json_io::NlohmannJsonIO json;
  json_interface::JsonIO &json_interface = json;
  auto game_points_map = json_interface.Import(kICGARawPath);
  auto game_points_struct = points_containers::GamePoints(game_points_map);
}

TEST_F(GamePointsTest, ToArray) {
  json_io::NlohmannJsonIO json;
  json_interface::JsonIO &json_interface = json;
  auto game_points_map = json_interface.Import(kICGARawPath);
  auto game_points_struct = points_containers::GamePoints(game_points_map);
  auto result = game_points_struct.ToArray();
}
