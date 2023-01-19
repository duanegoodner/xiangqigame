#include "common.hpp"
#include "piece_points_spec.hpp"
#include <algorithm>
#include <iostream>
#include <piece_points.hpp>
#include <unordered_map>
#include <utility_functs.hpp>

using namespace std;
using namespace board_components;
using namespace piece_points;
using namespace piece_points_spec;
using json = nlohmann::json;

GamePointsArrayBuilder::GamePointsArrayBuilder(
    PointsSpecBPOInternal internal_points_spec
)
    : points_spec_{internal_points_spec} {}

GamePointsArrayBuilder::GamePointsArrayBuilder(
    PointsSpecBPOExternal external_points_spec
)
    : GamePointsArrayBuilder(PointsSpecBPOInternal(external_points_spec)) {}

GamePointsArrayBuilder::GamePointsArrayBuilder(string spec_file_path)
    : GamePointsArrayBuilder(PointsSpecBPOExternal(spec_file_path)) {}

TeamPointsArray_t GamePointsArrayBuilder::ComputeBlackNetPoints() {
  TeamPointsArray_t black_net_points{};
  for (auto piece : points_spec_.black_base) {

    black_net_points[piece.first] = utility_functs::array_plus_const(
        points_spec_.black_position[piece.first],
        points_spec_.black_base[piece.first]
    );
  }
  return black_net_points;
}

TeamPointsArray_t GamePointsArrayBuilder::ComputeRedNetPoints() {
  TeamPointsArray_t red_net_points{};
  for (auto piece : points_spec_.red_base_offsets) {
    auto base_points = points_spec_.black_base[piece.first] +
                       points_spec_.red_base_offsets[piece.first];

    auto unflipped_position_points = utility_functs::two_array_sum(
        points_spec_.black_position[piece.first],
        points_spec_.red_position_offsets[piece.first]
    );

    auto flipped_position_points =
        utility_functs::vertical_flip_array(unflipped_position_points);

    red_net_points[piece.first] =
        utility_functs::array_plus_const(flipped_position_points, base_points);
  }
  return red_net_points;
}

GamePointsArray_t GamePointsArrayBuilder::BuildGamePointsArray() {
  GamePointsArray_t game_points_array{};
  game_points_array[get_zcolor_index(PieceColor::kBlk)] =
      ComputeBlackNetPoints();
  game_points_array[get_zcolor_index(PieceColor::kRed)] =
      ComputeRedNetPoints();

  return game_points_array;
}

PiecePoints::PiecePoints()
    : points_array{DEFAULT_GAME_POINTS_ARRAY} {}
PiecePoints::PiecePoints(GamePointsArray_t game_points_array)
    : points_array{game_points_array} {}
PiecePoints::PiecePoints(PointsSpecBPOInternal internal_bpo_spec)
    : PiecePoints(
          GamePointsArrayBuilder(internal_bpo_spec).BuildGamePointsArray()
      ) {}
PiecePoints::PiecePoints(PointsSpecBPOExternal external_bpo_spec)
    : PiecePoints(
          GamePointsArrayBuilder(external_bpo_spec).BuildGamePointsArray()
      ) {}
PiecePoints::PiecePoints(GamePointsSMap_t s_map)
    : PiecePoints(game_points_smap_to_array(s_map)) {}
PiecePoints::PiecePoints(json& j) : PiecePoints(json_to_smap(j)) {}
PiecePoints::PiecePoints(string json_file) {
  auto json_object = utility_functs::import_json(json_file);
  auto points_smap = json_to_smap(json_object);
  points_array = game_points_smap_to_array(points_smap);

}

TeamPointsEMap_t piece_points::team_array_to_emap(TeamPointsArray_t team_array
) {
  TeamPointsEMap_t team_map;
  for (auto piece_idx = 0; piece_idx < kNumPieceTypeVals; piece_idx++) {
    team_map[static_cast<PieceType>(piece_idx)] = team_array[piece_idx];
  }
  return team_map;
}

GamePointsEMap_t piece_points::game_points_array_to_emap(
    GamePointsArray_t game_array
) {
  GamePointsEMap_t pts_map;
  for (auto zcolor_idx = 0; zcolor_idx < game_array.size(); zcolor_idx++) {
    pts_map[get_piece_color(zcolor_idx)] =
        team_array_to_emap(game_array[zcolor_idx]);
  }
  return pts_map;
}

GamePointsSMap_t piece_points::game_points_array_to_smap(
    GamePointsArray_t game_array
) {
  auto e_map = game_points_array_to_emap(game_array);
  return game_points_emap_to_smap(e_map);
}

GamePointsSMap_t piece_points::game_points_emap_to_smap(GamePointsEMap_t e_map
) {
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

GamePointsArray_t piece_points::game_points_smap_to_array(
    const GamePointsSMap_t s_map
) {
  GamePointsArray_t game_points_array{};
  for (auto color : s_map) {
    auto piece_color = kPieceColorStringToEnum.at(color.first);
    auto zcolor_index = get_zcolor_index(piece_color);

    for (auto piece_type : s_map.at(color.first)) {
      auto piece_type_index =
          static_cast<size_t>(kPieceTypeStringToEnum.at(piece_type.first));
      game_points_array[zcolor_index][piece_type_index] =
          s_map.at(color.first).at(piece_type.first);
    }
  }
  return game_points_array;
}

GamePointsSMap_t piece_points::json_to_smap(const json &j) {
  GamePointsSMap_t s_map;
  for (auto& item : j.items()) {
    s_map[item.key()] = item.value();
  }
  return s_map;
}

json PiecePoints::ToJson() {
  json j;
  auto game_points_smap = game_points_array_to_smap(points_array);
  j = game_points_smap;

  return j;
}

void PiecePoints::ToFile(string output_file) {
  json j = game_points_array_to_smap(points_array);
  utility_functs::export_json(j, output_file);
}

GamePointsArray_t raw_file_to_array(string raw_points_file) {
  auto json_object = utility_functs::import_json(raw_points_file);
  auto s_map = json_to_smap(json_object);
  return game_points_smap_to_array(s_map);
}

GamePointsArray_t bpo_file_to_array(string bpo_spec_file) {
  auto json_object = utility_functs::import_json(bpo_spec_file);
  
}
