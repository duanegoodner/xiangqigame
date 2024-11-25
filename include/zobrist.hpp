
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
#include <thread>
#include <vector>

//! Calculate / manage board state and associate Minimax results.
namespace boardstate {

//! Uses Zobrist hashing to calculate a "reasonably unique" integer value
//! for each board configuration encountered during a game. KeyType can be any unsigned
//! integer type with a size = (n * 32 bits) where n is an integer >= 1.
template <typename KeyType>
class ZobristCalculator {
private:
  using PieceZarray_t =
      array<array<KeyType, gameboard::kNumFiles>, gameboard::kNumRanks>;
  using TeamZarray_t = array<PieceZarray_t, gameboard::kNumPieceTypeVals>;
  using GameZarray_t = array<TeamZarray_t, 2>;

  GameZarray_t zarray_;
  KeyType turn_key_;
  uint32_t seed_;
  KeyType board_state_;

public:
  //! Constructs a ZobristCalculator.
  //! @param seed Integer used as a seed for a PRNG that generates Zobrist key values.
  ZobristCalculator(uint32_t seed = std::random_device{}())
      : zarray_{}
      , turn_key_{}
      , board_state_{}
      , seed_{seed} {
    PseudoRandomKeyGenerator<KeyType> key_generator{seed};
    turn_key_ = key_generator.GenerateKey();
    zarray_ = CreateGameZarray(key_generator);
  };

  // Getters
  KeyType board_state() const { return board_state_; }
  GameZarray_t zarray() const { return zarray_; }
  KeyType turn_key() const { return turn_key_; }
  uint32_t seed() const { return seed_; }

  // Calculation methods

  void FullBoardStateCalc(const gameboard::BoardMap_t &board_map) {
    FullBoardStateCalInternal(board_map);
  }

  void UpdateBoardState(const gameboard::ExecutedMove &executed_move) {
    UpdateBoardStateInternal(executed_move);
  }

  KeyType GetHashValueAt(PieceColor color, PieceType piece_type, BoardSpace space) {
    return zarray_[GetZColorIndexOf(color)][piece_type][space.rank][space.file];
  }

private:
  //! Static helper method for building 4-D array of Zobrist keys in constuctor.
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
  }

  // Calculation methods

