// Filename: game_board.tpp
// Author: Duane Goodner
// Created: 2022-12-27
// Last Modified: 2024-08-16

// Description:
// Implementation of ZobristKeys and HashCalculator.

#include <board_components.hpp>
#include <common.hpp>
#include <experimental/source_location>
#include <filesystem>
#include <fstream>
#include <hash_calculator.hpp>
#include <hash_calculator_details.hpp>
#include <nlohmann/json.hpp>
#include <utility_functs.hpp>

using namespace board_components;
using json = nlohmann::json;
using namespace std;

zkey_t random_zkey() {
  return utility_functs::random(
      (zkey_t)0,
      (zkey_t)numeric_limits<zkey_t>::max
  );
}

game_zarray_t create_zarray() {
  game_zarray_t zarray{};

  for (auto color_idx = 0; color_idx < 2; color_idx++) {
    for (auto piece_id = 1; piece_id < kNumPieceTypeVals; piece_id++) {
      for (auto rank = 0; rank < kNumRanks; rank++) {
        for (auto file = 0; file < kNumFiles; file++) {
          zarray[color_idx][piece_id][rank][file] = utility_functs::random(
              (unsigned long long)0,
              (unsigned long long)numeric_limits<unsigned long long>::max
          );
        }
      }
    }
  }
  return zarray;
}

ZobristKeys::ZobristKeys()
    : zarray{0}
    , turn_key{0} {}

ZobristKeys::ZobristKeys(zkey_t new_turn_key, game_zarray_t &new_zarray)
    : turn_key{new_turn_key}
    , zarray{new_zarray} {}

ZobristKeys::ZobristKeys(const json &json_object) {
  json_object.at("turn_key").get_to(turn_key);
  json_object.at("zarray").get_to(zarray);
}

ZobristKeys::ZobristKeys(string json_file_path)
    : ZobristKeys(utility_functs::import_json(json_file_path)) {}

json ZobristKeys::ToJson() {
  json j;
  j["zarray"] = zarray;
  j["turn_key"] = turn_key;
  return j;
}

void HashCalculator::ImplementCalcInitialBoardState(const BoardMap_t &board_map
) {
  board_state_ = 0;
  for (size_t rank = 0; rank < kNumRanks; rank++) {
    for (size_t file = 0; file < kNumFiles; file++) {
      if (board_map[rank][file].piece_color != 0) {
        board_state_ = board_state_ ^ zkeys_.GetHashValue(
                                          board_map[rank][file].piece_color,
                                          board_map[rank][file].piece_type,
                                          BoardSpace{(int)rank, (int)file}
                                      );
      }
    }
  }
}

HashCalculator::HashCalculator(ZobristKeys zkeys)
    : zkeys_{zkeys}
    , board_state_{}
    , transposition_table_ {}

HashCalculator::HashCalculator()
    : HashCalculator(ZobristKeys(DEFAULT_ZKEYS_FILEPATH)) {}
