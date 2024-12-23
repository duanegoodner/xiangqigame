#include <piecepoints/base_position_offset.hpp>
#include <piecepoints/piece_position_points_for_concepts.hpp>
#include <unordered_map>
#include <utilities/utility_functs.hpp>

namespace piecepoints {

// PiecePositionPointsForConcepts::PiecePositionPointsForConcepts()
//     : points_array{BPOPointsSKeys(kICGABPOPath).ToGamePointsArray()} {}

PiecePositionPointsForConcepts::PiecePositionPointsForConcepts(
    GamePointsArray_t &game_points_array
)
    : points_array{game_points_array} {}

std::shared_ptr<PiecePositionPointsForConcepts> PiecePositionPointsForConcepts::Create(
    std::string json_file
) {
  piecepoints::BPOPointsSKeys bpo_points_skeys{json_file};
  auto game_points_array = bpo_points_skeys.ToGamePointsArray();
  return std::shared_ptr<PiecePositionPointsForConcepts>(
      new PiecePositionPointsForConcepts(game_points_array)
  );
}

TeamPointsEMap_t PiecePositionPointsForConcepts::TeamPointsArrayToEMap(
    TeamPointsArray_t team_array
) {
  TeamPointsEMap_t team_map;
  for (auto piece_idx = 0; piece_idx < gameboard::kNumPieceTypeVals; piece_idx++) {
    team_map[static_cast<gameboard::PieceType>(piece_idx)] = team_array[piece_idx];
  }
  return team_map;
}

GamePointsEMap_t PiecePositionPointsForConcepts::PointsArrayToEMap() {
  GamePointsEMap_t pts_map;
  for (auto zcolor_idx = 0; zcolor_idx < points_array.size(); zcolor_idx++) {
    pts_map[gameboard::GetPieceColorOf(zcolor_idx)] =
        TeamPointsArrayToEMap(points_array[zcolor_idx]);
  }
  return pts_map;
}

GamePointsSMap_t PiecePositionPointsForConcepts::PointsArrayToSmap() {
  auto e_map = PointsArrayToEMap();
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

} // namespace piecepoints
