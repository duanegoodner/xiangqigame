#ifndef ABF79E5F_F2D2_421C_80C6_8D1E0141EEC7
#define ABF79E5F_F2D2_421C_80C6_8D1E0141EEC7

#include <common.hpp>
#include <unordered_map>

namespace points_containers {

/*
struct w/ piece data members to map with piece name strings as keys
 */
template <typename ContainerType, typename PieceDataType>
unordered_map<string, PieceDataType> piece_member_map(ContainerType &container
) {
  unordered_map<string, PieceDataType> piece_values = {
      {"null", container.null},
      {"general", container.general},
      {"soldier", container.soldier},
      {"advisor", container.advisor},
      {"elephant", container.elephant},
      {"horse", container.horse},
      {"chariot", container.chariot},
      {"cannon", container.cannon}};
  return piece_values;
}

/*
map of piece-wise data to struct
 */
template <typename ContainerType, typename PieceDataType>
ContainerType piece_member_attrs(
    unordered_map<string, PieceDataType> piece_member_map
) {
  ContainerType team_level_struct{};
  team_level_struct.null = piece_member_map.at("null");
  team_level_struct.general = piece_member_map.at("general");
  team_level_struct.soldier = piece_member_map.at("soldier");
  team_level_struct.advisor = piece_member_map.at("advisor");
  team_level_struct.elephant = piece_member_map.at("elephant");
  team_level_struct.horse = piece_member_map.at("horse");
  team_level_struct.chariot = piece_member_map.at("chariot");
  team_level_struct.cannon = piece_member_map.at("cannon");
  return team_level_struct;
}

/*
struct w/ color data members to map with color name strings as keys
 */
template <typename ContainerType, typename ColorDataType>
unordered_map<string, ColorDataType> color_member_map(ContainerType &container
) {
  unordered_map<string, ColorDataType> color_values = {
      {"red", container.red},
      {"black", container.black}};
  return color_values;
}

struct TeamPoints {
  TeamPoints() = default;
  TeamPoints(TeamPointsSMap_t s_map);
  unordered_map<string, PiecePointsArray_t> PiecePointsArrays();
  TeamPointsArray_t ToArray();

  PiecePointsArray_t null;
  PiecePointsArray_t general;
  PiecePointsArray_t advisor;
  PiecePointsArray_t elephant;
  PiecePointsArray_t horse;
  PiecePointsArray_t chariot;
  PiecePointsArray_t cannon;
  PiecePointsArray_t soldier;
};

struct GamePoints {
  GamePoints() = default;
  GamePoints(TeamPoints black_points, TeamPoints red_points);
  GamePoints(GamePointsSMap_t s_map);
  GamePoints(string file_path);

  unordered_map<string, TeamPoints> TeamPointsStructs();
  GamePointsArray_t ToArray();
  void ToFile(string file_path);

  TeamPoints red;
  TeamPoints black;
};

struct PieceBasePoints {
  int advisor;
  int cannon;
  int chariot;
  int elephant;
  int general;
  int horse;
  int null;
  int soldier;
};

struct BasePointsOffsetSpec {
  BasePointsOffsetSpec() = default;

  PieceBasePoints black_base;
  TeamPoints black_position;
  PieceBasePoints red_base_offsets;
  TeamPoints red_position_offsets;

  GamePoints ToGamePoints();

  private:
  TeamPointsSMap_t BlackNetSMap();
  TeamPointsSMap_t RedNetSMap(TeamPointsSMap_t black_net_smap);
};

} // namespace points_containers

#endif /* ABF79E5F_F2D2_421C_80C6_8D1E0141EEC7 */
