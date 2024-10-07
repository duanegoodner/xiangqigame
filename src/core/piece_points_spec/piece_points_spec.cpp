// Filename: piece_points_spec.cpp
// Author: Duane Goodner
// Created: 2022-12-15
// Last Modified: 2024-08-16

// Description:
// Implementation of PiecePointsSpec

#include <algorithm>
// #include <common.hpp>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <piece_points_bpo.hpp>
#include <unordered_map>
#include <utility_functs.hpp>

using namespace std;
using namespace piece_points;
using namespace gameboard;
using nloh_json = nlohmann::json;

BPOPointsSKeys::BPOPointsSKeys()
    : black_base_{}
    , red_base_offsets_{}
    , black_position_{}
    , red_position_offsets_{} {};

BPOPointsSKeys::BPOPointsSKeys(
    BasePointsSMap_t black_base_input,
    BasePointsSMap_t red_base_offsets_input,
    TeamPointsSMap_t black_position_input,
    TeamPointsSMap_t red_position_offsets_input
)
    : black_base_{black_base_input}
    , red_base_offsets_{red_base_offsets_input}
    , black_position_{black_position_input}
    , red_position_offsets_{red_position_offsets_input} {}

BPOPointsSKeys::BPOPointsSKeys(const string &json_file_path)
    : black_base_{}
    , red_base_offsets_{}
    , black_position_{}
    , red_position_offsets_{} {
  json_utility_->Import(*this, json_file_path);
}

void BPOPointsSKeys::ToFile(string output_path) {
  json_utility_->Export(*this, output_path);
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

BPOPointsEKeys BPOPointsSKeys::ToBPOPointsEKeys() {
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

  auto ekey_black_base = utility_functs::replace_keys(black_base_, key_substitutions);
  auto ekey_red_base_offsets =
      utility_functs::replace_keys(red_base_offsets_, key_substitutions);
  auto ekey_black_position =
      utility_functs::replace_keys(black_position_, key_substitutions);
  auto ekey_red_position_offsets =
      utility_functs::replace_keys(red_position_offsets_, key_substitutions);

  return BPOPointsEKeys(
      ekey_black_base,
      ekey_red_base_offsets,
      ekey_black_position,
      ekey_red_position_offsets
  );
}

GamePointsArray_t BPOPointsSKeys::ToGamePointsArray() {
  auto bpo_points_ekeys = ToBPOPointsEKeys();
  return bpo_points_ekeys.ToGamePointsArray();
}

BPOPointsEKeys::BPOPointsEKeys(
    TeamBasePoints_t black_base_input,
    TeamBasePoints_t red_base_offsets_input,
    TeamPointsEMap_t black_position_input,
    TeamPointsEMap_t red_position_offsets_input
)
    : black_base_{black_base_input}
    , red_base_offsets_{red_base_offsets_input}
    , black_position_{black_position_input}
    , red_position_offsets_{red_position_offsets_input} {}

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

  black_base_ =
      utility_functs::replace_keys(external_spec.black_base_, key_substitutions);

  red_base_offsets_ =
      utility_functs::replace_keys(external_spec.red_base_offsets_, key_substitutions);

  black_position_ =
      utility_functs::replace_keys(external_spec.black_position_, key_substitutions);

  red_position_offsets_ = utility_functs::replace_keys(
      external_spec.red_position_offsets_,
      key_substitutions
  );
}

TeamPointsArray_t BPOPointsEKeys::BlackNetPoints() {
  TeamPointsArray_t black_net_points{};
  for (auto piece : black_base_) {

    black_net_points[piece.first] = utility_functs::array_plus_const(
        black_position_[piece.first],
        black_base_[piece.first]
    );
  }
  return black_net_points;
}

TeamPointsArray_t BPOPointsEKeys::RedNetPoints() {
  TeamPointsArray_t red_net_points{};
  for (auto piece : red_base_offsets_) {
    auto base_points = black_base_[piece.first] + red_base_offsets_[piece.first];

    auto unflipped_position_points = utility_functs::two_array_sum(
        black_position_[piece.first],
        red_position_offsets_[piece.first]
    );

    auto flipped_position_points =
        utility_functs::vertical_flip_array(unflipped_position_points);

    red_net_points[piece.first] =
        utility_functs::array_plus_const(flipped_position_points, base_points);
  }
  return red_net_points;
}

GamePointsArray_t BPOPointsEKeys::ToGamePointsArray() {
  GamePointsArray_t game_points_array{};
  game_points_array[get_zcolor_index(PieceColor::kBlk)] = BlackNetPoints();
  game_points_array[get_zcolor_index(PieceColor::kRed)] = RedNetPoints();
  return game_points_array;
}