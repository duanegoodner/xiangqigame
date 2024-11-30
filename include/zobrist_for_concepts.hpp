#pragma once

#include <array>
#include <atomic>
#include <board_data_structs.hpp>
#include <chrono>
#include <evaluator_data_structs.hpp>
#include <key_generator.hpp>
#include <memory>
#include <move_data_structs.hpp>
#include <mutex>
#include <optional>
#include <random>
#include <shared_mutex>
#include <thread>
#include <vector>
#include <zobrist_calculator_for_concepts.hpp>

namespace boardstate {

//! Container for one or more boardstate::ZobristCalculatorForConcepts objects.
//! Using more than one boardstate::ZobristCalculatorForConcepts (i.e. NumConfKeys > 0)
//! allows hash collisions to be detected.
template <typename KeyType, size_t NumConfKeys>
class ZobristComponentForConcepts {
private:
  std::shared_ptr<ZobristCalculatorForConcepts<KeyType>> primary_calculator_;
  std::array<std::shared_ptr<ZobristCalculatorForConcepts<KeyType>>, NumConfKeys>
      confirmation_calculators_;
  std::optional<uint32_t> prng_seed_;

public:
  static std::shared_ptr<ZobristComponentForConcepts<KeyType, NumConfKeys>>
  CreateFromSeed(uint32_t prng_seed = std::random_device{}()) {
    std::mt19937 prng{prng_seed};
    auto primary_calculator =
        ZobristCalculatorForConcepts<KeyType>::Create((uint32_t)prng());
    std::array<std::shared_ptr<ZobristCalculatorForConcepts<KeyType>>, NumConfKeys>
        confirmation_calculators;
    for (auto idx = 0; idx < NumConfKeys; ++idx) {
      confirmation_calculators[idx] =
          ZobristCalculatorForConcepts<KeyType>::Create((uint32_t)prng());
    }

    return std::shared_ptr<ZobristComponentForConcepts<KeyType, NumConfKeys>>(
        new ZobristComponentForConcepts<KeyType, NumConfKeys>(
            primary_calculator,
            confirmation_calculators,
            prng_seed
        )
    );
  }

  static std::shared_ptr<ZobristComponentForConcepts<KeyType, NumConfKeys>>
  CreateFromCalculators(
      std::shared_ptr<ZobristCalculatorForConcepts<KeyType>> primary_calculator,
      std::array<std::shared_ptr<ZobristCalculatorForConcepts<KeyType>>, NumConfKeys>
          confirmation_calculators
  ) {
    return std::shared_ptr<ZobristComponentForConcepts<KeyType, NumConfKeys>>(
        new ZobristComponentForConcepts<KeyType, NumConfKeys>(
            primary_calculator,
            confirmation_calculators
        )
    );
  }

  // Getters
  KeyType primary_board_state() { return primary_calculator_->board_state(); }
  std::array<KeyType, NumConfKeys> confirmation_board_states() {
    return confirmation_board_states_internal();
  }
  KeyType primary_calculator_seed() { return primary_calculator_->seed(); }
  std::array<uint32_t, NumConfKeys> confirmation_calculator_seeds() const {
    return confirmation_calculator_seeds_internal();
  }
  std::string primary_board_state_hex_str() const {
    return boardstate::IntToHexString(primary_calculator_->board_state());
  }
  uint32_t prng_seed() { return prng_seed_.value_or(0); }

private:
  //! Constructs ZobristComponentForConcepts from existing ZobristCalculatorForConcepts
  //! objects
  explicit ZobristComponentForConcepts(
      std::shared_ptr<ZobristCalculatorForConcepts<KeyType>> primary_calculator,
      std::array<std::shared_ptr<ZobristCalculatorForConcepts<KeyType>>, NumConfKeys>
          confirmation_calculators,
      uint32_t prng_seed = 0
  )
      : primary_calculator_{primary_calculator}
      , confirmation_calculators_{confirmation_calculators}
      , prng_seed_{prng_seed} {}

