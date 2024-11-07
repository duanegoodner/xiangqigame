

#pragma once

#include <array>
#include <board_data_structs.hpp>
#include <key_generator.hpp>
#include <move_data_structs.hpp>
#include <vector>

namespace boardstate {

//! Container for all of the hash keys needed to run a boardstate::SingleZobristTracker.
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

// template <typename KeyType>
template <typename KeyType, size_t NumConfCalculators>
class ZobristComponent {
public:
  explicit ZobristComponent(
      const ZobristCalculator<KeyType> primary_calculator,
      const std::array<ZobristCalculator<KeyType>, NumConfCalculators>
          &confirmation_calculators
  )
      : primary_calculator_{primary_calculator}
      , confirmation_calculators_(confirmation_calculators) {}

  ZobristComponent()
      : primary_calculator_{ZobristCalculator<KeyType>()}
      , confirmation_calculators_{} {

    for (auto index = 0; index < NumConfCalculators; ++index) {
      confirmation_calculators_[index] = ZobristCalculator<KeyType>();
    }
  }

  ZobristComponent(
      uint32_t primary_seed,
      std::array<uint32_t, NumConfCalculators> confirmation_seeds
  )
      : primary_calculator_{ZobristCalculator<KeyType>{primary_seed}}
      , confirmation_calculators_{} {
    for (auto index = 0; index < NumConfCalculators; ++index) {
      confirmation_calculators_[index] =
          ZobristCalculator<KeyType>{confirmation_seeds[index]};
    }
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

  std::array<KeyType, NumConfCalculators> confirmation_board_states() {
    std::array<KeyType, NumConfCalculators> confirmation_states;
    for (auto i = 0; i < NumConfCalculators; ++i) {
      confirmation_states[i] = confirmation_calculators_[i].board_state();
    }
    return confirmation_states;
  }

  KeyType primary_calculator_seed() { return primary_calculator_.seed(); }

  std::array<uint32_t, NumConfCalculators> confirmation_calculator_seeds() const {
    std::array<uint32_t, NumConfCalculators> seeds;
    for (auto i = 0; i < NumConfCalculators; ++i) {
      seeds[i] = confirmation_calculators_[i].seed();
    }
  }

  std::string primary_board_state_hex_str() const {
    return boardstate::IntToHexString(primary_calculator_.board_state());
  }

private:
  ZobristCalculator<KeyType> primary_calculator_;
  std::array<ZobristCalculator<KeyType>, NumConfCalculators> confirmation_calculators_;
};

template <typename KeyType, size_t NumConfirmationKeys>
class TranspositionTableEntry {
public:
  TranspositionTableEntry(
      moveselection::MinimaxCalcResult calc_result,
      std::array<KeyType, NumConfirmationKeys> confirmation_keys
  )
      : calc_result_{calc_result}
      , confirmation_keys_{confirmation_keys} {}

  moveselection::MinimaxCalcResult calc_result() { return calc_result_; }

  std::array<KeyType, NumConfirmationKeys> confirmation_keys() {
    return confirmation_keys_;
  }

  bool ConfirmationKeysMatchExpected(
      std::array<KeyType, NumConfirmationKeys> expected_keys
  ) {
    for (auto i = 0; i < NumConfirmationKeys; ++i) {
      if (expected_keys[i] != confirmation_keys_[i]) {
        return false;
      }
    }
    return true;
  }

private:
  moveselection::MinimaxCalcResult calc_result_;
  std::array<KeyType, NumConfirmationKeys> confirmation_keys_;
};

template <typename KeyType, size_t NumConfirmationKeys>
class TranspositionTable {
public:
  moveselection::TranspositionTableSearchResult GetDataAt(
      KeyType primary_board_state,
      int remaining_search_depth,
      std::array<KeyType, NumConfirmationKeys> expected_keys
  ) {
    moveselection::TranspositionTableSearchResult result{};
    auto tr_table_entry_it = data_.find(primary_board_state);
    if (tr_table_entry_it != data_.end()) {
      auto tr_table_entry = tr_table_entry_it->second;
      if (tr_table_entry.calc_result().remaining_search_depth >=
          remaining_search_depth) {
        result.found = true;
        result.table_entry = tr_table_entry.calc_result();
      }
      if (result.found and
          !tr_table_entry.ConfirmationKeysMatchExpectedKeys(expected_keys)) {
        result.known_collision = true;
      }
    }
    return result;
  }

  void RecordData(
      KeyType primary_board_state,
      int search_depth,
      moveselection::MinimaxResultType result_type,
      moveselection::EqualScoreMoves &similar_moves,
      std::array<KeyType, NumConfirmationKeys> &confirmation_keys
  ) {
    data_.insert_or_assign(
        primary_board_state,
        TranspositionTableEntry<KeyType, NumConfirmationKeys>{
            moveselection::MinimaxCalcResult{search_depth, result_type, similar_moves},
            confirmation_keys
        }
    );
  }

  size_t num_entries() {return data_.size();}
  size_t num_states() {return data_.size(); }

private:
  std::unordered_map<KeyType, TranspositionTableEntry<KeyType, NumConfirmationKeys>>
      data_;
};

} // namespace boardstate