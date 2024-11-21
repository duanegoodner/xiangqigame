#include <piece_points_bpo.hpp>
#include <piece_position_points_new.hpp>
#include <unordered_map>
#include <utility_functs.hpp>


namespace piecepoints {

PiecePositionPointsNew::PiecePositionPointsNew()
    : points_array{BPOPointsSKeys(kICGABPOPath).ToGamePointsArray()} {}

PiecePositionPointsNew::PiecePositionPointsNew(GamePointsArray_t game_points_array)
    : points_array{game_points_array} {}

PiecePositionPointsNew::PiecePositionPointsNew(BPOPointsEKeys& bpo_points_ekeys)
    : points_array{bpo_points_ekeys.ToGamePointsArray()} {}

PiecePositionPointsNew::PiecePositionPointsNew(BPOPointsSKeys& bpo_points_skeys)
    : points_array{bpo_points_skeys.ToGamePointsArray()} {}

PiecePositionPointsNew::PiecePositionPointsNew(string json_file)
    : points_array{BPOPointsSKeys(json_file).ToGamePointsArray()} {}

TeamPointsEMap_t PiecePositionPointsNew::TeamPointsArrayToEMap(TeamPointsArray_t team_array) {
  TeamPointsEMap_t team_map;
  for (auto piece_idx = 0; piece_idx < gameboard::kNumPieceTypeVals; piece_idx++) {
    team_map[static_cast<gameboard::PieceType>(piece_idx)] = team_array[piece_idx];
  }
  return team_map;
}

GamePointsEMap_t PiecePositionPointsNew::PointsArraytoEMap() {
  GamePointsEMap_t pts_map;
  for (auto zcolor_idx = 0; zcolor_idx < points_array.size(); zcolor_idx++) {
    pts_map[gameboard::GetPieceColorOf(zcolor_idx)] = TeamPointsArrayToEMap(points_array[zcolor_idx]);
  }
  return pts_map;
}

GamePointsSMap_t PiecePositionPointsNew::PointsArrayToSmap() {
  auto e_map = PointsArraytoEMap();
  GamePointsSMap_t game_string_map;

  for (auto color : gameboard::kPieceColorStringToEnum) {
    if (e_map.contains(color.second)) {
      game_string_map[color.first] = utility_functs::replace_keys_reverse(
          e_map[color.second],
          gameboard::kPieceTypeStringToEnum
      );
    }
  }
  return game_string_map;
}

}


