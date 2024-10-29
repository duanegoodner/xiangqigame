//! @file hash_calculator.hpp
//! Class templates for boardstate::SingleZobristTracker and its supporting class
//! boardstate::ZobristCalculator.

#pragma once

#include <array>
// #include <boost/math/distributions/uniform.hpp>
#include <board_data_structs.hpp>
#include <game_board.hpp>
#include <key_generator.hpp>
#include <utility_functs.hpp>

using namespace gameboard;
using namespace moveselection;
using namespace std;

namespace boardstate {

//! Container for all of the hash keys needed to run a boardstate::SingleZobristTracker.
template <typename KeyType>
class ZobristCalculator {
public:
  typedef array<array<KeyType, kNumFiles>, kNumRanks> PieceZarray_t;
  typedef array<PieceZarray_t, kNumPieceTypeVals> TeamZarray_t;
  typedef array<TeamZarray_t, 2> GameZarray_t;

  ZobristCalculator(uint32_t seed)
      : zarray_{}
      , turn_key_{}
      , seed_{seed} {
    PseudoRandomKeyGenerator<KeyType> key_generator{seed};
    turn_key_ = key_generator.GenerateKey();
    zarray_ = CreateGameZarray(key_generator);
  };

  ZobristCalculator(KeyType new_turn_key, GameZarray_t &new_zarray)
      : turn_key_{new_turn_key}
      , zarray_{new_zarray}
      , seed_{} {};

  void FullBoardStateCalc(const BoardMap_t &board_map) {
    board_state_ = 0;
    for (size_t rank = 0; rank < kNumRanks; rank++) {
      for (size_t file = 0; file < kNumFiles; file++) {
        if (board_map[rank][file].piece_color != 0) {
          board_state_ = board_state_ ^ GetHashValueAt(
                                            board_map[rank][file].piece_color,
                                            board_map[rank][file].piece_type,
                                            BoardSpace{(int)rank, (int)file}
                                        );
        }
      }
    }
  }

  void UpdateBoardState(ExecutedMove move) {
    board_state_ = board_state_ ^ GetHashValueAt(
                                      move.moving_piece.piece_color,
                                      move.moving_piece.piece_type,
                                      move.spaces.start
                                  );

    // if capture piece, remove from board
    if (move.destination_piece.piece_color != PieceColor::kNul) {
      board_state_ = board_state_ ^ GetHashValueAt(
                                        move.destination_piece.piece_color,
                                        move.destination_piece.piece_type,
                                        move.spaces.end
                                    );
    }

    // moving piece to new space
    board_state_ = board_state_ ^ GetHashValueAt(
                                      move.moving_piece.piece_color,
                                      move.moving_piece.piece_type,
                                      move.spaces.end
                                  );

    // change state now that its other player's turn
    board_state_ = board_state_ ^ turn_key();
  }

  KeyType GetHashValueAt(PieceColor color, PieceType piece_type, BoardSpace space) {
    return zarray_[GetZColorIndexOf(color)][piece_type][space.rank][space.file];
  }

  //! Collects all keys into a std::vector (typically used for entropy calc).
  std::vector<KeyType> KeysVector() const {
    std::vector<KeyType> keys_vector;

    for (const auto &team : zarray) {
      for (const auto &piece_zarray : team) {
        for (const auto &rank : piece_zarray) {
          for (KeyType key : rank) {
            keys_vector.push_back(key);
          }
        }
      }
    }
    keys_vector.push_back(turn_key);

    return keys_vector;
  }

  KeyType board_state() { return board_state_; }
  GameZarray_t zarray() { return zarray_; }
  KeyType turn_key() { return turn_key_; }
  uint32_t seed() { return seed_; }

private:
  GameZarray_t zarray_;
  KeyType turn_key_;
  uint32_t seed_;
  KeyType board_state_;

  static const GameZarray_t CreateGameZarray(
      PseudoRandomKeyGenerator<KeyType> &key_generator
  ) {
    GameZarray_t game_zarray{};
    for (auto color_idx = 0; color_idx < 2; color_idx++) {
      for (auto piece_id = 1; piece_id < kNumPieceTypeVals; piece_id++) {
        for (auto rank = 0; rank < kNumRanks; rank++) {
          for (auto file = 0; file < kNumFiles; file++) {
            game_zarray[color_idx][piece_id][rank][file] = key_generator.GenerateKey();
          }
        }
      }
    }
    return game_zarray;
  };
};

//! Container where boardstate::SingleZobristTracker stores moveselection::MinimaxMoveEvaluator
//! results; supports recording, look-up and retrieval of data.
template <typename KeyType>
class TranspositionTable {
public:
  TranspositionTable()
      : num_entries_{0} {}

  TranspositionTableSearchResult GetDataAt(
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
          break;
        }
      }
    }
    return result;
  }

  void RecordData(
      KeyType state,
      int search_depth,
      MinimaxResultType result_type,
      EqualScoreMoves &similar_moves
  ) {
    data_[state].emplace_back(search_depth, result_type, similar_moves);
    num_entries_++;
  };

  uint64_t num_entries() { return num_entries_; }

  size_t num_states() { return data_.size(); }

