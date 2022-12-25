
#include <array>
#include <cstddef>
#include <fstream>
#include <iostream>
#include "../nlohmann_json/json.hpp"
#include "../utility_functs/utility_functs.hpp"
#include "../board_components/board_components.hpp"


using namespace board_components;
using namespace nlohmann::json_abi_v3_11_2;
using namespace std;


typedef array < array< unsigned long long, kNumFiles >, kNumRanks > piece_zarray_t;
typedef array < piece_zarray_t, kNumPieceTypeVals > team_zarray_t;
typedef array <team_zarray_t, 2> game_zarray_t;


void fill_std_piece_vals(game_zarray_t& game_zarray) {
    for (auto piece_id = 1; piece_id < kNumPieceTypeVals; piece_id++) {
        for (auto rank = 0; rank < kNumRanks; rank++) {
            for (auto file = 0; file < kNumFiles; file++) {
                game_zarray[0][piece_id][rank][file] = utility_functs::random(
                    (unsigned long long)0, (unsigned long long) numeric_limits<unsigned long long>::max);
            }
        }
    }
}


int main(int argc, char* argv[]) {

    game_zarray_t zarray{0};
    fill_std_piece_vals(zarray);

    json zobrist_keys(zarray);
    ofstream output("zobrist_keys.json");
    output << setw(4) << zobrist_keys << endl;

    return 0;    
    
}

