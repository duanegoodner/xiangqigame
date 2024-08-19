// Filename: game_board.tpp
// Author: Duane Goodner
// Created: 2022-12-27
// Last Modified: 2024-08-16

// Description:
// Defines struct and class used for Zobrist Hash tracking of board state.

#ifndef XIANGQI_CPP_hash_calculator_HPP
#define XIANGQI_CPP_hash_calculator_HPP

#include <board_components.hpp>
#include <common.hpp>
#include <game_board.hpp>
#include <move_selector.hpp>
#include <utility_functs.hpp>

using namespace board_components;
using namespace std;
using json = nlohmann::json;

struct ZobristKeys {
  game_zarray_t zarray;
  zkey_t turn_key;
  ZobristKeys();
  ZobristKeys(zkey_t new_turn_key, game_zarray_t &new_zarray);
  ZobristKeys(const json &json_object);
  ZobristKeys(string json_file_path);
  json ToJson();

  zkey_t GetHashValue(
      PieceColor color,
      PieceType piece_type,
      BoardSpace space
  ) {
    return zarray[get_zcolor_index(color)][piece_type][space.rank][space.file];
  }
};

class HashCalculator : public BoardStateTracker<HashCalculator>,
                       public BoardStateProvider<HashCalculator> {
public:
  HashCalculator(ZobristKeys zkeys);
  HashCalculator();
  zkey_t ImplementGetBoardState() {return board_state_;}
  void ImplementCalcNewBoardState(const ExecutedMove &move) {
    PrivateImplementCalcNewBoardState(move);   
  }
  void ImplementCalcInitialBoardState(const BoardMap_t &board_map);

private:
  ZobristKeys zkeys_;
  zkey_t board_state_;
  map<zkey_t, Points_t> transposition_table_; 

  void PrivateImplementCalcNewBoardState(ExecutedMove move) {
    // moving piece moves away from space
    board_state_ = board_state_ ^ zkeys_.GetHashValue(
                                      move.moving_piece.piece_color,
                                      move.moving_piece.piece_type,
                                      move.spaces.start
                                  );

    // if capture piece, remove from board
    if (move.destination_piece.piece_color != PieceColor::kNul) {
      board_state_ = board_state_ ^ zkeys_.GetHashValue(
                                        move.destination_piece.piece_color,
                                        move.destination_piece.piece_type,
                                        move.spaces.end
                                    );
    }

    // moving piece to new space
    board_state_ = board_state_ ^ zkeys_.GetHashValue(
                                      move.moving_piece.piece_color,
                                      move.moving_piece.piece_type,
                                      move.spaces.end
                                  );

    // change state now that its other player's turn
    board_state_ = board_state_ ^ zkeys_.turn_key;    
  }
};

#endif // XIANGQI_CPP_hash_calculator_HPP