  // Internal getters
  std::array<KeyType, NumConfKeys> confirmation_board_states_internal() {
    std::array<KeyType, NumConfKeys> confirmation_states;
    for (auto i = 0; i < NumConfKeys; ++i) {
      confirmation_states[i] = confirmation_calculators_[i]->board_state();
    }
    return confirmation_states;
  }

  std::array<uint32_t, NumConfKeys> confirmation_calculator_seeds_internal() const {
    std::array<uint32_t, NumConfKeys> seeds;
    for (auto i = 0; i < NumConfKeys; ++i) {
      seeds[i] = confirmation_calculators_[i].seed();
    }
  }
};

//! Data structure to hold calculation results that get entered into a
//! boardstate::TranspositionTableForConcepts.
//! When NumConfKeys > 0, also holds board state values calculated by objects in a
//! boardstate::ZobristComponent.confirmation_calculators_.
template <typename KeyType, size_t NumConfKeys>
class TranspositionTableEntryForConcepts {
private:
  moveselection::MinimaxCalcResult minimax_calc_result_;
  std::array<KeyType, NumConfKeys> confirmation_keys_;
  MoveCountType last_access_index_;

public:
  TranspositionTableEntryForConcepts(
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
};

//! Stores and manages key-value pairs consisting of a board_state (from a
//! boardstate::ZobristComponent) and results of Minimax calculations performed by
//! boardstate::MinimaxMoveEvaluator. Provides read/write access to
//! moveselection::MinimaxMoveEvaluator via a boardstate::ZobristCoordinator.
template <typename KeyType, size_t NumConfKeys>
class TranspositionTableForConcepts {
  std::unordered_map<KeyType, TranspositionTableEntryForConcepts<KeyType, NumConfKeys>>
      data_;
  MoveCountType move_counter_;

public:
  TranspositionTableForConcepts() = default;

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
        TranspositionTableEntryForConcepts<KeyType, NumConfKeys>{
            moveselection::MinimaxCalcResult{search_depth, result_type, similar_moves},
            confirmation_keys,
            move_counter_
        }
    );
  }

  size_t size() { return data_.size(); }

  void IncrementMoveCounter() { move_counter_++; }

  MoveCountType NumIdleMovesAt(KeyType board_state) {
    auto tr_table_entry_it = data_.at(board_state);
    return NumMovesSinceLastUseOf(tr_table_entry_it->second);
  }

private:
  MoveCountType NumMovesSinceLastUseOf(
      const TranspositionTableEntryForConcepts<KeyType, NumConfKeys> &tr_table_entry
  ) {
    return move_counter_ - tr_table_entry.last_access_index();
  }
};

//! Contains std::mutex that other classes lock before accessing
//! TranspositionTableForConcepts
class TranspositionTableGuardForConcepts {
  mutable std::mutex tr_table_mutex_;

public:
  TranspositionTableGuardForConcepts() = default;
  TranspositionTableGuardForConcepts(const TranspositionTableGuardForConcepts &) =
      delete;
  TranspositionTableGuardForConcepts &operator=(const TranspositionTableGuardForConcepts
                                                    &) = delete;

  std::unique_lock<std::mutex> GetExclusiveLock() {
    return std::unique_lock<std::mutex>(tr_table_mutex_);
  }
};

//! Removes old entries from TranspositionTableForConcepts to prevent excessive memory
//! use.
template <typename KeyType, size_t NumConfKeys>
class TranspositionTablePrunerForConcepts {
  TranspositionTableForConcepts<KeyType, NumConfKeys> &tr_table_;
  TranspositionTableGuardForConcepts &tr_table_guard_;
  std::thread pruning_thread_;
  std::atomic<bool> keep_running_;

public:
  TranspositionTablePrunerForConcepts(const TranspositionTablePrunerForConcepts &) =
      delete;
  TranspositionTablePrunerForConcepts &
  operator=(const TranspositionTablePrunerForConcepts &) = delete;

