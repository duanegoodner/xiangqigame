#include <filesystem>
#include <fstream>
#include <source_location>
#include <hash_calculator.hpp>
#include <json.hpp>

using namespace board_components;
using namespace nlohmann::json_abi_v3_11_2;
using namespace std;

string get_sibling_path(string sibling_filename) {
    filesystem::path test_file_path = source_location::current().file_name();
    
    // initialize without filename, then append
    string sibling_path = test_file_path.parent_path().string();
    sibling_filename.insert(0, "/");
    sibling_path.append(sibling_filename);

    return sibling_path;
}

game_zarray_t zkeys_from_json(string zkeys_json_filename) {
    auto zkeys_json_path = get_sibling_path(zkeys_json_filename);
    ifstream input(zkeys_json_path);
    json zkeys = json::parse(input);
    return zkeys;
}

unsigned long long HashCalculator::CalcInitialBoardState(BoardMap_t& board_map) {
    unsigned long long board_state{} ;
    for (size_t rank = 0; rank < kNumRanks; rank++) {
            for (size_t file = 0; file < kNumFiles; file++) {
                if (board_map[rank][file].piece_color != 0) {
                    board_state = board_state ^ GetHashValue(
                        board_map[rank][file].piece_color,
                        board_map[rank][file].piece_type,
                        BoardSpace{(int) rank, (int) file}
                    );
                }
            }
        }
    return board_state;
}

HashCalculator::HashCalculator()
    : zkeys_{zkeys_from_json(kDefaultKeysFile)} {}

HashCalculator::HashCalculator(game_zarray_t zkeys)
    : zkeys_{zkeys} {}
