
//! @file zobrist.hpp
//! Contains implementaion of BoardStateTracker interface as a boardstate::ZobristTracker
//! and supporting classes.

#pragma once

#include <array>
#include <atomic>
#include <board_data_structs.hpp>
#include <chrono>
#include <key_generator.hpp>
#include <move_data_structs.hpp>
#include <mutex>
#include <optional>
#include <random>
#include <shared_mutex>
#include <vector>

namespace boardstate {

//! Uses Zobrist hashing to calculate a "reasonably unique" integer value
//! for each board configuration encountered during a game. KeyType can be any unsigned
//! integer type with a size = (n * 32 bits) where n is an integer >= 1.
template <typename KeyType>
class ZobristCalculator {
public:
  typedef array<array<KeyType, gameboard::kNumFiles>, gameboard::kNumRanks>
      PieceZarray_t;
  typedef array<PieceZarray_t, gameboard::kNumPieceTypeVals> TeamZarray_t;
  typedef array<TeamZarray_t, 2> GameZarray_t;

  ZobristCalculator(uint32_t seed)
      : zarray_{}
      , turn_key_{}
      , board_state_{}
      , seed_{seed} {
    PseudoRandomKeyGenerator<KeyType> key_generator{seed};
    turn_key_ = key_generator.GenerateKey();
    zarray_ = CreateGameZarray(key_generator);
  };

  ZobristCalculator()
      : ZobristCalculator(random_device{}()) {}

  void FullBoardStateCalc(const gameboard::BoardMap_t &board_map) {
    board_state_ = 0;
    for (size_t rank = 0; rank < gameboard::kNumRanks; rank++) {
      for (size_t file = 0; file < gameboard::kNumFiles; file++) {
        if (board_map[rank][file].piece_color != 0) {
          board_state_ = board_state_ ^ GetHashValueAt(
                                            board_map[rank][file].piece_color,
                                            board_map[rank][file].piece_type,
                                            gameboard::BoardSpace{(int)rank, (int)file}
                                        );
        }
      }
    }
  }

  void UpdateBoardState(const gameboard::ExecutedMove &executed_move) {
    board_state_ = board_state_ ^ GetHashValueAt(
                                      executed_move.moving_piece.piece_color,
                                      executed_move.moving_piece.piece_type,
                                      executed_move.spaces.start
                                  );

    // if capture piece, remove from board
    if (executed_move.destination_piece.piece_color != gameboard::PieceColor::kNul) {
      board_state_ = board_state_ ^ GetHashValueAt(
                                        executed_move.destination_piece.piece_color,
                                        executed_move.destination_piece.piece_type,
                                        executed_move.spaces.end
                                    );
    }

    // moving piece to new space
    board_state_ = board_state_ ^ GetHashValueAt(
                                      executed_move.moving_piece.piece_color,
                                      executed_move.moving_piece.piece_type,
                                      executed_move.spaces.end
                                  );

    // change state now that its other player's turn
    board_state_ = board_state_ ^ turn_key();
  }

  KeyType GetHashValueAt(PieceColor color, PieceType piece_type, BoardSpace space) {
    return zarray_[GetZColorIndexOf(color)][piece_type][space.rank][space.file];
  }

  KeyType board_state() const { return board_state_; }
  GameZarray_t zarray() const { return zarray_; }
  KeyType turn_key() const { return turn_key_; }
  uint32_t seed() const { return seed_; }

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

//! Container for one or more boardstate::ZobristCalculator objects.
//! Using more than one boardstate::ZobristCalculator (i.e. NumConfKeys > 0) allows hash
//! collisions to be detected.
template <typename KeyType, size_t NumConfKeys>
class ZobristComponent {
public:
  explicit ZobristComponent(
      const ZobristCalculator<KeyType> primary_calculator,
      const std::array<ZobristCalculator<KeyType>, NumConfKeys> &confirmation_calculators
  )
      : primary_calculator_{primary_calculator}
      , confirmation_calculators_(confirmation_calculators) {}

  ZobristComponent()
      : primary_calculator_{ZobristCalculator<KeyType>()}
      , confirmation_calculators_{} {

    for (auto i = 0; i < NumConfKeys; ++i) {
      confirmation_calculators_[i] = ZobristCalculator<KeyType>();
    }
  }

  explicit ZobristComponent(
      uint32_t primary_seed,
      std::array<uint32_t, NumConfKeys> confirmation_seeds
  )
      : primary_calculator_{ZobristCalculator<KeyType>{primary_seed}}
      , confirmation_calculators_{} {
    for (auto i = 0; i < NumConfKeys; ++i) {
      confirmation_calculators_[i] = ZobristCalculator<KeyType>{confirmation_seeds[i]};
    }
  }

