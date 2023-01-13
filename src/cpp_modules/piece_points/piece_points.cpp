#include "common.hpp"
#include <algorithm>
#include <iostream>
#include <piece_points.hpp>
#include <unordered_map>
#include <utility_functs.hpp>

using namespace std;
using namespace board_components;
using namespace piece_points;
using json = nlohmann::json;

PointsSpecExternal::PointsSpecExternal(
    base_points_map_t black_base_input,
    base_points_map_t red_base_offsets_input,
    position_points_map_t black_position_input,
    position_points_map_t red_position_offsets_input
)
    : black_base{black_base_input}
    , red_base_offsets{red_base_offsets_input}
    , black_position{black_position_input}
    , red_position_offsets{red_position_offsets_input} {}

PointsSpecExternal::PointsSpecExternal(const json &json_object) {
  json_object.at("black_base").get_to(black_base);
  json_object.at("red_base_offsets").get_to(red_base_offsets);
  json_object.at("black_position").get_to(black_position);
  json_object.at("red_position_offsets").get_to(red_position_offsets);
}

PointsSpecExternal::PointsSpecExternal(string json_file_path)
    : PointsSpecExternal(utility_functs::import_json(json_file_path)){};

json PointsSpecExternal::ToJson() {
  json j;
  j["black_base"] = black_base;
  j["red_base_offsets"] = red_base_offsets;
  j["black_position"] = black_position;
  j["red_position_offsets"] = red_position_offsets;
  return j;
}

PointsSpecInternal::PointsSpecInternal(
    TeamBasePoints_t black_base_input,
    TeamBasePoints_t red_base_offsets_input,
    TeamPositionPoints_t black_position_input,
    TeamPositionPoints_t red_position_offsets_input
)
    : black_base{black_base_input}
    , red_base_offsets{red_base_offsets_input}
    , black_position{black_position_input}
    , red_position_offsets{red_position_offsets_input} {}

PointsSpecInternal::PointsSpecInternal(PointsSpecExternal external_spec) {
  unordered_map<string, PieceType> key_substitutions = {
      {"null", PieceType::kNnn},
      {"general", PieceType::kGen},
      {"advisor", PieceType::kAdv},
      {"elephant", PieceType::kEle},
      {"chariot", PieceType::kCha},
      {"horse", PieceType::kHor},
      {"cannon", PieceType::kCan},
      {"soldier", PieceType::kSol}};

  black_base = utility_functs::replace_keys(
      external_spec.black_base,
      key_substitutions
  );

  red_base_offsets = utility_functs::replace_keys(
      external_spec.red_base_offsets,
      key_substitutions
  );

  black_position = utility_functs::replace_keys(
      external_spec.black_position,
      key_substitutions
  );

  red_position_offsets = utility_functs::replace_keys(
      external_spec.red_position_offsets,
      key_substitutions
  );
}

PiecePositionPoints_t PiecePointsBuilder::ComputePieceNetPositionPoints(
    int base_val,
    PiecePositionPoints_t position_offsets
) {
  PiecePositionPoints_t net_points;
  for (auto rank = 0; rank < kNumRanks; rank++) {
    for (auto file = 0; file < kNumFiles; file++) {
      net_points[rank][file] = base_val + position_offsets[rank][file];
    }
  }
  return net_points;
}

TeamPositionPoints_t PiecePointsBuilder::ComputeBlackPositionPoints(
    TeamBasePoints_t black_base_points,
    TeamPositionPoints_t black_position_offsets
) {
  TeamPositionPoints_t black_team_points;

  for (auto piece : black_base_points) {
    black_team_points[piece.first] = ComputePieceNetPositionPoints(
        black_base_points[piece.first],
        black_position_offsets[piece.first]
    );
  }
  return black_team_points;
}

PiecePositionPoints_t PiecePointsBuilder::FlipBoardDirection(
    PiecePositionPoints_t orig_piece_pts
) {
  auto flipped_pts_array = orig_piece_pts;
  reverse(flipped_pts_array.begin(), flipped_pts_array.end());
  return flipped_pts_array;
}

TeamPositionPoints_t PiecePointsBuilder::ComputeRedPositionPoints(
    TeamPositionPoints_t black_team_points
) {
  TeamPositionPoints_t red_team_points = black_team_points;
  for (auto array : red_team_points) {
    red_team_points[array.first] =
        FlipBoardDirection(red_team_points[array.first]);
  }
  return red_team_points;
}

GamePositionPoints_t PiecePointsBuilder::BuildGamePositionPoints(
    TeamPositionPoints_t black_points
) {
  GamePositionPoints_t game_position_points;
  game_position_points[PieceColor::kBlk] = black_points;
  game_position_points[PieceColor::kRed] =
      ComputeRedPositionPoints(black_points);
  return game_position_points;
}

GamePositionPoints_t PiecePointsBuilder::BuildGamePositionPoints(
    TeamBasePoints_t black_base_points,
    TeamPositionPoints_t black_position_offsets
) {
  auto black_position_points =
      ComputeBlackPositionPoints(black_base_points, black_position_offsets);
  return BuildGamePositionPoints(black_position_points);
}
