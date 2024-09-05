// Filename: game_board.tpp
// Author: Duane Goodner
// Created: 2022-12-27
// Last Modified: 2024-08-16

// Description:
// Defines struct and class used for Zobrist Hash tracking of board state.

#pragma once

#include <array>
#include <board_components.hpp>
#include <common.hpp>
#include <game_board.hpp>
#include <key_generator.hpp>
#include <utility_functs.hpp>

using namespace board_components;
using namespace std;
using json = nlohmann::json;

template <typename KeyType>
struct ZobristKeys {
  typedef array<array<KeyType, kNumFiles>, kNumRanks> PieceZarray_t;
  typedef array<PieceZarray_t, kNumPieceTypeVals> TeamZarray_t;
  typedef array<TeamZarray_t, 2> GameZarray_t;
  GameZarray_t zarray;
  KeyType turn_key;

  ZobristKeys()
      : zarray{}
      , turn_key{} {
    std::random_device rd;
    std::mt19937_64 gen_64{rd()};    
    turn_key = KeyGenerator::generate_zkey(gen_64);
    zarray = create_game_zarray(gen_64);
  };

  ZobristKeys(uint32_t seed)
      : zarray{}
      , turn_key{} {
    std::mt19937_64 gen_64(seed);
    turn_key = KeyGenerator::generate_zkey(gen_64);
    zarray = create_game_zarray(gen_64);
  };

  ZobristKeys(KeyType new_turn_key, GameZarray_t &new_zarray)
      : turn_key{new_turn_key}
      , zarray{new_zarray} {};

  ZobristKeys(const json &json_object) {
    json_object.at("turn_key").get_to(turn_key);
    json_object.at("zarray").get_to(zarray);
  };

  ZobristKeys(string json_file_path)
      : ZobristKeys(utility_functs::import_json(json_file_path)) {};

  json ToJson() {
    json j;
    j["zarray"] = zarray;
    j["turn_key"] = turn_key;
    return j;
  };

  KeyType GetHashValue(
      PieceColor color,
      PieceType piece_type,
      BoardSpace space
  ) {
    return zarray[get_zcolor_index(color)][piece_type][space.rank][space.file];
  }
  static const GameZarray_t create_game_zarray(std::mt19937_64 &gen_64) {
    GameZarray_t game_zarray{};
    for (auto color_idx = 0; color_idx < 2; color_idx++) {
      for (auto piece_id = 1; piece_id < kNumPieceTypeVals; piece_id++) {
        for (auto rank = 0; rank < kNumRanks; rank++) {
          for (auto file = 0; file < kNumFiles; file++) {
            game_zarray[color_idx][piece_id][rank][file] =
                KeyGenerator::generate_zkey(gen_64);
          }
        }
      }
    }
    return game_zarray;
  };
};

template <typename KeyType>
class HashCalculator : public BoardStateSummarizer<HashCalculator<KeyType>, KeyType> {
public:
  HashCalculator(ZobristKeys<KeyType> zkeys)
      : zkeys_{zkeys}
      , board_state_{} {};
  HashCalculator()
      : HashCalculator(ZobristKeys<KeyType>()) {};
  KeyType ImplementGetState() { return board_state_; }
  void ImplementCalcNewBoardState(const ExecutedMove &move) {
    PrivateImplementCalcNewBoardState(move);
  }
  void ImplementCalcInitialBoardState(const BoardMap_t &board_map) {
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
  };

private:
  ZobristKeys<KeyType> zkeys_;
  KeyType board_state_;

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
