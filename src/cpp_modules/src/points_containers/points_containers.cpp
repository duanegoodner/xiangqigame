#include <common.hpp>
#include <points_containers.hpp>
#include <unordered_map>
#include <utility_functs.hpp>

using namespace std;

namespace points_containers {


TeamPoints::TeamPoints(TeamPointsSMap_t s_map) {
  set_piece_attributes<TeamPoints, PiecePointsArray_t>(*this, s_map);
}

TeamPointsArray_t TeamPoints::ToArray() {
  TeamPointsArray_t team_array{};
  auto name_map = team_level_struct_to_map<TeamPoints, PiecePointsArray_t>(*this);
  for (auto piece : name_map) {
    auto piece_index = kPieceTypeStringToEnum.at(piece.first);
    team_array[piece_index] = name_map.at(piece.first);
  }
  return team_array;
}

GamePoints::GamePoints(GamePointsSMap_t s_map) {
  set_color_attributes<GamePoints, TeamPoints, PiecePointsArray_t>(*this, s_map);
}

GamePoints::GamePoints(TeamPoints black_points, TeamPoints red_points)
    : black{black_points}
    , red{red_points} {}

unordered_map<string, TeamPoints> GamePoints::TeamPointsStructs() {
  unordered_map<string, TeamPoints> name_map{{"red", red}, {"black", black}};
  return name_map;
}

GamePointsArray_t GamePoints::ToArray() {
  GamePointsArray_t game_array{};
  auto map_of_team_points_structs =
      color_member_map<GamePoints, TeamPoints>(*this);
  for (auto color : map_of_team_points_structs) {
    game_array[get_zcolor_index(kPieceColorStringToEnum.at(color.first))] =
        color.second.ToArray();
  }
  return game_array;
}

PieceBasePoints::PieceBasePoints(BasePointsSMap_t base_points_map) {
  set_piece_attributes<PieceBasePoints, int>(*this, base_points_map);
} 

TeamPointsSMap_t BasePointsOffsetSpec::BlackNetSMap() {
  auto black_base_points_map =
      team_level_struct_to_map<PieceBasePoints, int>(black_base);
  auto black_position_points_map =
      team_level_struct_to_map<TeamPoints, PiecePointsArray_t>(black_position);
  TeamPointsSMap_t black_net_points{};
  for (auto piece : black_position_points_map) {
    black_net_points[piece.first] = utility_functs::array_plus_const(
        piece.second,
        black_base_points_map[piece.first]
    );
  }
  return black_net_points;
}

TeamPointsSMap_t BasePointsOffsetSpec::RedNetSMap(
    TeamPointsSMap_t black_net_smap
) {
  auto red_base_offsets_map =
      team_level_struct_to_map<PieceBasePoints, int>(red_base_offsets);
  auto red_position_offsets_map =
      team_level_struct_to_map<TeamPoints, PiecePointsArray_t>(red_position_offsets);
  TeamPointsSMap_t red_net_points{};
  for (auto piece : red_position_offsets_map) {
    auto red_net_offset = utility_functs::array_plus_const(
        red_position_offsets_map[piece.first],
        red_base_offsets_map[piece.first]
    );
    auto red_net_points_pre_flip = utility_functs::two_array_sum(
        red_net_offset,
        black_net_smap[piece.first]
    );
    red_net_points[piece.first] =
        utility_functs::vertical_flip_array(red_net_points_pre_flip);
  }

  return red_net_points;
}

GamePoints BasePointsOffsetSpec::ToGamePoints() {

  auto black_net_points = BlackNetSMap();
  auto red_net_points = RedNetSMap(black_net_points);
  GamePointsSMap_t game_points_smap{};
  game_points_smap["black"] = black_net_points;
  game_points_smap["red"] = red_net_points;
  return GamePoints(game_points_smap);
}
} // namespace points_containers