  TranspositionTablePrunerForConcepts(
      TranspositionTableForConcepts<KeyType, NumConfKeys> &tr_table,
      TranspositionTableGuardForConcepts &tr_table_guard
  )
      : tr_table_{tr_table}
      , tr_table_guard_{tr_table_guard}
      , pruning_thread_{}
      , keep_running_{true} {}

  ~TranspositionTablePrunerForConcepts() {
    if (pruning_thread_.joinable()) {
      pruning_thread_.join();
    }
  }

  void Start() {
    pruning_thread_ =
        std::thread(&TranspositionTablePrunerForConcepts::RepeatedlyPrune, this);
  }

  void Stop() { keep_running_ = false; }

  // void IncrementMoveCounter() { move_count_++; }

private:
  void UnsafePruneEntry() {
    // TODO: Implement me!
    std::cout << "Pruning!" << std::endl;
  }

  void ThreadSafePruneEntry() {
    std::this_thread::sleep_for(std::chrono::microseconds(200));
    auto lock = tr_table_guard_.GetExclusiveLock();
    UnsafePruneEntry();
  }

  void RepeatedlyPrune() {
    while (keep_running_) {
      ThreadSafePruneEntry();
    }
  }
};

template <typename KeyType, size_t NumConfKeys>
class ZobristCoordinatorForConcepts {
  ZobristComponentForConcepts<KeyType, NumConfKeys> zobrist_component_;
  TranspositionTableForConcepts<KeyType, NumConfKeys> tr_table_;
  TranspositionTableGuardForConcepts tr_table_guard_;
  TranspositionTablePrunerForConcepts<KeyType, NumConfKeys> tr_table_pruner_;

public:
  ZobristCoordinatorForConcepts(const ZobristCoordinatorForConcepts &) = delete;
  ZobristCoordinatorForConcepts &operator=(const ZobristCoordinatorForConcepts &) =
      delete;

  explicit ZobristCoordinatorForConcepts(
      ZobristComponentForConcepts<KeyType, NumConfKeys> zobrist_component
  )
      : zobrist_component_{std::move(zobrist_component)}
      , tr_table_{}
      , tr_table_guard_{}
      , tr_table_pruner_{TranspositionTablePrunerForConcepts{tr_table_, tr_table_guard_}
        } {
    // tr_table_pruner_.Start();
  }



  KeyType GetState() { return zobrist_component_.primary_board_state(); }
  // void UpdateBoardState(const ExecutedMove &executed_move) {
  //   zobrist_component_.UpdateBoardStates(executed_move);
  // }
  // void FullBoardStateCalc(const BoardMap_t &board_map) {
  //   zobrist_component_.FullBoardStateCalc(board_map);
  // }
  void RecordTrData(
      DepthType search_depth,
      moveselection::MinimaxResultType result_type,
      moveselection::EqualScoreMoves &similar_moves,
      MoveCountType access_index
  ) {
    tr_table_.RecordData(
        zobrist_component_.primary_board_state(),
        search_depth,
        result_type,
        similar_moves,
        zobrist_component_.confirmation_board_states()
    );
  }
  moveselection::TranspositionTableSearchResult GetTrData(
      DepthType search_depth,
      MoveCountType access_index
  ) {
    return tr_table_.GetDataAt(
        zobrist_component_.primary_board_state(),
        search_depth,
        zobrist_component_.confirmation_board_states()
    );
  }
  size_t GetTrTableSize() { return tr_table_.size(); }

  void UpdateMoveCounter() {
    // tr_table_pruner_.IncrementMoveCounter();
    tr_table_.IncrementMoveCounter();
  }

  const std::string board_state_hex_str() {
    return IntToHexString(zobrist_component_.primary_board_state());
  }

  uint32_t zkeys_seed() { return zobrist_component_.prng_seed(); }
};

} // namespace boardstate
