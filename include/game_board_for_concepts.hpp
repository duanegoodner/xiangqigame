#pragma once

#include <base_space_info_provider.hpp>
#include <board_data_structs.hpp>
#include <concepts/board_state_calculator.hpp>
#include <functional>
#include <map>
#include <memory>
#include <move_calculator.hpp>
#include <move_data_structs.hpp>

namespace gameboard {

extern const BoardMapInt_t kStandardInitialBoard;
extern const int kRepeatPeriodsToCheck[3];
extern const int kRepeatPeriodsMaxAllowed;
extern const int kMaxMovesWithoutCapture;

//! Must comply with SpaceInfoProviderConcept; stores piece positions, and exposes
//! methods for calculating, executing, an un-doing moves.

class GameBoardForConcepts : public SpaceInfoProviderBase {
  BoardMap_t board_map_;
  MoveCalculator move_calculator_;
  vector<function<void(const ExecutedMove &)>> move_callbacks_;
  std::map<PieceColor, std::vector<ExecutedMove>> move_log_;
  MoveCountType moves_since_last_capture_;

public:
  static std::shared_ptr<GameBoardForConcepts> Create(
      const BoardMapInt_t &starting_board = kStandardInitialBoard
  ) {
    return std::shared_ptr<GameBoardForConcepts>(new GameBoardForConcepts(starting_board)
    );
  }

  inline const BoardMap_t &map() const { return board_map_; }

  inline const std::map<PieceColor, std::vector<ExecutedMove>> &move_log() const {
    return move_log_;
  }

  void AttachMoveCallback(const function<void(const ExecutedMove &)> &callback) {
    move_callbacks_.emplace_back(callback);
  }

  inline PieceColor GetColor(const BoardSpace &space) const {
    return board_map_[space.rank][space.file].piece_color;
  }

  bool IsInCheck(PieceColor color) { return IsInCheckInternal(color); }

  ExecutedMove ExecuteMove(const Move &move) { return ExecuteMoveInternal(move); }

  inline bool IsDraw() { return moves_since_last_capture_ >= kMaxMovesWithoutCapture; }

  inline std::vector<BoardSpace> GetAllSpacesOccupiedBy(const PieceColor color) const {
    return GetAllSpacesOccupiedByInternal(color);
  }

  inline PieceType GetType(const BoardSpace &space) const {
    return board_map_[space.rank][space.file].piece_type;
  }

  MoveCollection CalcFinalMovesOf(PieceColor color) {
    return CalcFinalMovesOfInternal(color);
  }

  inline bool IsCaptureMove(const ExecutedMove &executed_move) const {
    return executed_move.destination_piece != PieceColor::kNul;
  }

  void UndoMove(const ExecutedMove &executed_move) {
    return UndoMoveInternal(executed_move);
  }

  inline GamePiece GetOccupantAt(const BoardSpace &space) const {
    return board_map_[space.rank][space.file];
  }

private:
  //! Initializes a gameboard::GameBoard from array of pieces represented as integers.
  //! @param starting_board An array of integers representing pieces on the board.
  GameBoardForConcepts(const BoardMapInt_t starting_board)
      : board_map_{int_board_to_game_pieces(starting_board)}
      , move_calculator_{MoveCalculator()}
      , moves_since_last_capture_{}
      , move_callbacks_{} {}

  MoveCollection CalcFinalMovesOfInternal(PieceColor color) {
    
    // initialize MoveCollection where we will store allowed moves
    MoveCollection validated_moves{};

    // if board meets "draw" condition, no moves allowed, so return empty collection 
    if (IsDraw()) {
      return validated_moves;
    }

    auto un_tested_moves = move_calculator_.CalcAllMovesNoCheckTest(color, board_map_);

    validated_moves.moves.reserve(un_tested_moves.moves.size());

    for (auto move : un_tested_moves.moves) {
      auto executed_move = ExecuteMove(move);
      auto resulting_opponent_moves =
          move_calculator_.CalcAllMovesNoCheckTest(opponent_of(color), board_map_);
      auto resulting_gen_position = get_general_position(board_map_, color);

      if (not resulting_opponent_moves.ContainsDestination(resulting_gen_position) and
          not ViolatesRepeatRule(color)) {
        validated_moves.Append(move);
      }

      UndoMove(executed_move);
    }
    return validated_moves;
  }

