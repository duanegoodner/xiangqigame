//
// Created by duane on 12/27/22.
//

#ifndef XIANGQI_CPP_ZOBRIST_HASH_HPP
#define XIANGQI_CPP_ZOBRIST_HASH_HPP

#include <array>
#include <bits/stdc++.h>
#include <fstream>
#include <iostream>
#include <string>
#include <game_board.hpp>
#include <json.hpp>
#include <board_utilities.hpp>
#include <board_components.hpp>
#include <utility_functs.hpp>



using namespace board_components;
using namespace nlohmann::json_abi_v3_11_2;
using namespace std;

typedef array < array< unsigned long long, kNumFiles >, kNumRanks > piece_zarray_t;
typedef array < piece_zarray_t, kNumPieceTypeVals > team_zarray_t;
typedef array <team_zarray_t, 2> game_zarray_t;

class ZobristHash {
public:
    ZobristHash(string keys_file, GameBoard& game_board);
    ZobristHash(game_zarray_t zkeys, GameBoard& game_board);
    ZobristHash(GameBoard& game_board);
    const unsigned long long get_board_state();
    void UpdateBoardState(ExecutedMove move) {

    }

private:
    game_zarray_t zkeys_;
    unsigned long long board_state_;

    size_t get_zcolor_index(PieceColor color) {
        return (size_t) (color + (int) (color < 0));
    }

    unsigned long long GetHashValue(PieceColor color, PieceType piece_type, BoardSpace space) {
        return zkeys_[get_zcolor_index(color)][piece_type][space.rank][space.file];
    }






};


#endif //XIANGQI_CPP_ZOBRIST_HASH_HPP
