#include <array>
#include <gameboard/board_data_structs.hpp>
#include <gameboard/game_piece.hpp>
#include <boardstate/key_generator.hpp>
#include <memory>
#include <gameboard/move_data_structs.hpp>
#include <random>

template <typename KeyType>
class MyCalculator {
  using PieceArrayType = std::array<std::array<KeyType, 9>, 10>;
  using TeamArrayType = std::array<PieceArrayType, 7>;
  using GameArrayType = std::array<TeamArrayType, 2>;

  GameArrayType zarray_;
  KeyType turn_key_;
  uint32_t seed_;
  KeyType board_state_;

public:
  MyCalculator(uint32_t seed = std::random_device{}())
      : zarray_{}
      , turn_key_{}
      , board_state_{}
      , seed_{seed} {
    boardstate::PseudoRandomKeyGenerator<KeyType> key_generator{seed};
    turn_key_ = key_generator.GenerateKey();
    zarray_ = CreateGameZarray(key_generator);
  }

  KeyType board_state() const { return board_state_; }
  uint32_t seed() const { return seed; }
  void FullBoardStateCalc(const gameboard::BoardMap_t &board_map) {
    FullBoardStateCalcInternal(board_map);
  }
  void UpdateBoardState(const gameboard::ExecutedMove &executed_move) {
    UpdateBoardStateInternal(executed_move);
  }

private:
  //! Static helper method for building 4-D array of Zobrist keys in constuctor.
  static const GameArrayType CreateGameZarray(
      boardstate::PseudoRandomKeyGenerator<KeyType> &key_generator
  ) {
    GameArrayType game_zarray{};
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

  KeyType GetHashValueAt(PieceColor color, PieceType piece_type, BoardSpace space) {
    return zarray_[GetZColorIndexOf(color)][piece_type][space.rank][space.file];
  }

  void FullBoardStateCalcInternal(const gameboard::BoardMap_t &board_map) {
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
    board_state_ = board_state_ ^ turn_key_;
  }
};

template <typename KeyType>
class MyCalculatorBuilder {
  std::shared_ptr<MyCalculator<KeyType>> build(uint32_t seed = std::random_device{}()) {
    return std::make_shared<MyCalculator<KeyType>>(seed);
  }
};
