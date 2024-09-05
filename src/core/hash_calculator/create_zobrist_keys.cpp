// Filename: game_board.tpp
// Author: Duane Goodner
// Created: 2022-12-28
// Last Modified: 2024-08-16

// Description:
// Dev code for quick check / test of Zobrist key creation and storage.

#include <board_components.hpp>
#include <hash_calculator.hpp>
#include <hash_calculator_details.hpp>
#include <key_generator.hpp>
#include <nlohmann/json.hpp>
#include <random>
#include <utility_functs.hpp>

using namespace board_components;
using json = nlohmann::json;
using namespace std;

// commenting out to ensure we don't use;
// nhlohmann json does not natively support 128 bit keys. Not doing (nor
// worrying about) any json io until/if we implement for 128 bit keys.
int main(int argc, char *argv[]) {

  // std::random_device rd;
  // std::mt19937_64 gen_64{rd()};

  // auto new_turn_key = KeyGenerator::generate_zkey(gen_64);
  // auto new_zarray = create_game_zarray(gen_64);
  // auto new_zorbist_keys = ZobristKeys(new_turn_key, new_zarray);
  // json json_object = new_zorbist_keys.ToJson();
  // utility_functs::export_json(json_object, argv[1]);

  return 0;
}
