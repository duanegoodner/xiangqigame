#include <common.hpp>
#include <points_containers.hpp>
#include <unordered_map>
#include <utility_functs.hpp>

using namespace std;

namespace points_containers {

TeamPoints::TeamPoints(TeamPointsSMap_t s_map) {
  set_piece_attributes<TeamPoints, PiecePointsArray_t>(*this, s_map);
}

TeamPoints::TeamPoints(TeamPointsArray_t team_points_array)
    : null{team_points_array[kPieceTypeStringToEnum.at("null")]}
    , general{team_points_array[kPieceTypeStringToEnum.at("general")]}
    , advisor{team_points_array[kPieceTypeStringToEnum.at("advisor")]}
    , elephant{team_points_array[kPieceTypeStringToEnum.at("elephant")]}
    , chariot{team_points_array[kPieceTypeStringToEnum.at("chariot")]}
    , horse{team_points_array[kPieceTypeStringToEnum.at("horse")]}
    , cannon{team_points_array[kPieceTypeStringToEnum.at("cannon")]}
    , soldier{team_points_array[kPieceTypeStringToEnum.at("soldier")]} {}

TeamPointsArray_t TeamPoints::ToArray() {
  TeamPointsArray_t team_array{};
  auto name_map =
      team_level_struct_to_map<TeamPoints, PiecePointsArray_t>(*this);
  for (auto piece : name_map) {
    auto piece_index = kPieceTypeStringToEnum.at(piece.first);
    team_array[piece_index] = name_map.at(piece.first);
  }
  return team_array;
}

TeamPointsSMap_t TeamPoints::ToMap() {
  return team_level_struct_to_map<TeamPoints, PiecePointsArray_t>(*this);
}

GamePoints::GamePoints(GamePointsSMap_t s_map) {
  set_color_attributes<GamePoints, TeamPoints, PiecePointsArray_t>(
      *this,
      s_map
  );
}

GamePoints::GamePoints(TeamPoints black_points, TeamPoints red_points)
    : black{black_points}
    , red{red_points} {}

GamePoints::GamePoints(GamePointsArray_t game_points_array)
    : red{TeamPoints(game_points_array[get_zcolor_index(PieceColor::kRed)])}
    , black{
          TeamPoints(game_points_array[get_zcolor_index(PieceColor::kBlk)])} {}

GamePointsArray_t GamePoints::ToArray() {
  GamePointsArray_t game_array{};
  auto map_of_team_points_structs =
      game_level_struct_to_map<GamePoints, TeamPoints>(*this);
  for (auto color : map_of_team_points_structs) {
    game_array[get_zcolor_index(kPieceColorStringToEnum.at(color.first))] =
        color.second.ToArray();
  }
  return game_array;
}

GamePointsSMap_t GamePoints::ToMap() {
  auto map_of_team_points_structs =
      game_level_struct_to_map<GamePoints, TeamPoints>(*this);
  GamePointsSMap_t game_points_map{};
  for (auto color : map_of_team_points_structs) {
    game_points_map[color.first] =
        team_level_struct_to_map<TeamPoints, PiecePointsArray_t>(color.second);
  }
  return game_points_map;
}

TeamBasePositionPair::TeamBasePositionPair(
    BasePointsSMap_t base_points_map,
    TeamPointsSMap_t team_points_map
)
    : piece_base_points(PieceBasePoints(base_points_map))
    , position_points{TeamPoints(team_points_map)} {}

TeamBasePositionPair::TeamBasePositionPair(
    PieceBasePoints base,
    TeamPoints position
)
    : piece_base_points{base}
    , position_points{position} {}

BasePositionSMapPair_t TeamBasePositionPair::ToPairOfMaps() {
  BasePositionSMapPair_t map_pair{};
  map_pair.first = piece_base_points.ToMap();
  map_pair.second = position_points.ToMap();
  return map_pair;
}

TeamPoints TeamBasePositionPair::NetPoints() {
  BasePointsSMap_t base_points_map =
      team_level_struct_to_map<PieceBasePoints, int>(piece_base_points);
  TeamPointsSMap_t team_points_map =
      team_level_struct_to_map<TeamPoints, PiecePointsArray_t>(position_points
      );
  TeamPointsSMap_t net_points_map{};
  for (auto piece : base_points_map) {
    net_points_map[piece.first] = utility_functs::array_plus_const(
      team_points_map[piece.first],
      base_points_map[piece.first]
    );
  }
  return TeamPoints(net_points_map);
}

PieceBasePoints::PieceBasePoints(BasePointsSMap_t base_points_map) {
  set_piece_attributes<PieceBasePoints, int>(*this, base_points_map);
}

BasePointsSMap_t PieceBasePoints::ToMap() {
  return team_level_struct_to_map<PieceBasePoints, int>(*this);
}

BasePointsOffsetSpec::BasePointsOffsetSpec(BPOSpecSMap_t s_map)
    : black{TeamBasePositionPair(s_map["black"].first, s_map["black"].second)}
    , red{TeamBasePositionPair(s_map["red"].first, s_map["black"].second)} {}

BPOSpecSMap_t BasePointsOffsetSpec::ToMap() {
  BPOSpecSMap_t bpo_spec_map{};
  bpo_spec_map["black"] = black.ToPairOfMaps();
  bpo_spec_map["red"] = red.ToPairOfMaps();
  // bpo_spec_map["black"] = {black_base.ToMap(), black_position.ToMap()};
  // bpo_spec_map["red"] = {
  //     red_base_offsets.ToMap(),
  //     red_position_offsets.ToMap()};

  return bpo_spec_map;
}

TeamPoints BasePointsOffsetSpec::BlackNetPoints() {
  return black.NetPoints();

  // BasePositionSMapPair_t black_base_offset = black.ToPairOfMaps();
  // auto black_base_points_map = black_base_offset.first;
  // auto black_position_points_map = black_base_offset.second;
  // TeamPointsSMap_t black_net_points{};
  // for (auto piece : black_position_points_map) {
  //   black_net_points[piece.first] = utility_functs::array_plus_const(
  //       piece.second,
  //       black_base_points_map[piece.first]
  //   );
  // }
  // return TeamPoints(black_net_points);
}

TeamPoints BasePointsOffsetSpec::RedNetPoints() {
  // BasePositionSMapPair_t red_base_offset = red.ToPairOfMaps();
  auto black_net_map = BlackNetPoints().ToMap();
  // auto red_base_map = red_base_offset.first;
  // auto red_position_map = red_base_offset.second;

  auto red_net_offset_map = red.NetPoints().ToMap();

  TeamPointsSMap_t red_net_points{};
  for (auto piece : red_net_offset_map) {
    // auto red_net_offset = utility_functs::array_plus_const(
    //     red_position_map[piece.first],
    //     red_base_map[piece.first]
    // );

    auto red_net_points_pre_flip = utility_functs::two_array_sum(
        red_net_offset_map[piece.first],
        black_net_map[piece.first]
    );
    red_net_points[piece.first] =
        utility_functs::vertical_flip_array(red_net_points_pre_flip);
  }

  return TeamPoints(red_net_points);
}

GamePoints BasePointsOffsetSpec::ToGamePoints() {
  return GamePoints(BlackNetPoints(), RedNetPoints());
}
} // namespace points_containers