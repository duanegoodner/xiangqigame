#include <common.hpp>
#include <points_containers.hpp>
#include <unordered_map>
#include <utility_functs.hpp>

using namespace std;

namespace points_containers {

TeamPoints::TeamPoints(TeamPointsSMap_t s_map)
    : null{s_map.at("null")}
    , general{s_map.at("general")}
    , advisor{s_map.at("advisor")}
    , elephant{s_map.at("elephant")}
    , horse{s_map.at("horse")}
    , chariot{s_map.at("chariot")}
    , cannon{s_map.at("cannon")}
    , soldier{s_map.at("soldier")} {}

unordered_map<string, PiecePointsArray_t> TeamPoints::PiecePointsArrays() {
  unordered_map<string, PiecePointsArray_t> name_map{
      {"null", null},
      {"general", general},
      {"advisor", advisor},
      {"elephant", elephant},
      {"horse", horse},
      {"chariot", chariot},
      {"cannon", cannon},
      {"soldier", soldier}};

  return name_map;
}

TeamPointsArray_t TeamPoints::ToArray() {
  TeamPointsArray_t team_array{};
  auto name_map = PiecePointsArrays();
  for (auto piece : name_map) {
    auto piece_index = kPieceTypeStringToEnum.at(piece.first);
    team_array[piece_index] = name_map.at(piece.first);
  }
  return team_array;
}

GamePoints::GamePoints(GamePointsSMap_t s_map)
    : black{TeamPoints(s_map.at("black"))}
    , red{TeamPoints(s_map.at("red"))} {}

GamePoints::GamePoints(TeamPoints black_points, TeamPoints red_points)
    : black{black_points}
    , red{red_points} {}
// GamePoints::GamePoints(string file_path) {}

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

TeamPointsSMap_t BasePointsOffsetSpec::BlackNetSMap() {
  auto black_base_points_map =
      piece_member_map<PieceBasePoints, int>(black_base);
  auto black_position_points_map =
      piece_member_map<TeamPoints, PiecePointsArray_t>(black_position);
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
      piece_member_map<PieceBasePoints, int>(red_base_offsets);
  auto red_position_offsets_map =
      piece_member_map<TeamPoints, PiecePointsArray_t>(red_position_offsets);
  TeamPointsSMap_t red_net_points{};
  for (auto piece : red_position_offsets_map) {
    auto red_net_offset = utility_functs::array_plus_const(
      red_position_offsets_map[piece.first],
      red_base_offsets_map[piece.first]
    );
    auto red_net_points_pre_flip = utility_functs::two_array_sum(
      red_net_offset, black_net_smap[piece.first]
    );
    red_net_points[piece.first] = utility_functs::vertical_flip_array(red_net_points_pre_flip);
  }

  return red_net_points;
}

GamePoints BasePointsOffsetSpec::ToGamePoints() {

  auto black_net_points = BlackNetSMap();
  auto red_net_points = RedNetSMap(black_net_points);
  return GamePoints(black_net_points, red_net_points);
}
} // namespace points_containers