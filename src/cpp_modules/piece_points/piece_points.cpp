#include "common.hpp"
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

PiecePositionPoints_t GamePointsArrayBuilder::FlipBoardDirection(
    PiecePositionPoints_t orig_piece_pts
) {
  auto flipped_pts_array = orig_piece_pts;
  reverse(flipped_pts_array.begin(), flipped_pts_array.end());
  return flipped_pts_array;
}

PiecePositionPoints_t GamePointsArrayBuilder::PiecePointsArraySum(
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

PiecePositionPoints_t GamePointsArrayBuilder::ComputePieceNetPoints(
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

TeamPointsArray_t GamePointsArrayBuilder::ComputeBlackNetPoints() {
  TeamPointsArray_t black_net_points{};
  for (auto piece : points_spec_.black_base) {
    black_net_points[piece.first] = ComputePieceNetPoints(
        points_spec_.black_base[piece.first],
        points_spec_.black_position[piece.first]
    );
  }
  return black_net_points;
}

TeamPointsArray_t GamePointsArrayBuilder::ComputeRedNetPoints() {
  TeamPointsArray_t red_net_points{};
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

GamePointsArray_t GamePointsArrayBuilder::BuildGamePointsArray() {
  GamePointsArray_t game_points_array{};
  game_points_array[get_zcolor_index(PieceColor::kBlk)] = ComputeBlackNetPoints();
  game_points_array[get_zcolor_index(PieceColor::kRed)] = ComputeRedNetPoints();

  return game_points_array;
}

PiecePoints::PiecePoints()
: points_array{DEFAULT_GAME_POINTS_ARRAY} {}
PiecePoints::PiecePoints(GamePointsArray_t game_points_array) 
: points_array{game_points_array} {}


TeamPointsEMap_t piece_points::team_array_to_map(
    TeamPointsArray_t team_array
) {
  TeamPointsEMap_t team_map;
  for (auto piece_idx = 0; piece_idx < kNumPieceTypeVals; piece_idx++) {
    team_map[static_cast<PieceType>(piece_idx)] = team_array[piece_idx];
  }
  return team_map;
}

GamePointsEMap_t piece_points::game_points_array_to_map(
    GamePointsArray_t game_array
) {
  GamePointsEMap_t pts_map;
  for (auto zcolor_idx = 0; zcolor_idx < game_array.size(); zcolor_idx++) {
    pts_map[get_piece_color(zcolor_idx)] =
        team_array_to_map(game_array[zcolor_idx]);
  }
  return pts_map;
}

GamePointsSMap_t piece_points::game_points_emap_to_smap(GamePointsEMap_t e_map
) {
  GamePointsSMap_t game_string_map;

  for (auto color : kPieceColorStringToEnum) {
    game_string_map[color.first] = utility_functs::replace_keys_reverse(
        e_map[color.second],
        kPieceTypeStringToEnum
    );
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


