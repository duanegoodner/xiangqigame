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

PointsSpecBPOExternal::PointsSpecBPOExternal(
    base_points_map_t black_base_input,
    base_points_map_t red_base_offsets_input,
    position_points_map_t black_position_input,
    position_points_map_t red_position_offsets_input
)
    : black_base{black_base_input}
    , red_base_offsets{red_base_offsets_input}
    , black_position{black_position_input}
    , red_position_offsets{red_position_offsets_input} {}

PointsSpecBPOExternal::PointsSpecBPOExternal(const json &json_object) {
  json_object.at("black_base").get_to(black_base);
  json_object.at("red_base_offsets").get_to(red_base_offsets);
  json_object.at("black_position").get_to(black_position);
  json_object.at("red_position_offsets").get_to(red_position_offsets);
}

PointsSpecBPOExternal::PointsSpecBPOExternal(string json_file_path)
    : PointsSpecBPOExternal(utility_functs::import_json(json_file_path)){};

json PointsSpecBPOExternal::ToJson() {
  json j;
  j["black_base"] = black_base;
  j["red_base_offsets"] = red_base_offsets;
  j["black_position"] = black_position;
  j["red_position_offsets"] = red_position_offsets;
  return j;
}

void PointsSpecBPOExternal::ToFile(string output_path) {
  auto json_object = ToJson();
  utility_functs::export_json(json_object, output_path);
}

PointsSpecBPOInternal::PointsSpecBPOInternal(
    TeamBasePoints_t black_base_input,
    TeamBasePoints_t red_base_offsets_input,
    TeamPositionPoints_t black_position_input,
    TeamPositionPoints_t red_position_offsets_input
)
    : black_base{black_base_input}
    , red_base_offsets{red_base_offsets_input}
    , black_position{black_position_input}
    , red_position_offsets{red_position_offsets_input} {}

PointsSpecBPOInternal::PointsSpecBPOInternal(PointsSpecBPOExternal external_spec) {
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

PiecePointsBuilder::PiecePointsBuilder(PointsSpecBPOInternal internal_points_spec)
    : points_spec_{internal_points_spec} {}

PiecePointsBuilder::PiecePointsBuilder(PointsSpecBPOExternal external_points_spec)
    : PiecePointsBuilder(PointsSpecBPOInternal(external_points_spec)) {}

PiecePointsBuilder::PiecePointsBuilder(string spec_file_path)
    : PiecePointsBuilder(PointsSpecBPOExternal(spec_file_path)) {}

PiecePositionPoints_t PiecePointsBuilder::ComputePieceNetPoints(
    Points_t base,
    PiecePositionPoints_t position_points
) {
  PiecePositionPoints_t net_points;
  for (auto rank = 0; rank < kNumRanks; rank++) {
    for (auto file = 0; file < kNumFiles; file++) {
      net_points[rank][file] = base + position_points[rank][file];
    }
  }
  return net_points;
}

PiecePositionPoints_t PiecePointsBuilder::FlipBoardDirection(
    PiecePositionPoints_t orig_piece_pts
) {
  auto flipped_pts_array = orig_piece_pts;
  reverse(flipped_pts_array.begin(), flipped_pts_array.end());
  return flipped_pts_array;
}

TeamPositionPoints_t PiecePointsBuilder::ComputeBlackNetPoints() {
  TeamPositionPoints_t black_net_points{};
  for (auto piece : points_spec_.black_base) {
    black_net_points[piece.first] = ComputePieceNetPoints(
        points_spec_.black_base[piece.first],
        points_spec_.black_position[piece.first]
    );
  }

  return black_net_points;
}

PiecePositionPoints_t PiecePointsBuilder::PiecePointsArraySum(
    PiecePositionPoints_t a,
    PiecePositionPoints_t b
) {
  PiecePositionPoints_t result{};
  for (auto rank = 0; rank < kNumRanks; rank++) {
    for (auto file = 0; file < kNumFiles; file++) {
      result[rank][file] = a[rank][file] + b[rank][file];
    }
  }
  return result;
}

TeamPositionPoints_t PiecePointsBuilder::ComputeRedNetPoints() {
  TeamPositionPoints_t red_net_points{};
  for (auto piece : points_spec_.red_base_offsets) {
    auto base_points = points_spec_.black_base[piece.first] +
                       points_spec_.red_base_offsets[piece.first];
    auto unflipped_position_points = PiecePointsArraySum(
        points_spec_.black_position[piece.first],
        points_spec_.red_position_offsets[piece.first]
    );
    auto flipped_position_points =
        FlipBoardDirection(unflipped_position_points);
    red_net_points[piece.first] =
        ComputePieceNetPoints(base_points, flipped_position_points);
  }
  return red_net_points;
}

GamePositionPoints_t PiecePointsBuilder::BuildGamePositionPoints() {
  GamePositionPoints_t game_position_points{};
  game_position_points[PieceColor::kBlk] = ComputeBlackNetPoints();
  game_position_points[PieceColor::kRed] = ComputeRedNetPoints();
  return game_position_points;
}
