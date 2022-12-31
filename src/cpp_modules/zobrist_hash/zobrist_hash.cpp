#include <filesystem>
#include <fstream>
#include <source_location>
#include <zobrist_hash.hpp>
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

void ZobristHash::InitializeBoardState(BoardMap_t& board_map) {
    board_state_ = (unsigned long long) 0;
    for (size_t rank = 0; rank < kNumRanks; rank++) {
            for (size_t file = 0; file < kNumFiles; file++) {
                if (board_map[rank][file].piece_color != 0) {
                    board_state_ = board_state_ ^ GetHashValue(
                        board_map[rank][file].piece_color,
                        board_map[rank][file].piece_type,
                        BoardSpace{(int) rank, (int) file}
                    );
                }
            }
        }
}

ZobristHash::ZobristHash(BoardMap_t& board_map)
    : zkeys_{zkeys_from_json(kDefaultKeysFile)}, board_state_{} {
        InitializeBoardState(board_map);
    }

ZobristHash::ZobristHash(game_zarray_t zkeys, BoardMap_t& board_map)
    : zkeys_{zkeys}, board_state_{} {
        InitializeBoardState(board_map);
    }
