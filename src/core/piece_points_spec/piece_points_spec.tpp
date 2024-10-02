// Filename: piece_points_spec.tpp
// Author: Duane Goodner
// Created: 2022-12-15
// Last Modified: 2024-08-16

// Description:
// Implementations of template classes that support PiecePointsSpec

#ifndef B3123E8C_B936_4802_A0CD_13BABD26E0A8
#define B3123E8C_B936_4802_A0CD_13BABD26E0A8

// #include <piece_points_spec.hpp>
// #include "json_internal.tpp"
#include <common.hpp>
#include <filesystem>
#include <iostream>
#include <json_internal.hpp>
#include <nlohmann/json.hpp>
#include <piece_points_spec.hpp>
#include <unordered_map>

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

// inline TeamPointsArray_t TeamPointsNonTemp::ToArray() {
//   TeamPointsArray_t team_array{};
//   auto name_map = PiecePointsArrays();
//   for (auto piece : name_map) {
//     auto piece_index = kPieceTypeStringToEnum.at(piece.first);
//     team_array[piece_index] = name_map.at(piece.first);
//   }
//   return team_array;
// }

// inline unordered_map<string, TeamPointsNonTemp>
// GamePointsNonTemp::TeamPointsStructs() {
//   unordered_map<string, TeamPointsNonTemp> name_map{
//       {"red", red},
//       {"black", black}};
//   return name_map;
// }

template <typename JsonType>
inline BaseOffsetPointSpec<JsonType>::BaseOffsetPointSpec(string file_path)
    : BaseOffsetPointSpec(json_internal::import_json<JsonType>(file_path)) {}

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

template <>
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

template <>
inline bool game_points_struct_match_json<nloh_json>(
    GamePoints<nloh_json> &game_points,
    nloh_json &j
) {

  // TODO: Validate json format (but need validator for nloh_json)

  for (auto &[color_key, color_value] : j.items()) {
    for (auto &[piece_type_key, piece_type_value] : color_value.items()) {
      PiecePointsArray_t array_from_json = j.at(color_key).at(piece_type_key);
      
      if (!(array_from_json ==
          game_points.TeamPointsJsons().at(color_key).PiecePointsArrays().at(
              piece_type_key
          ))) {
        return false;
      }
    }
  }
  return true;
}

} // namespace piece_points_spec

#endif /* B3123E8C_B936_4802_A0CD_13BABD26E0A8 */