  explicit ZobristComponent(std::mt19937 prng)
      : primary_calculator_{(uint32_t)prng()}
      , confirmation_calculators_{} {
    for (auto i = 0; i < NumConfKeys; i++) {
      confirmation_calculators_[i] = ZobristCalculator<KeyType>((uint32_t)prng());
    }
  }

  explicit ZobristComponent(uint32_t prng_seed)
      : ZobristComponent(std::mt19937{prng_seed}) {
    prng_seed_ = prng_seed;
  }

  void UpdateBoardStates(const ExecutedMove &executed_move) {
    primary_calculator_.UpdateBoardState(executed_move);
    for (auto &calculator : confirmation_calculators_) {
      calculator.UpdateBoardState(executed_move);
    }
  }

  void FullBoardStateCalc(const BoardMap_t &board_map) {
    primary_calculator_.FullBoardStateCalc(board_map);
    for (auto &calculator : confirmation_calculators_) {
      calculator.FullBoardStateCalc(board_map);
    }
  }

  KeyType primary_board_state() { return primary_calculator_.board_state(); }

  std::array<KeyType, NumConfKeys> confirmation_board_states() {
    std::array<KeyType, NumConfKeys> confirmation_states;
    for (auto i = 0; i < NumConfKeys; ++i) {
      confirmation_states[i] = confirmation_calculators_[i].board_state();
    }
    return confirmation_states;
  }

  KeyType primary_calculator_seed() { return primary_calculator_.seed(); }

  std::array<uint32_t, NumConfKeys> confirmation_calculator_seeds() const {
    std::array<uint32_t, NumConfKeys> seeds;
    for (auto i = 0; i < NumConfKeys; ++i) {
      seeds[i] = confirmation_calculators_[i].seed();
    }
  }

  std::string primary_board_state_hex_str() const {
    return boardstate::IntToHexString(primary_calculator_.board_state());
  }

  uint32_t prng_seed() { return prng_seed_.value_or(0); }

private:
  ZobristCalculator<KeyType> primary_calculator_;
  std::array<ZobristCalculator<KeyType>, NumConfKeys> confirmation_calculators_;
  std::optional<uint32_t> prng_seed_;
};

//! Data structure to hold calculation results that get entered into a
//! boardstate::TranspositionTable.
//! When NumConfKeys > 0, also holds board state values calculated by objects in a
//! boardstate::ZobristComponent.confirmation_calculators_.
template <typename KeyType, size_t NumConfKeys>
class TranspositionTableEntry {
public:
  TranspositionTableEntry(
      moveselection::MinimaxCalcResult minimax_calc_result,
      std::array<KeyType, NumConfKeys> confirmation_keys,
      MoveCountType last_access_index
  )
      : minimax_calc_result_{minimax_calc_result}
      , confirmation_keys_{confirmation_keys}
      , last_access_index_{last_access_index} {}

  moveselection::MinimaxCalcResult minimax_calc_result() { return minimax_calc_result_; }

  std::array<KeyType, NumConfKeys> confirmation_keys() { return confirmation_keys_; }

  MoveCountType last_access_index() { return last_access_index_; }

  void set_last_access_index(MoveCountType last_access_index) {
    last_access_index_ = last_access_index;
  }

  bool ConfirmationKeysMatchExpected(std::array<KeyType, NumConfKeys> expected_keys) {
    for (auto i = 0; i < NumConfKeys; ++i) {
      if (expected_keys[i] != confirmation_keys_[i]) {
        return false;
      }
    }
    return true;
  }

  DepthType remaining_search_depth() {
    return minimax_calc_result_.remaining_search_depth();
  }

private:
  moveselection::MinimaxCalcResult minimax_calc_result_;
  std::array<KeyType, NumConfKeys> confirmation_keys_;
  MoveCountType last_access_index_;
};

//! Stores and manages key-value pairs consisting of a board_state (from a
//! boardstate::ZobristComponent) and results of Minimax calculations performed by
//! boardstate::MinimaxMoveEvaluator. Provides read/write access to
//! moveselection::MinimaxMoveEvaluator via a boardstate::ZobristSummarizer.
template <typename KeyType, size_t NumConfKeys>
class TranspositionTable {
public:
  moveselection::TranspositionTableSearchResult GetDataAt(
      KeyType primary_board_state,
      DepthType remaining_search_depth,
      std::array<KeyType, NumConfKeys> expected_keys
  ) {
    moveselection::TranspositionTableSearchResult result{};
    auto tr_table_entry_it = data_.find(primary_board_state);
    if (tr_table_entry_it != data_.end()) {
      auto tr_table_entry = tr_table_entry_it->second;
      if (tr_table_entry.remaining_search_depth() >= remaining_search_depth) {
        tr_table_entry.set_last_access_index(move_counter_);
        result.set_found(true);
        result.set_minimax_calc_result(tr_table_entry.minimax_calc_result());
      }
      if (result.found() and
          !tr_table_entry.ConfirmationKeysMatchExpected(expected_keys)) {
        result.set_known_collision(true);
      }
    }
    return result;
  }

