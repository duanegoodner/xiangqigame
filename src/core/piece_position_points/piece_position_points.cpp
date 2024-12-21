//! @file piece_position_points.cpp
//! Implementation of piecepoints::PiecePositionPoints. 

#include <piecepoints/base_position_offset.hpp>
#include <piecepoints/piece_position_points.hpp>
#include <unordered_map>
#include <utility_functs.hpp>

using namespace std;
using namespace gameboard;
using namespace piecepoints;



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

TeamPointsEMap_t PiecePositionPoints::TeamPointsArrayToEMap(TeamPointsArray_t team_array) {
  TeamPointsEMap_t team_map;
  for (auto piece_idx = 0; piece_idx < kNumPieceTypeVals; piece_idx++) {
    team_map[static_cast<PieceType>(piece_idx)] = team_array[piece_idx];
  }
  return team_map;
}

GamePointsEMap_t PiecePositionPoints::PointsArraytoEMap() {
  GamePointsEMap_t pts_map;
  for (auto zcolor_idx = 0; zcolor_idx < points_array.size(); zcolor_idx++) {
    pts_map[GetPieceColorOf(zcolor_idx)] = TeamPointsArrayToEMap(points_array[zcolor_idx]);
  }
  return pts_map;
}

GamePointsSMap_t PiecePositionPoints::PointsArrayToSmap() {
  auto e_map = PointsArraytoEMap();
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
