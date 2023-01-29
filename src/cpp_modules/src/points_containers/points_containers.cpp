#include <common.hpp>
#include <points_containers.hpp>
#include <unordered_map>

using namespace std;

namespace points_containers {

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

unordered_map<string, TeamPoints> GamePoints::TeamPointsStructs() {
  unordered_map<string, TeamPoints> name_map{{"red", red}, {"black", black}};
  return name_map;
}

GamePointsArray_t GamePoints::ToArray() {
  GamePointsArray_t game_array{};
  auto name_map = TeamPointsStructs();
  for (auto color : name_map) {
    game_array[get_zcolor_index(kPieceColorStringToEnum.at(color.first))] =
        color.second.ToArray();
  }
  return game_array;
}

} // namespace points_containers