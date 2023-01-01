//
// Created by duane on 12/27/22.
//

#ifndef XIANGQI_CPP_hash_calculator_HPP
#define XIANGQI_CPP_hash_calculator_HPP

#include <array>
#include <bits/stdc++.h>
#include <fstream>
#include <iostream>
#include <string>
// #include <game_board.hpp>
#include <board_components.hpp>
#include <utility_functs.hpp>

using namespace board_components;
using namespace std;

typedef array < array< unsigned long long, kNumFiles >, kNumRanks > piece_zarray_t;
typedef array < piece_zarray_t, kNumPieceTypeVals > team_zarray_t;
typedef array <team_zarray_t, 2> game_zarray_t;

const string kDefaultKeysFile = "zkeys_v01.json";

string get_sibling_path(string sibling_filename);
game_zarray_t zkeys_from_json(string zkeys_json_filename);


class HashCalculator {
public:
    HashCalculator(game_zarray_t zkeys);
    HashCalculator();
    unsigned long long CalcNewBoardState(const ExecutedMove& move, unsigned long long board_state) {
        return CalcNewBoardStatePrivate(move, board_state);      
    }
    unsigned long long CalcInitialBoardState(BoardMap_t& board_map);

private:
    game_zarray_t zkeys_;   
    
    size_t get_zcolor_index(PieceColor color) {
        return (size_t) (color + (int) (color < 0));
    }

    unsigned long long GetHashValue(PieceColor color, PieceType piece_type, BoardSpace space) {
        return zkeys_[get_zcolor_index(color)][piece_type][space.rank][space.file];
    }

    unsigned long long CalcNewBoardStatePrivate(const ExecutedMove& move, unsigned long long board_state) {
            // moving piece moves away from space
            board_state = board_state ^ GetHashValue(
                move.moving_piece.piece_color,
                move.moving_piece.piece_type,
                move.spaces.start);
            
            // if capture piece, remove from board
            if (move.destination_piece.piece_color != PieceColor::kNul) {
                board_state = board_state ^ GetHashValue(
                    move.destination_piece.piece_color,
                    move.destination_piece.piece_type,
                    move.spaces.end);
            }

            // moving piece to new space
            board_state = board_state ^ GetHashValue(
                move.moving_piece.piece_color,
                move.moving_piece.piece_type,
                move.spaces.end
            );

            return board_state;
        }






};


#endif //XIANGQI_CPP_hash_calculator_HPP
