//! @file hash_calculator.hpp
//! Class templates for boardstate::SingleZobristTracker and its supporting class
//! boardstate::ZobristCalculator.

#pragma once

#include <array>
#include <board_data_structs.hpp>
#include <game_board.hpp>
#include <key_generator.hpp>
#include <random>
#include <utility_functs.hpp>
#include <zobrist.hpp>

using namespace gameboard;
using namespace moveselection;
using namespace std;

namespace boardstate {

//! Container where boardstate::SingleZobristTracker stores
//! moveselection::MinimaxMoveEvaluator results; supports recording, look-up and
//! retrieval of data.
template <typename KeyType>
class SingleKeyTranspositionTable {
public:
  TranspositionTableSearchResult GetDataAt(
      KeyType board_state,
      int remaining_search_depth
  ) {
    TranspositionTableSearchResult result{};

    auto tr_table_entry_it = data_.find(board_state);

    if (tr_table_entry_it != data_.end()) {
      auto tr_table_entry = tr_table_entry_it->second;
      if (tr_table_entry.remaining_search_depth >= remaining_search_depth) {
        result.found = true;
        result.table_entry = tr_table_entry;
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

    data_.insert_or_assign(
        state,
        MinimaxCalcResult{search_depth, result_type, similar_moves}
    );
  };

  uint64_t num_entries() { return data_.size(); }

  size_t num_states() { return data_.size(); }

private:
  unordered_map<KeyType, MinimaxCalcResult> data_;
};

template <typename KeyType>
class TranspositionTableEntry {
public:
  TranspositionTableEntry(
      MinimaxCalcResult calc_result,
      std::vector<KeyType> confirmation_keys
  )
      : calc_result_{calc_result}
      , confirmation_keys_{confirmation_keys} {}

  moveselection::MinimaxCalcResult calc_result() { return calc_result_; }

  std::vector<KeyType> confirmation_keys() { return confirmation_keys_; }

  bool HasConfirmationKeys() { return confirmation_keys_.size() > 0; }

  size_t num_confirmation_keys() { return confirmation_keys_.size(); }

  bool ConfirmationKeysMatchExpected(std::vector<KeyType> expected_keys) {
    if (expected_keys.size() != confirmation_keys_.size()) {
      throw std::invalid_argument("Vectors have different sizes");
    }

    for (auto index = 0; index < expected_keys.size(); ++index) {
      if (expected_keys[index] != confirmation_keys_[index]) {
        return false;
      }
    }
    return true;
  }

private:
  moveselection::MinimaxCalcResult calc_result_;
  std::vector<KeyType> confirmation_keys_;
};

template <typename KeyType>
struct TranspositionTable {
public:
  TranspositionTableSearchResult GetDataAt(
      KeyType board_state,
      int remaining_search_depth
  ) {
    TranspositionTableSearchResult result{};
    auto tr_table_entry_it = data_.find(board_state);

    if (tr_table_entry_it != data_.end()) {
      auto tr_table_entry = tr_table_entry_it->second;
      if (tr_table_entry.calc_result().remaining_search_depth >=
          remaining_search_depth) {
        result.found = true;
        result.table_entry = tr_table_entry.calc_result();
      }
    }
    return result;
  }

  TranspositionTableSearchResult GetDataAt(
      KeyType board_state,
      int remaining_search_depth,
      const std::vector<KeyType> &expected_keys
  ) {

    auto result = GetDataAt(board_state, remaining_search_depth);
    if (result.found) {
      if (result.tr_table_entry.ConfirmationKeysMatchExpectedKeys(expected_keys)) {
        result.known_collision = true;
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
    data_.insert_or_assign(
        state,
        TranspositionTableEntry<KeyType>{
            MinimaxCalcResult{search_depth, result_type, similar_moves},
            std::vector<KeyType>{}
        }
    );
  }

  void RecordData(
      KeyType state,
      int search_depth,
      MinimaxResultType result_type,
      EqualScoreMoves &similar_moves,
      std::vector<KeyType> &confirmation_keys
  ) {
    data_.insert_or_assign(
        state,
        TranspositionTableEntry{
            MinimaxCalcResult{search_depth, result_type, similar_moves},
            confirmation_keys
        }
    );
  }

  size_t num_entries() { return data_.size(); }
  size_t num_states() { return data_.size(); }

private:
  unordered_map<KeyType, TranspositionTableEntry<KeyType>> data_;
};

template <typename KeyType>
struct DualKeyMinimaxCalcResult {
  KeyType confirmation_state;
  MinimaxCalcResult single_key_entry;
};

template <typename KeyType>
class DualKeyTranspositionTable {
public:
  TranspositionTableSearchResult GetDataAt(
      KeyType board_state,
      KeyType expected_confirmation_state,
      int remaining_search_depth
  ) {
    TranspositionTableSearchResult result{};

    // auto dual_key_entry_vector_it = data_.find(board_state);
    auto dual_key_tr_table_entry_it = data_.find(board_state);
    if (dual_key_tr_table_entry_it != data_.end()) {
      auto dual_key_tr_table_entry = dual_key_tr_table_entry_it->second;
      if (dual_key_tr_table_entry.single_key_entry.remaining_search_depth >=
          remaining_search_depth) {
        result.found = true;
        result.table_entry = dual_key_tr_table_entry.single_key_entry;
        if (dual_key_tr_table_entry.confirmation_state != expected_confirmation_state) {
          result.known_collision = true;
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
    MinimaxCalcResult single_key_entry{search_depth, result_type, similar_moves};
    data_.insert_or_assign(
        board_state,
        DualKeyMinimaxCalcResult<KeyType>{confirmation_state, single_key_entry}
    );
  }

  uint64_t num_entries() { return data_.size(); }
  size_t num_states() { return data_.size(); }

private:
  unordered_map<KeyType, DualKeyMinimaxCalcResult<KeyType>> data_;
};

//! Tracks board state using one boardstate::ZobristCalculator and a Implements
//! BoardStateSummarizer interface; calculates Zobrist hash value of board configuration;
//! provides moveselection::MinimaxMoveEvaluator access to boardstate::TranspositionTable
template <typename KeyType>
class SingleZobristTracker
    : public BoardStateSummarizer<SingleZobristTracker<KeyType>, KeyType> {
public:
  SingleZobristTracker(ZobristCalculator<KeyType> calculator)
      : calculator_{calculator}
      , transposition_table_{} {};

  SingleZobristTracker()
      : calculator_{}
      , transposition_table_{} {};

  SingleZobristTracker(uint32_t seed)
      : SingleZobristTracker(ZobristCalculator<KeyType>(seed)) {};

  KeyType ImplementGetState() { return calculator_.board_state(); }

  void ImplementUpdateBoardState(const ExecutedMove &executed_move) {
    calculator_.UpdateBoardState(executed_move);
  }
  void ImplementFullBoardStateCalc(const BoardMap_t &board_map) {
    calculator_.FullBoardStateCalc(board_map);
  };
  void ImplementRecordTrData(
      int search_depth,
      MinimaxResultType result_type,
      EqualScoreMoves &similar_moves
  ) {
    transposition_table_
        .RecordData(calculator_.board_state(), search_depth, result_type, similar_moves);
  }

  TranspositionTableSearchResult ImplementGetTrData(int search_depth) {
    return transposition_table_.GetDataAt(calculator_.board_state(), search_depth);
  }

  moveselection::TranspositionTableSize ImplementGetTrTableSize() {
    moveselection::TranspositionTableSize result{
        transposition_table_.num_entries(),
        transposition_table_.num_states()
    };
    return result;
  }

  const uint32_t zkeys_seed() { return calculator_.seed(); }

  const KeyType board_state() { return calculator_.board_state(); }

  const string board_state_hex_str() {
    return boardstate::IntToHexString(board_state());
  }

private:
  ZobristCalculator<KeyType> calculator_;
  SingleKeyTranspositionTable<KeyType> transposition_table_;
};

template <typename KeyType>
class DualZobristTracker
    : public BoardStateSummarizer<DualZobristTracker<KeyType>, KeyType> {
public:
  DualZobristTracker(
      ZobristCalculator<KeyType> primary_calculator,
      ZobristCalculator<KeyType> confirmation_calculator
  )
      : primary_calculator_{primary_calculator}
      , confirmation_calculator_{confirmation_calculator}
      , transposition_table_{}
      , zkeys_seed_{} {}

  DualZobristTracker(uint32_t zkeys_seed)
      : transposition_table_{}
      , zkeys_seed_{zkeys_seed} {
    std::mt19937 calculator_seed_generator{zkeys_seed_};
    primary_calculator_ =
        ZobristCalculator<KeyType>{(uint32_t)calculator_seed_generator()};
    confirmation_calculator_ =
        ZobristCalculator<KeyType>{(uint32_t)calculator_seed_generator()};
  }

  DualZobristTracker()
      : DualZobristTracker(random_device{}()) {}

  KeyType ImplementGetState() { return primary_calculator_.board_state(); }

  void ImplementUpdateBoardState(const ExecutedMove &executed_move) {
    primary_calculator_.UpdateBoardState(executed_move);
    confirmation_calculator_.UpdateBoardState(executed_move);
  }

  void ImplementFullBoardStateCalc(const BoardMap_t &board_map) {
    primary_calculator_.FullBoardStateCalc(board_map);
    confirmation_calculator_.FullBoardStateCalc(board_map);
  }

  void ImplementRecordTrData(
      int search_depth,
      MinimaxResultType result_type,
      EqualScoreMoves &similar_moves
  ) {
    transposition_table_.RecordData(
        primary_calculator_.board_state(),
        confirmation_calculator_.board_state(),
        search_depth,
        result_type,
        similar_moves
    );
  }

  TranspositionTableSearchResult ImplementGetTrData(int search_depth) {
    return transposition_table_.GetDataAt(
        primary_calculator_.board_state(),
        confirmation_calculator_.board_state(),
        search_depth
    );
  }

  moveselection::TranspositionTableSize ImplementGetTrTableSize() {
    moveselection::TranspositionTableSize result{
        transposition_table_.num_entries(),
        transposition_table_.num_states()
    };
    return result;
  }

  const uint32_t zkeys_seed() { return zkeys_seed_; }
  const uint32_t primary_calculator_seed() { return primary_calculator_.seed(); }
  const uint32_t confirmation_calculator_seed() {
    return confirmation_calculator_.seed();
  }
  const KeyType board_state() { return primary_calculator_.board_state(); }
  const string board_state_hex_str() {
    return boardstate::IntToHexString(board_state());
  }

private:
  uint32_t zkeys_seed_;
  ZobristCalculator<KeyType> primary_calculator_;
  ZobristCalculator<KeyType> confirmation_calculator_;
  DualKeyTranspositionTable<KeyType> transposition_table_;
};

template <typename KeyType>
class ZobristTracker : public BoardStateSummarizer<ZobristTracker<KeyType>, KeyType> {
public:
  explicit ZobristTracker(ZobristComponent<KeyType> zobrist_component)
      : zobrist_component_{std::move(zobrist_component)} {}

  ZobristTracker(int num_calculators)
      : ZobristTracker(ZobristComponent<KeyType>(num_calculators)) {}

  ZobristTracker(std::vector<uint32_t> seeds)
      : ZobristTracker(ZobristComponent<KeyType>(seeds)) {}

  ZobristTracker()
      : ZobristTracker(2) {}

  KeyType ImplementGetState() { return zobrist_component_.GetPrimaryBoardState(); }

  void ImplementUpdateBoardState(const ExecutedMove &executed_move) {
    zobrist_component_.UpdateBoardStates(executed_move);
  }

  void ImplementFullBoardStateCalc(const BoardMap_t &board_map) {
    zobrist_component_.FullBoardStateCalc(board_map);
  }

  void ImplementRecordTrData(
      int search_depth,
      MinimaxResultType result_type,
      EqualScoreMoves &similar_move
  ) {
    transposition_table_.RecordData(
        zobrist_component_.GetPrimaryBoardState(),
        search_depth,
        result_type,
        similar_move
    );
  }

  TranspositionTableSearchResult ImplementGetTrData(int search_depth) {
    return transposition_table_.GetDataAt(
        zobrist_component_.GetPrimaryBoardState(),
        search_depth
    );
  }

  TranspositionTableSearchResult ImplementGetTrData(
      int search_depth,
      const std::vector<KeyType> &expected_keys
  ) {
    return transposition_table_.GetDataAt(
        zobrist_component_.GetPrimaryBoardState(),
        search_depth,
        expected_keys
    );
  }

  moveselection::TranspositionTableSize ImplementGetTrTableSize() {
    moveselection::TranspositionTableSize result{
        transposition_table_.num_entries(),
        transposition_table_.num_states()
    };
    return result;
  }

private:
  ZobristComponent<KeyType> zobrist_component_;
  TranspositionTable<KeyType> transposition_table_;
};

} // namespace boardstate
