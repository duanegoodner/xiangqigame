// Filename: piece_points_spec.cpp
// Author: Duane Goodner
// Created: 2022-12-15
// Last Modified: 2024-08-16

// Description:
// Implementation of PiecePointsSpec

#include <algorithm>
#include <common.hpp>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <piece_points_spec.hpp>
#include <unordered_map>
#include <utility_functs.hpp>

using namespace std;
using namespace piece_points;
using nloh_json = nlohmann::json;

void NlohmannBPOFileHandler::Import(
    BPOPointsSKeys &bpo_points,
    const string file_path
) {
  ifstream input{file_path};
  auto json_object = nloh_json::parse(input);
  json_object.at("black_base").get_to(bpo_points.black_base_);
  json_object.at("red_base_offsets").get_to(bpo_points.red_base_offsets_);
  json_object.at("black_position").get_to(bpo_points.black_position_);
  json_object.at("red_position_offsets").get_to(bpo_points.red_position_offsets_);
}

nloh_json NlohmannBPOFileHandler::ToJsonObject(BPOPointsSKeys &bpo_points) {
  nloh_json j;
  j["black_base"] = bpo_points.black_base_;
  j["red_base_offsets"] = bpo_points.red_base_offsets_;
  j["black_position"] = bpo_points.black_position_;
  j["red_position_offsets"] = bpo_points.red_position_offsets_;
  return j;
}

void NlohmannBPOFileHandler::Export(
    BPOPointsSKeys &bpo_points,
    string file_path
) {
  auto json_object = ToJsonObject(bpo_points);
  ofstream fout(file_path);
  fout << setw(4) << json_object << endl;
}

BPOPointsSKeys::BPOPointsSKeys()
    : black_base_{}
    , red_base_offsets_{}
    , black_position_{}
    , red_position_offsets_{}
    , file_handler_{} {};

BPOPointsSKeys::BPOPointsSKeys(
    BasePointsSMap_t black_base_input,
    BasePointsSMap_t red_base_offsets_input,
    TeamPointsSMap_t black_position_input,
    TeamPointsSMap_t red_position_offsets_input
)
    : black_base_{black_base_input}
    , red_base_offsets_{red_base_offsets_input}
    , black_position_{black_position_input}
    , red_position_offsets_{red_position_offsets_input}
    , file_handler_{} {}

BPOPointsSKeys::BPOPointsSKeys(const string &json_file_path)
    : black_base_{}
    , red_base_offsets_{}
    , black_position_{}
    , red_position_offsets_{}
    , file_handler_{} {
  file_handler_.Import(*this, json_file_path);
}

void BPOPointsSKeys::ToFile(string output_path) {
  file_handler_.Export(*this, output_path);
}

GamePointsSMap_t BPOPointsSKeys::ToGamePointsSmap() {
  GamePointsSMap_t s_map{};

  for (auto piece : black_base_) {
    s_map["black"][piece.first] = utility_functs::array_plus_const(
        black_position_[piece.first],
        black_base_[piece.first]
    );
  }

  for (auto piece : red_base_offsets_) {
    auto red_base = black_base_[piece.first] + red_base_offsets_[piece.first];
    auto red_position = utility_functs::two_array_sum(
        black_position_[piece.first],
        red_position_offsets_[piece.first]
    );
    s_map["red"][piece.first] = utility_functs::array_plus_const(red_position, red_base);
  }

  return s_map;
}

BPOPointsEKeys::BPOPointsEKeys(
    TeamBasePoints_t black_base_input,
    TeamBasePoints_t red_base_offsets_input,
    TeamPointsEMap_t black_position_input,
    TeamPointsEMap_t red_position_offsets_input
)
    : black_base{black_base_input}
    , red_base_offsets{red_base_offsets_input}
    , black_position{black_position_input}
    , red_position_offsets{red_position_offsets_input} {}

BPOPointsEKeys::BPOPointsEKeys(BPOPointsSKeys external_spec) {
  unordered_map<string, PieceType> key_substitutions = {
      {"null", PieceType::kNnn},
      {"general", PieceType::kGen},
      {"advisor", PieceType::kAdv},
      {"elephant", PieceType::kEle},
      {"chariot", PieceType::kCha},
      {"horse", PieceType::kHor},
      {"cannon", PieceType::kCan},
      {"soldier", PieceType::kSol}
  };

  black_base = utility_functs::replace_keys(external_spec.black_base_, key_substitutions);

  red_base_offsets =
      utility_functs::replace_keys(external_spec.red_base_offsets_, key_substitutions);

  black_position =
      utility_functs::replace_keys(external_spec.black_position_, key_substitutions);

  red_position_offsets = utility_functs::replace_keys(
      external_spec.red_position_offsets_,
      key_substitutions
  );
}