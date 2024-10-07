//! @file hash_calculator.hpp
//! Class templates for HashCalculator and its supporting class ZobristKeys.

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

using namespace gameboard;
using namespace moveselection;
using namespace std;

namespace boardstate {

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
    KeyGenerator key_generator;
    turn_key = key_generator.GenerateKey<KeyType>();
    zarray = CreateGameZarray(key_generator);
  };

  ZobristKeys(uint32_t seed)
      : zarray{}
      , turn_key{} {
    KeyGenerator key_generator;
    turn_key = key_generator.GenerateKey<KeyType>();
    zarray = CreateGameZarray(key_generator);
  };

  ZobristKeys(KeyType new_turn_key, GameZarray_t &new_zarray)
      : turn_key{new_turn_key}
      , zarray{new_zarray} {};

  KeyType GetHashValue(PieceColor color, PieceType piece_type, BoardSpace space) {
    return zarray[get_zcolor_index(color)][piece_type][space.rank][space.file];
  }
  static const GameZarray_t CreateGameZarray(KeyGenerator key_generator) {
    GameZarray_t game_zarray{};
    for (auto color_idx = 0; color_idx < 2; color_idx++) {
      for (auto piece_id = 1; piece_id < kNumPieceTypeVals; piece_id++) {
        for (auto rank = 0; rank < kNumRanks; rank++) {
          for (auto file = 0; file < kNumFiles; file++) {
            game_zarray[color_idx][piece_id][rank][file] =
                key_generator.GenerateKey<KeyType>();
          }
        }
      }
    }
    return game_zarray;
  };
};

template <typename KeyType>
struct TranspositionTable {

  TranspositionTableSearchResult GetData(
      KeyType board_state,
      int remaining_search_depth
  ) {
    TranspositionTableSearchResult result{};

    auto entry_vector_it = data_.find(board_state);
    if (entry_vector_it != data_.end()) {
      auto entry_vector = entry_vector_it->second;
      for (auto entry : entry_vector) {
        if (entry.remaining_search_depth == remaining_search_depth) {
          result.found = true;
          result.table_entry = entry;
        }
      }
    }
    return result;
  }

  void RecordData(
      KeyType state,
      int search_depth,
      MinimaxResultType result_type,
      BestMoves &best_moves
  ) {
    TranspositionTableEntry transposition_table_entry{
        search_depth,
        result_type,
        best_moves
    };
    data_[state].push_back(transposition_table_entry);
  };

private:
  unordered_map<KeyType, vector<TranspositionTableEntry>> data_;
};

template <typename KeyType>
class HashCalculator : public BoardStateSummarizer<HashCalculator<KeyType>, KeyType> {
public:
  HashCalculator(ZobristKeys<KeyType> zkeys)
      : zkeys_{zkeys}
      , board_state_{}
      , transposition_table_{} {};
  HashCalculator()
      : HashCalculator(ZobristKeys<KeyType>()) {};
  KeyType ImplementGetState() { return board_state_; }
  void ImplementUpdateBoardState(const ExecutedMove &move) {
    _ImplementUpdateBoardState(move);
  }
  void ImplementFullBoardStateCalc(const BoardMap_t &board_map) {
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
  void ImplementRecordTrData(
      int search_depth,
      MinimaxResultType result_type,
      BestMoves &best_moves
  ) {
    transposition_table_.RecordData(board_state_, search_depth, result_type, best_moves);
  }

  TranspositionTableSearchResult ImplementGetTrData(int search_depth) {
    return transposition_table_.GetData(board_state_, search_depth);
  }

private:
  ZobristKeys<KeyType> zkeys_;
  KeyType board_state_;
  TranspositionTable<KeyType> transposition_table_;

  void _ImplementUpdateBoardState(ExecutedMove move) {
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

} // namespace boardstate
