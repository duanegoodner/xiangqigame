#include "common.hpp"
#include "piece_points_spec.hpp"
#include <algorithm>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <nlohmann/json-schema.hpp>
#include <piece_position_points.hpp>
#include <pthread.h>
#include <unordered_map>
#include <utility_functs.hpp>

using namespace std;
using namespace gameboard;
using namespace piece_points;
using json = nlohmann::json;
using nlohmann::json_schema::json_validator;


PiecePositionPoints::PiecePositionPoints()
    : points_array{BPOPointsSKeys(kICGABPOPath).ToGamePointsArray()} {}

PiecePositionPoints::PiecePositionPoints(GamePointsArray_t game_points_array)
    : points_array{game_points_array} {}

PiecePositionPoints::PiecePositionPoints(BPOPointsEKeys& bpo_points_ekeys)
    : points_array{bpo_points_ekeys.ToGamePointsArray()} {}

PiecePositionPoints::PiecePositionPoints(BPOPointsSKeys& bpo_points_skeys)
    : points_array{bpo_points_skeys.ToGamePointsArray()} {}

PiecePositionPoints::PiecePositionPoints(string json_file)
    : points_array{BPOPointsSKeys(json_file).ToGamePointsArray()} {}

TeamPointsEMap_t team_array_to_emap(TeamPointsArray_t team_array) {
  TeamPointsEMap_t team_map;
  for (auto piece_idx = 0; piece_idx < kNumPieceTypeVals; piece_idx++) {
    team_map[static_cast<PieceType>(piece_idx)] = team_array[piece_idx];
  }
  return team_map;
}

GamePointsEMap_t game_points_array_to_emap(GamePointsArray_t game_array) {
  GamePointsEMap_t pts_map;
  for (auto zcolor_idx = 0; zcolor_idx < game_array.size(); zcolor_idx++) {
    pts_map[get_piece_color(zcolor_idx)] = team_array_to_emap(game_array[zcolor_idx]);
  }
  return pts_map;
}

GamePointsSMap_t game_points_array_to_smap(GamePointsArray_t game_array) {
  auto e_map = game_points_array_to_emap(game_array);
  return game_points_emap_to_smap(e_map);
}

GamePointsSMap_t game_points_emap_to_smap(GamePointsEMap_t e_map) {
  GamePointsSMap_t game_string_map;

  for (auto color : kPieceColorStringToEnum) {
    if (e_map.contains(color.second)) {
      game_string_map[color.first] = utility_functs::replace_keys_reverse(
          e_map[color.second],
          kPieceTypeStringToEnum
      );
    }
  }
  return game_string_map;
}

json PiecePositionPoints::ToJson() {
  json j;
  auto game_points_smap = game_points_array_to_smap(points_array);
  j = game_points_smap;

  return j;
}

void PiecePositionPoints::ToFile(string output_file) {
  json j = game_points_array_to_smap(points_array);
  utility_functs::export_json(j, output_file);
}

bool piece_points::json_matches_schema(const json &imported_data, const json &schema) {
  json_validator validator;

  try {
    validator.set_root_schema(schema);
  } catch (const exception &e) {
    cerr << "Schema validation failed: " << e.what() << endl;
    exit(1);
  }

  try {
    auto data_validation_result = validator.validate(imported_data);
    return true;
  } catch (const exception &e) {
    return false;
  }
}

// }