  bool IsInCheckInternal(PieceColor color) {
    auto gen_position = get_general_position(board_map_, color);
    auto opponent_moves =
        move_calculator_.CalcAllMovesNoCheckTest(opponent_of(color), board_map_);
    return opponent_moves.ContainsDestination(gen_position);
  }

  ExecutedMove ExecuteMoveInternal(const Move &move) {
    auto moving_piece = GetOccupantAt(move.start);
    auto destination_piece = GetOccupantAt(move.end);
    SetOccupantAt(move.end, moving_piece);
    SetOccupantAt(move.start, GamePiece(PieceType::kNnn, PieceColor::kNul));

    auto executed_move =
        ExecutedMove{move, moving_piece, destination_piece, moves_since_last_capture_};
    UpdateStateTracker(executed_move);
    AddToMoveLog(executed_move);
    if (!IsCaptureMove(executed_move)) {
      moves_since_last_capture_++;
    } else {
      moves_since_last_capture_ = 0;
    }

    return executed_move;
  }

  inline std::vector<BoardSpace> GetAllSpacesOccupiedByInternal(const PieceColor color
  ) const {
    vector<BoardSpace> occupied_spaces;
    occupied_spaces.reserve(16);
    for (auto rank = 0; rank < kNumRanks; rank++) {
      for (auto file = 0; file < kNumFiles; file++) {
        if (GetColor(BoardSpace{rank, file}) == color) {
          occupied_spaces.emplace_back(BoardSpace{rank, file});
        }
      }
    }
    return occupied_spaces;
  }

  void UndoMoveInternal(const ExecutedMove &executed_move) {
    SetOccupantAt(executed_move.spaces.start, executed_move.moving_piece);
    SetOccupantAt(executed_move.spaces.end, executed_move.destination_piece);
    UpdateStateTracker(executed_move);
    moves_since_last_capture_ = executed_move.moves_since_last_capture;
    RemoveFromMoveLog(executed_move);
  };

  void UpdateStateTracker(const ExecutedMove &executed_move) {

    for (const auto &callback : move_callbacks_) {
      callback(executed_move);
    }
  }

  void SetOccupantAt(const BoardSpace &space, GamePiece piece) {
    board_map_[space.rank][space.file] = piece;
  }

  void AddToMoveLog(const ExecutedMove &executed_move) {
    auto piece_color = executed_move.moving_piece.piece_color;
    move_log_[piece_color].push_back(executed_move);
  }

  void RemoveFromMoveLog(const ExecutedMove &executed_move) {
    auto piece_color = executed_move.moving_piece.piece_color;
    auto last_move_by_color = move_log_[piece_color].back();
    if (!(executed_move == last_move_by_color)) {
      throw runtime_error("Last move in log does not match move to be removed");
    }
    move_log_[piece_color].pop_back();
  }

  bool ViolatesRepeatRule(PieceColor color) {
    for (auto period_length : kRepeatPeriodsToCheck) {
      auto lookback_length = (kRepeatPeriodsMaxAllowed + 1) * period_length;
      if (utility_functs::hasRepeatingPattern(
              move_log_[color],
              lookback_length,
              period_length
          )) {
        return true;
      }
    }
    return false;
  }
};

class GameBoardFactory {
public:
  std::shared_ptr<GameBoardForConcepts> Create(
      const BoardMapInt_t &starting_board = kStandardInitialBoard
  ) {
    return GameBoardForConcepts::Create(starting_board);
  }


};



} // namespace gameboard