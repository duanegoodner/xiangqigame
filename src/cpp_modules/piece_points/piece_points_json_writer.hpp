#ifndef _PIECE_POINTS_JSON_WRITER
#define _PIECE_POINTS_JSON_WRITER

#include <array>
#include <common.hpp>
#include <json.hpp>
#include <piece_points.hpp>
#include <string>
#include <unordered_map>

using namespace std;
using namespace piece_points;
using json = nlohmann::json;

namespace piece_points_json {

struct PointsSpec {
  base_points_map_t black_base;
  base_points_map_t red_base_offsets;
  position_points_map_t black_position;
  position_points_map_t red_position_offsets;
  json ToJson();
};

} // namespace piece_points_json

#endif