private:
  unordered_map<KeyType, vector<TranspositionTableEntry>> data_;
  uint64_t num_entries_;
};

template <typename KeyType>
struct DualKeyTranspositionTableEntry {
  KeyType confirmation_state;
  TranspositionTableEntry single_key_entry;
};

template <typename KeyType>
class DualKeyTranspositionTable {
public:
  DualKeyTranspositionTable()
      : num_entries_{0} {}

  TranspositionTableSearchResult GetDataAt(
      KeyType board_state,
      KeyType expected_confirmation_state,
      int remaining_search_depth
  ) {
    TranspositionTableSearchResult result{};
    auto dual_key_entry_vector_it = data_.find(board_state);
    if (dual_key_entry_vector_it != data_.end()) {
      auto dual_key_entry_vector = dual_key_entry_vector_it->second;
      for (auto dual_key_entry : dual_key_entry_vector) {
        if (dual_key_entry.single_key_entry.remaining_search_depth ==
            remaining_search_depth) {
          result.found = true;
          result.table_entry = dual_key_entry.single_key_entry;
          if (dual_key_entry.confirmation_state != expected_confirmation_state) {
            result.known_collision = true;
          }
          break;
        }
      }
    }
    return result;
  }

  void RecordData(
      KeyType board_state,
      KeyType confirmation_state,
      int search_depth,
      MinimaxResultType result_type,
      EqualScoreMoves &similar_moves
  ) {
    TranspositionTableEntry single_key_entry{search_depth, result_type, similar_moves};
    data_[board_state].emplace_back(confirmation_state, single_key_entry);
    num_entries_++;
  }

  uint64_t num_entries() { return num_entries_; }
  size_t num_states() { return data_.size(); }

private:
  unordered_map<KeyType, vector<DualKeyTranspositionTableEntry<KeyType>>> data_;
  uint64_t num_entries_;
};

//! Implements BoardStateSummarizer interface; calculates Zobrist hash value of board
//! configuration; provides moveselection::MinimaxMoveEvaluator access to
//! boardstate::TranspositionTable
template <typename KeyType>
class SingleZobristTracker : public BoardStateSummarizer<SingleZobristTracker<KeyType>, KeyType> {
public:
  SingleZobristTracker(ZobristCalculator<KeyType> zkeys)
      : zkeys_{zkeys}
      // , board_state_{}
      , transposition_table_{} {};
  SingleZobristTracker(uint32_t seed)
      : SingleZobristTracker(ZobristCalculator<KeyType>(seed)) {};

  KeyType ImplementGetState() { return zkeys_.board_state(); }

  void ImplementUpdateBoardState(const ExecutedMove &executed_move) {
    zkeys_.UpdateBoardState(executed_move);
  }
  void ImplementFullBoardStateCalc(const BoardMap_t &board_map) {
    zkeys_.FullBoardStateCalc(board_map);
  };
  void ImplementRecordTrData(
      int search_depth,
      MinimaxResultType result_type,
      EqualScoreMoves &similar_moves
  ) {
    transposition_table_
        .RecordData(zkeys_.board_state(), search_depth, result_type, similar_moves);
  }

  TranspositionTableSearchResult ImplementGetTrData(int search_depth) {
    return transposition_table_.GetDataAt(zkeys_.board_state(), search_depth);
  }

  moveselection::TranspositionTableSize ImplementGetTrTableSize() {
    moveselection::TranspositionTableSize result{
        transposition_table_.num_entries(),
        transposition_table_.num_states()
    };
    return result;
  }

  const ZobristCalculator<KeyType> &GetZobristCalculator() const { return zkeys_; }

  const uint32_t zkeys_seed() { return zkeys_.seed(); }

  const KeyType board_state() { return zkeys_.board_state(); }

  const string board_state_hex_str() {
    return boardstate::IntToHexString(board_state());
  }

private:
  ZobristCalculator<KeyType> zkeys_;
  TranspositionTable<KeyType> transposition_table_;
};

// template <typename KeyType>
// class DualSingleZobristTracker
//     : public BoardStateSummarizer<SingleZobristTracker<KeyType>, KeyType> {
// public:
//   DualSingleZobristTracker(uint32_t seed);

//   KeyType ImplementGetState() { return primary_hash_calculator_.GetState(); }

//   void ImplementUpdateBoardState(const ExecutedMove &move) {
//     primary_hash_calculator_.UpdateBoardState();
//     confirmation_hash_calculator_.UpdateBoardState();
//   }

//   void ImplementFullBoardStateCalc(const BoardMap_t &board_map) {
//     primary_hash_calculator_.FullBoardStateCalc();
//     confirmation_hash_calculator_.FullBoardStateCalc();
//   }

//   void ImplementRecordTrData(
//     int search_depth,
//     MinimaxResultType result_type,
//     EqualScoreMoves &similar_moves
//   ) {

//   }

// private:
//   SingleZobristTracker<KeyType> primary_hash_calculator_;
//   SingleZobristTracker<KeyType> confirmation_hash_calculator_;
// };

} // namespace boardstate
