#include <board_components.hpp>
#include <experimental/source_location>
#include <filesystem>
#include <fstream>
#include <hash_calculator.hpp>
#include <json.hpp>
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

string get_sibling_path(string sibling_filename) {
  filesystem::path test_file_path =
      experimental::source_location::current().file_name();

  // initialize without filename, then append
  string sibling_path = test_file_path.parent_path().string();
  sibling_filename.insert(0, "/");
  sibling_path.append(sibling_filename);

  return sibling_path;
}

string default_keys_filepath() {
  return get_sibling_path(kDefaultKeysFile);
}

json import_json(string file_path) {
  ifstream input(file_path);
  return json::parse(input);
}

void export_json(const json &j, string filename) {
  ofstream fout(filename);
  fout << setw(4) << j << endl;
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
    : ZobristKeys(import_json(json_file_path)) {

//   auto json_object = import_json(json_file_path);
//   json_object.at("turn_key").get_to(turn_key);
//   json_object.at("zarray").get_to(zarray);
}

json ZobristKeys::ToJson() {
  json j;
  j["zarray"] = zarray;
  j["turn_key"] = turn_key;
  return j;
}

unsigned long long HashCalculator::CalcInitialBoardState(BoardMap_t &board_map
) {
  unsigned long long board_state{};
  for (size_t rank = 0; rank < kNumRanks; rank++) {
    for (size_t file = 0; file < kNumFiles; file++) {
      if (board_map[rank][file].piece_color != 0) {
        board_state = board_state ^ zkeys_.GetHashValue(
                                        board_map[rank][file].piece_color,
                                        board_map[rank][file].piece_type,
                                        BoardSpace{(int)rank, (int)file}
                                    );
      }
    }
  }
  return board_state;
}

HashCalculator::HashCalculator()
    : zkeys_{ZobristKeys(kDefaultKeysFile)} {}

HashCalculator::HashCalculator(ZobristKeys zkeys)
    : zkeys_{zkeys} {}