  void RecordData(
      KeyType primary_board_state,
      DepthType search_depth,
      moveselection::MinimaxResultType result_type,
      moveselection::EqualScoreMoves &similar_moves,
      const std::array<KeyType, NumConfKeys> &confirmation_keys
  ) {
    data_.insert_or_assign(
        primary_board_state,
        TranspositionTableEntry<KeyType, NumConfKeys>{
            moveselection::MinimaxCalcResult{search_depth, result_type, similar_moves},
            confirmation_keys,
            move_counter_
        }
    );
  }

  size_t size() { return data_.size(); }

  void IncrementMoveCounter() { move_counter_++; }

private:
  std::unordered_map<KeyType, TranspositionTableEntry<KeyType, NumConfKeys>> data_;
  MoveCountType move_counter_;

  MoveCountType NumMovesSinceLastUseOf(
      const TranspositionTableEntry<KeyType, NumConfKeys> &tr_table_entry
  ) {
    return move_counter_ - tr_table_entry.last_access_index();
  }
};

//! Implements the BoardStateSummarizer interface, providing a
//! moveselection::MinimaxMoveEvaluator with "reasonably unique" hash values for each
//! encountered board state, and the ability to read/write board_state-MinimaxResult
//! pairs in a boardstate::TranspositionTable.
template <typename KeyType, size_t NumConfKeys>
class ZobristSummarizer
    : public BoardStateSummarizer<ZobristSummarizer<KeyType, NumConfKeys>, KeyType> {
public:
  explicit ZobristSummarizer(ZobristComponent<KeyType, NumConfKeys> zobrist_component)
      : zobrist_component_{std::move(zobrist_component)}
      , transposition_table_{} {}

  explicit ZobristSummarizer(
      uint32_t primary_seed,
      std::array<uint32_t, NumConfKeys> confirmation_seeds
  )
      : ZobristSummarizer(
            ZobristComponent<KeyType, NumConfKeys>{primary_seed, confirmation_seeds}
        ) {}

  explicit ZobristSummarizer(uint32_t prng_seed)
      : ZobristSummarizer(ZobristComponent<KeyType, NumConfKeys>{prng_seed}) {}

  ZobristSummarizer()
      : zobrist_component_{}
      , transposition_table_{} {}

  KeyType ImplementGetState() { return zobrist_component_.primary_board_state(); }

  void ImplementUpdateBoardState(const ExecutedMove &executed_move) {
    zobrist_component_.UpdateBoardStates(executed_move);
  }

  void ImplementFullBoardStateCalc(const BoardMap_t &board_map) {
    zobrist_component_.FullBoardStateCalc(board_map);
  }

  void ImplementRecordTrData(
      DepthType search_depth,
      moveselection::MinimaxResultType result_type,
      moveselection::EqualScoreMoves &similar_moves,
      MoveCountType access_index
  ) {
    transposition_table_.RecordData(
        zobrist_component_.primary_board_state(),
        search_depth,
        result_type,
        similar_moves,
        zobrist_component_.confirmation_board_states()
    );
  }

  moveselection::TranspositionTableSearchResult ImplementGetTrData(
      DepthType search_depth,
      MoveCountType access_index
  ) {
    return transposition_table_.GetDataAt(
        zobrist_component_.primary_board_state(),
        search_depth,
        zobrist_component_.confirmation_board_states()
    );
  }

  size_t ImplementGetTrTableSize() { return transposition_table_.size(); }

  void ImplementUpdateMoveCounter() { transposition_table_.IncrementMoveCounter(); }

  const std::string board_state_hex_str() {
    return IntToHexString(zobrist_component_.primary_board_state());
  }

  uint32_t zkeys_seed() { return zobrist_component_.prng_seed(); }

private:
  ZobristComponent<KeyType, NumConfKeys> zobrist_component_;
  TranspositionTable<KeyType, NumConfKeys> transposition_table_;
};

} // namespace boardstate