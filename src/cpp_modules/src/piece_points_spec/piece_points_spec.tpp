#ifndef B3123E8C_B936_4802_A0CD_13BABD26E0A8
#define B3123E8C_B936_4802_A0CD_13BABD26E0A8

// #include <piece_points_spec.hpp>
#include <piece_points_spec.hpp>
#include <common.hpp>
#include <filesystem>
#include <iostream>
#include <nlohmann/json.hpp>
#include <unordered_map>
#include <json_internal.hpp>

namespace piece_points_spec {

// using namespace std;
using nloh_json = nlohmann::json;

template <>
inline TeamPoints<nloh_json>::TeamPoints(nloh_json &j) {
  j.at("null").get_to(null);
  j.at("general").get_to(general);
  j.at("advisor").get_to(advisor);
  j.at("elephant").get_to(elephant);
  j.at("horse").get_to(horse);
  j.at("chariot").get_to(chariot);
  j.at("cannon").get_to(cannon);
  j.at("soldier").get_to(soldier);
}

template <typename JsonType>
inline unordered_map<string, PiecePointsArray_t>
TeamPoints<JsonType>::PiecePointsArrays() {
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

template <>
inline TeamPointsArray_t TeamPoints<nloh_json>::ToArray() {
  TeamPointsArray_t team_array{};
  auto name_map = PiecePointsArrays();
  for (auto piece : name_map) {
    auto piece_index = kPieceTypeStringToEnum.at(piece.first);
    team_array[piece_index] = name_map.at(piece.first);
  }
  return team_array;
}

template <typename JsonType>
inline unordered_map<string, TeamPoints<JsonType>>
GamePoints<JsonType>::TeamPointsJsons() {
  unordered_map<string, TeamPoints<JsonType>> name_map{
      {"red", red},
      {"black", black}};

  return name_map;
}

template <>
inline GamePoints<nloh_json>::GamePoints(nloh_json& j)
    : black{TeamPoints<nloh_json>(j.at("black"))}
    , red{TeamPoints<nloh_json>(j.at("red"))} {}

template <>
inline GamePointsArray_t GamePoints<nloh_json>::ToArray() {
  GamePointsArray_t game_array{};
  auto name_map = TeamPointsJsons();
  for (auto color : name_map) {
    game_array[get_zcolor_index(kPieceColorStringToEnum.at(color.first))] =
        color.second.ToArray();
  }
  return game_array;
}

template <>
inline nloh_json TeamPoints<nloh_json>::ToJson() {
  auto name_map = PiecePointsArrays();
  nloh_json result = name_map;
  return result;
}

template<>
inline nloh_json GamePoints<nloh_json>::ToJson() {
    auto name_map = TeamPointsJsons();
    nloh_json j;
    for (auto color : name_map) {
        j[color.first] = color.second.ToJson();
    }
    return j;
}

template <typename JsonType>
inline void GamePoints<JsonType>::ToFile(string file_path) {
    auto json_object = ToJson();
    json_internal::export_json<JsonType>(json_object, file_path);
}
} // namespace piece_points_spec

#endif /* B3123E8C_B936_4802_A0CD_13BABD26E0A8 */
