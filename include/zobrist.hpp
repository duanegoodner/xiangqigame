

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
template <typename KeyType, size_t NumCalculators>
class ZobristComponent {
public:
  explicit ZobristComponent(
      const std::array<ZobristCalculator<KeyType>, NumCalculators> &calculators
  )
      : calculators_(calculators) {}

  ZobristComponent()
      : calculators_{} {
    for (auto index = 0; index < NumCalculators; ++index) {
      //   calculators_.emplace_back();
      calculators_[index] = ZobristCalculator<KeyType>();
    }
  }

  //   ZobristComponent(std::vector<uint32_t> seeds)
  ZobristComponent(std::array<uint32_t, NumCalculators> seeds)
      : calculators_{} {
    for (auto index = 0; index < seeds.size(); ++index) {
      calculators_[index] = ZobristCalculator<KeyType>{seeds[index]};
    }
  }

  void UpdateBoardStates(const ExecutedMove &executed_move) {
    for (auto &calculator : calculators_) {
      calculator.UpdateBoardState(executed_move);
    }
  }

  void FullBoardStateCalc(const BoardMap_t &board_map) {
    for (auto &calculator : calculators_) {
      calculator.FullBoardStateCalc(board_map);
    }
  }

  std::array<KeyType, NumCalculators> GetAllBoardStates() {
    std::array<KeyType, NumCalculators> states;
    for (auto i = 0; i < calculators_.size(); ++i) {
      states[i] = calculators_[i].board_state();
    }
    return states;
  }

  KeyType GetPrimaryBoardState() { return calculators_[0].board_state(); }

  std::array<uint32_t, NumCalculators> seeds() const {
    std::array<uint32_t, NumCalculators> seeds;
    for (auto i = 0; i < NumCalculators; ++i) {
      seeds[i] = calculators_[i].seed();
    }
  }

  std::string primary_board_state_hex_str() const {
    return boardstate::IntToHexString(calculators_[0].board_state());
  }

  // Other methods as needed...
private:
  std::array<ZobristCalculator<KeyType>, NumCalculators> calculators_;
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

} // namespace boardstate