  void FullBoardStateCalInternal(const gameboard::BoardMap_t &board_map) {
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

  void UpdateBoardStateInternal(const gameboard::ExecutedMove &executed_move) {
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
};

// //! Container for one or more boardstate::ZobristCalculator objects.
// //! Using more than one boardstate::ZobristCalculator (i.e. NumConfKeys > 0) allows hash
// //! collisions to be detected.
// template <typename KeyType, size_t NumConfKeys>
// class ZobristComponentNew {
// private:
//   ZobristCalculator<KeyType> &primary_calculator_;
//   std::array<ZobristCalculator<KeyType>, NumConfKeys> &confirmation_calculators_;
//   std::optional<uint32_t> prng_seed_;

// public:
//   //! Constructs ZobristComponent from existing ZobristCalculator objects
//   explicit ZobristComponentNew(
//       ZobristCalculator<KeyType> &primary_calculator,
//       std::array<ZobristCalculator<KeyType>, NumConfKeys> &confirmation_calculators
//   )
//       : primary_calculator_{primary_calculator}
//       , confirmation_calculators_(confirmation_calculators) {}

//   // Getters
//   KeyType primary_board_state() { return primary_calculator_.board_state(); }
//   std::array<KeyType, NumConfKeys> confirmation_board_states() {
//     return confirmation_board_states_internal();
//   }
//   KeyType primary_calculator_seed() { return primary_calculator_.seed(); }
//   std::array<uint32_t, NumConfKeys> confirmation_calculator_seeds() const {
//     return confirmation_calculator_seeds_internal();
//   }
//   std::string primary_board_state_hex_str() const {
//     return boardstate::IntToHexString(primary_calculator_.board_state());
//   }
//   uint32_t prng_seed() { return prng_seed_.value_or(0); }

//   // Calculation methods
//   void UpdateBoardStates(const ExecutedMove &executed_move) {
//     UpdateBoardStatesInternal(executed_move);
//   }

//   void FullBoardStateCalc(const BoardMap_t &board_map) {
//     FullBoardStateCalcInternal(board_map);
//   }

// private:
//   // Internal getters
//   std::array<KeyType, NumConfKeys> confirmation_board_states_internal() {
//     std::array<KeyType, NumConfKeys> confirmation_states;
//     for (auto i = 0; i < NumConfKeys; ++i) {
//       confirmation_states[i] = confirmation_calculators_[i].board_state();
//     }
//     return confirmation_states;
//   }

//   std::array<uint32_t, NumConfKeys> confirmation_calculator_seeds_internal() const {
//     std::array<uint32_t, NumConfKeys> seeds;
//     for (auto i = 0; i < NumConfKeys; ++i) {
//       seeds[i] = confirmation_calculators_[i].seed();
//     }
//   }

//   // Internal calculation methods
//   void UpdateBoardStatesInternal(const ExecutedMove &executed_move) {
//     primary_calculator_.UpdateBoardState(executed_move);
//     for (auto &calculator : confirmation_calculators_) {
//       calculator.UpdateBoardState(executed_move);
//     }
//   }

//   void FullBoardStateCalcInternal(const BoardMap_t &board_map) {
//     primary_calculator_.FullBoardStateCalc(board_map);
//     for (auto &calculator : confirmation_calculators_) {
//       calculator.FullBoardStateCalc(board_map);
//     }
//   }
// };

//! Container for one or more boardstate::ZobristCalculator objects.
//! Using more than one boardstate::ZobristCalculator (i.e. NumConfKeys > 0) allows hash
//! collisions to be detected.
template <typename KeyType, size_t NumConfKeys>
class ZobristComponent {
private:
  ZobristCalculator<KeyType> primary_calculator_;
  std::array<ZobristCalculator<KeyType>, NumConfKeys> confirmation_calculators_;
  std::optional<uint32_t> prng_seed_;

public:
  //! Constructs ZobristComponent from existing ZobristCalculator objects
  explicit ZobristComponent(
      const ZobristCalculator<KeyType> primary_calculator,
      const std::array<ZobristCalculator<KeyType>, NumConfKeys> &confirmation_calculators
  )
      : primary_calculator_{primary_calculator}
      , confirmation_calculators_(confirmation_calculators) {}

  //! Constructs ZobristComponent using 32-bit unsigned in as a PRNG seed.
  explicit ZobristComponent(uint32_t prng_seed = std::random_device{}())
      : ZobristComponent(std::mt19937{prng_seed}) {
    prng_seed_ = prng_seed;
  }

  // Getters
  KeyType primary_board_state() { return primary_calculator_.board_state(); }
  std::array<KeyType, NumConfKeys> confirmation_board_states() {
    return confirmation_board_states_internal();
  }
  KeyType primary_calculator_seed() { return primary_calculator_.seed(); }
  std::array<uint32_t, NumConfKeys> confirmation_calculator_seeds() const {
    return confirmation_calculator_seeds_internal();
  }
  std::string primary_board_state_hex_str() const {
    return boardstate::IntToHexString(primary_calculator_.board_state());
  }
  uint32_t prng_seed() { return prng_seed_.value_or(0); }

  // Calculation methods
  void UpdateBoardStates(const ExecutedMove &executed_move) {
    UpdateBoardStatesInternal(executed_move);
  }

  void FullBoardStateCalc(const BoardMap_t &board_map) {
    FullBoardStateCalcInternal(board_map);
  }

private:
  //! Constructs ZobristComponent from a std::mt19937 pseudorandom number generator.
  //! Used as a helper to public constructor (via delegation).
  explicit ZobristComponent(std::mt19937 prng)
      : primary_calculator_{(uint32_t)prng()}
      , confirmation_calculators_{} {
    for (auto i = 0; i < NumConfKeys; i++) {
      confirmation_calculators_[i] = ZobristCalculator<KeyType>((uint32_t)prng());
    }
  }

  // Internal getters
  std::array<KeyType, NumConfKeys> confirmation_board_states_internal() {
    std::array<KeyType, NumConfKeys> confirmation_states;
    for (auto i = 0; i < NumConfKeys; ++i) {
      confirmation_states[i] = confirmation_calculators_[i].board_state();
    }
    return confirmation_states;
  }

  std::array<uint32_t, NumConfKeys> confirmation_calculator_seeds_internal() const {
    std::array<uint32_t, NumConfKeys> seeds;
    for (auto i = 0; i < NumConfKeys; ++i) {
      seeds[i] = confirmation_calculators_[i].seed();
    }
  }

  // Internal calculation methods
  void UpdateBoardStatesInternal(const ExecutedMove &executed_move) {
    primary_calculator_.UpdateBoardState(executed_move);
    for (auto &calculator : confirmation_calculators_) {
      calculator.UpdateBoardState(executed_move);
    }
  }

  void FullBoardStateCalcInternal(const BoardMap_t &board_map) {
    primary_calculator_.FullBoardStateCalc(board_map);
    for (auto &calculator : confirmation_calculators_) {
      calculator.FullBoardStateCalc(board_map);
    }
  }
};

//! Data structure to hold calculation results that get entered into a
//! boardstate::TranspositionTable.
//! When NumConfKeys > 0, also holds board state values calculated by objects in a
//! boardstate::ZobristComponent.confirmation_calculators_.
template <typename KeyType, size_t NumConfKeys>
class TranspositionTableEntry {
private:
  moveselection::MinimaxCalcResult minimax_calc_result_;
  std::array<KeyType, NumConfKeys> confirmation_keys_;
  MoveCountType last_access_index_;

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
};

//! Stores and manages key-value pairs consisting of a board_state (from a
//! boardstate::ZobristComponent) and results of Minimax calculations performed by
//! boardstate::MinimaxMoveEvaluator. Provides read/write access to
//! moveselection::MinimaxMoveEvaluator via a boardstate::ZobristCoordinator.
template <typename KeyType, size_t NumConfKeys>
class TranspositionTable {
  std::unordered_map<KeyType, TranspositionTableEntry<KeyType, NumConfKeys>> data_;
  MoveCountType move_counter_;

public:
  TranspositionTable() = default;

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

  MoveCountType NumIdleMovesAt(KeyType board_state) {
    auto tr_table_entry_it = data_.at(board_state);
    return NumMovesSinceLastUseOf(tr_table_entry_it->second);
  }

private:
  MoveCountType NumMovesSinceLastUseOf(
      const TranspositionTableEntry<KeyType, NumConfKeys> &tr_table_entry
  ) {
    return move_counter_ - tr_table_entry.last_access_index();
  }
};

//! Contains std::mutex that other classes lock before accessing TranspositionTable
class TranspositionTableGuard {
  mutable std::mutex tr_table_mutex_;

public:
  TranspositionTableGuard() = default;
  TranspositionTableGuard(const TranspositionTableGuard &) = delete;
  TranspositionTableGuard &operator=(const TranspositionTableGuard &) = delete;

  std::unique_lock<std::mutex> GetExclusiveLock() {
    return std::unique_lock<std::mutex>(tr_table_mutex_);
  }
};

//! Removes old entries from TranspositionTable to prevent excessive memory use.
template <typename KeyType, size_t NumConfKeys>
class TranspositionTablePruner {
  TranspositionTable<KeyType, NumConfKeys> &tr_table_;
  TranspositionTableGuard &tr_table_guard_;
  std::thread pruning_thread_;
  std::atomic<bool> keep_running_;

public:
  TranspositionTablePruner(const TranspositionTablePruner &) = delete;
  TranspositionTablePruner &operator=(const TranspositionTablePruner &) = delete;

  TranspositionTablePruner(
      TranspositionTable<KeyType, NumConfKeys> &tr_table,
      TranspositionTableGuard &tr_table_guard
  )
      : tr_table_{tr_table}
      , tr_table_guard_{tr_table_guard}
      , pruning_thread_{}
      , keep_running_{true} {}

  ~TranspositionTablePruner() {
    if (pruning_thread_.joinable()) {
      pruning_thread_.join();
    }
  }

  void Start() {
    pruning_thread_ = std::thread(&TranspositionTablePruner::RepeatedlyPrune, this);
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

//! Implements the BoardStateCoordinator interface, providing a
//! moveselection::MinimaxMoveEvaluator with "reasonably unique" hash values for each
//! encountered board state, and the ability to read/write board_state-MinimaxResult
//! pairs in a boardstate::TranspositionTable.
template <typename KeyType, size_t NumConfKeys>
class ZobristCoordinator
    : public BoardStateCoordinator<ZobristCoordinator<KeyType, NumConfKeys>, KeyType> {
  ZobristComponent<KeyType, NumConfKeys> zobrist_component_;
  TranspositionTable<KeyType, NumConfKeys> tr_table_;
  TranspositionTableGuard tr_table_guard_;
  TranspositionTablePruner<KeyType, NumConfKeys> tr_table_pruner_;

public:
  ZobristCoordinator(const ZobristCoordinator &) = delete;
  ZobristCoordinator &operator=(const ZobristCoordinator &) = delete;

  explicit ZobristCoordinator(ZobristComponent<KeyType, NumConfKeys> zobrist_component)
      : zobrist_component_{std::move(zobrist_component)}
      , tr_table_{}
      , tr_table_guard_{}
      , tr_table_pruner_{TranspositionTablePruner{tr_table_, tr_table_guard_}} {
    // tr_table_pruner_.Start();
  }

  explicit ZobristCoordinator(
      uint32_t primary_seed,
      std::array<uint32_t, NumConfKeys> confirmation_seeds
  )
      : ZobristCoordinator(
            ZobristComponent<KeyType, NumConfKeys>{primary_seed, confirmation_seeds}
        ) {}

  explicit ZobristCoordinator(uint32_t prng_seed = (uint32_t)std::random_device{}())
      : ZobristCoordinator(ZobristComponent<KeyType, NumConfKeys>{prng_seed}) {}

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
    tr_table_.RecordData(
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
    return tr_table_.GetDataAt(
        zobrist_component_.primary_board_state(),
        search_depth,
        zobrist_component_.confirmation_board_states()
    );
  }

  size_t ImplementGetTrTableSize() { return tr_table_.size(); }

  void ImplementUpdateMoveCounter() {
    // tr_table_pruner_.IncrementMoveCounter();
    tr_table_.IncrementMoveCounter();
  }

  const std::string board_state_hex_str() {
    return IntToHexString(zobrist_component_.primary_board_state());
  }

  uint32_t zkeys_seed() { return zobrist_component_.prng_seed(); }
};

// //! Implements the BoardStateCoordinator interface, providing a
// //! moveselection::MinimaxMoveEvaluator with "reasonably unique" hash values for each
// //! encountered board state, and the ability to read/write board_state-MinimaxResult
// //! pairs in a boardstate::TranspositionTable.
// template <typename KeyType, size_t NumConfKeys>
// class ZobristCoordinatorNew : public BoardStateCoordinator<
//                                   ZobristCoordinatorNew<KeyType, NumConfKeys>,
//                                   KeyType> {
//   ZobristComponentNew<KeyType, NumConfKeys> &zobrist_component_;
//   TranspositionTable<KeyType, NumConfKeys> &tr_table_;
//   TranspositionTableGuard &tr_table_guard_;
//   TranspositionTablePruner<KeyType, NumConfKeys> &tr_table_pruner_;

// public:
//   ZobristCoordinatorNew(const ZobristCoordinatorNew &) = delete;
//   ZobristCoordinatorNew &operator=(const ZobristCoordinatorNew &) = delete;

//   explicit ZobristCoordinatorNew(
//       ZobristComponentNew<KeyType, NumConfKeys> &zobrist_component,
//       TranspositionTable<KeyType, NumConfKeys> &tr_table,
//       TranspositionTableGuard &tr_table_guard,
//       TranspositionTablePruner<KeyType, NumConfKeys> &tr_table_pruner
//   )
//       : zobrist_component_{zobrist_component}
//       , tr_table_{tr_table}
//       , tr_table_guard_{tr_table_guard}
//       , tr_table_pruner_{tr_table_pruner} {
//     // tr_table_pruner_.Start();
//   }

//   KeyType ImplementGetState() { return zobrist_component_.primary_board_state(); }

//   void ImplementUpdateBoardState(const ExecutedMove &executed_move) {
//     zobrist_component_.UpdateBoardStates(executed_move);
//   }

//   void ImplementFullBoardStateCalc(const BoardMap_t &board_map) {
//     zobrist_component_.FullBoardStateCalc(board_map);
//   }

//   void ImplementRecordTrData(
//       DepthType search_depth,
//       moveselection::MinimaxResultType result_type,
//       moveselection::EqualScoreMoves &similar_moves,
//       MoveCountType access_index
//   ) {
//     tr_table_.RecordData(
//         zobrist_component_.primary_board_state(),
//         search_depth,
//         result_type,
//         similar_moves,
//         zobrist_component_.confirmation_board_states()
//     );
//   }

//   moveselection::TranspositionTableSearchResult ImplementGetTrData(
//       DepthType search_depth,
//       MoveCountType access_index
//   ) {
//     return tr_table_.GetDataAt(
//         zobrist_component_.primary_board_state(),
//         search_depth,
//         zobrist_component_.confirmation_board_states()
//     );
//   }

//   size_t ImplementGetTrTableSize() { return tr_table_.size(); }

//   void ImplementUpdateMoveCounter() {
//     // tr_table_pruner_.IncrementMoveCounter();
//     tr_table_.IncrementMoveCounter();
//   }

//   const std::string board_state_hex_str() {
//     return IntToHexString(zobrist_component_.primary_board_state());
//   }

//   uint32_t zkeys_seed() { return zobrist_component_.prng_seed(); }
// };

} // namespace boardstate