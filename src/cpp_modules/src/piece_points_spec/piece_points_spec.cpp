
#include <common.hpp>
#include <algorithm>
#include <iostream>
#include <piece_points_spec.hpp>
#include <unordered_map>
#include <utility_functs.hpp>


using namespace std;
using namespace piece_points_spec;
using json = nlohmann::json;

PointsSpecBPOExternal::PointsSpecBPOExternal(
    BasePointsSMap_t black_base_input,
    BasePointsSMap_t red_base_offsets_input,
    TeamPointsSMap_t black_position_input,
    TeamPointsSMap_t red_position_offsets_input
)
    : black_base{black_base_input}
    , red_base_offsets{red_base_offsets_input}
    , black_position{black_position_input}
    , red_position_offsets{red_position_offsets_input} {}

PointsSpecBPOExternal::PointsSpecBPOExternal(const json &json_object) {
  json_object.at("black_base").get_to(black_base);
  json_object.at("red_base_offsets").get_to(red_base_offsets);
  json_object.at("black_position").get_to(black_position);
  json_object.at("red_position_offsets").get_to(red_position_offsets);
}

PointsSpecBPOExternal::PointsSpecBPOExternal(string json_file_path)
    : PointsSpecBPOExternal(utility_functs::import_json(json_file_path)){};

json PointsSpecBPOExternal::ToJson() {
  json j;
  j["black_base"] = black_base;
  j["red_base_offsets"] = red_base_offsets;
  j["black_position"] = black_position;
  j["red_position_offsets"] = red_position_offsets;
  return j;
}

void PointsSpecBPOExternal::ToFile(string output_path) {
  auto json_object = ToJson();
  utility_functs::export_json(json_object, output_path);
}

GamePointsSMap_t PointsSpecBPOExternal::ToGamePointsSmap() {
    GamePointsSMap_t s_map{};

    for (auto piece : black_base) {
        s_map["black"][piece.first] = utility_functs::array_plus_const(
            black_position[piece.first], black_base[piece.first]);
    }

    for (auto piece : red_base_offsets) {
        auto red_base = black_base[piece.first] + red_base_offsets[piece.first];
        auto red_position = utility_functs::two_array_sum(
            black_position[piece.first], red_position_offsets[piece.first]);
        s_map["red"][piece.first] = utility_functs::array_plus_const(
            red_position, red_base);
    }

    return s_map;
}

PointsSpecBPOInternal::PointsSpecBPOInternal(
    TeamBasePoints_t black_base_input,
    TeamBasePoints_t red_base_offsets_input,
    TeamPointsEMap_t black_position_input,
    TeamPointsEMap_t red_position_offsets_input
)
    : black_base{black_base_input}
    , red_base_offsets{red_base_offsets_input}
    , black_position{black_position_input}
    , red_position_offsets{red_position_offsets_input} {}

PointsSpecBPOInternal::PointsSpecBPOInternal(
    PointsSpecBPOExternal external_spec
) {
  unordered_map<string, PieceType> key_substitutions = {
      {"null", PieceType::kNnn},
      {"general", PieceType::kGen},
      {"advisor", PieceType::kAdv},
      {"elephant", PieceType::kEle},
      {"chariot", PieceType::kCha},
      {"horse", PieceType::kHor},
      {"cannon", PieceType::kCan},
      {"soldier", PieceType::kSol}};

  black_base = utility_functs::replace_keys(
      external_spec.black_base,
      key_substitutions
  );

  red_base_offsets = utility_functs::replace_keys(
      external_spec.red_base_offsets,
      key_substitutions
  );

  black_position = utility_functs::replace_keys(
      external_spec.black_position,
      key_substitutions
  );

  red_position_offsets = utility_functs::replace_keys(
      external_spec.red_position_offsets,
      key_substitutions
  );
}