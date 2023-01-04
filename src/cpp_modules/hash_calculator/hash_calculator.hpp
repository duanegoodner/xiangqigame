//
// Created by duane on 12/27/22.
//

#ifndef XIANGQI_CPP_hash_calculator_HPP
#define XIANGQI_CPP_hash_calculator_HPP

#include <bits/stdc++.h>

#include <array>
#include <fstream>
#include <iostream>
#include <string>
#include <board_components.hpp>
#include <json.hpp>
#include <utility_functs.hpp>

using namespace board_components;
using namespace std;
using json = nlohmann::json;

// TODO: move this and other config items to settings file
const string kDefaultKeysFile = "zkeys_v01.json";


typedef unsigned long long zkey_t;
typedef array<array<zkey_t, kNumFiles>, kNumRanks> piece_zarray_t;
typedef array<piece_zarray_t, kNumPieceTypeVals> team_zarray_t;
typedef array<team_zarray_t, 2> game_zarray_t;

zkey_t random_zkey();
game_zarray_t create_zarray();
json import_json(string file_path);
void export_json(const json &j, string filename);
string get_sibling_path(string sibling_filename);
string default_keys_filepath();



struct ZobristKeys {
  game_zarray_t zarray;
  zkey_t turn_key;
  ZobristKeys();
  ZobristKeys(zkey_t new_turn_key, game_zarray_t& new_zarray);
  ZobristKeys(const json &json_object);
  ZobristKeys(string json_file_path);
  json ToJson();

  zkey_t
  GetHashValue(PieceColor color, PieceType piece_type, BoardSpace space) {
    return zarray[get_zcolor_index(color)][piece_type][space.rank][space.file];
  }

  private:
  size_t get_zcolor_index(PieceColor color) {
    return (size_t)(color + (int)(color < 0));
  }  
};

class HashCalculator {
public:
  HashCalculator(ZobristKeys zkeys);
  HashCalculator();
  zkey_t CalcNewBoardState(const ExecutedMove &move, zkey_t board_state) {
    return CalcNewBoardStatePrivate(move, board_state);
  }
  zkey_t CalcInitialBoardState(BoardMap_t &board_map);

private:
  ZobristKeys zkeys_;
 

  zkey_t CalcNewBoardStatePrivate(
      const ExecutedMove &move,
      zkey_t board_state
  ) {
    // moving piece moves away from space

    board_state = board_state ^ zkeys_.GetHashValue(
                                    move.moving_piece.piece_color,
                                    move.moving_piece.piece_type,
                                    move.spaces.start
                                );

    // if capture piece, remove from board
    if (move.destination_piece.piece_color != PieceColor::kNul) {
      board_state = board_state ^ zkeys_.GetHashValue(
                                      move.destination_piece.piece_color,
                                      move.destination_piece.piece_type,
                                      move.spaces.end
                                  );
    }

    // moving piece to new space
    board_state = board_state ^ zkeys_.GetHashValue(
                                    move.moving_piece.piece_color,
                                    move.moving_piece.piece_type,
                                    move.spaces.end
                                );
    
    // change state now that its other player's turn
    board_state = board_state ^ zkeys_.turn_key;

    return board_state;
  }
};

#endif // XIANGQI_CPP_hash_calculator_HPP
