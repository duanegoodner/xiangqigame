// Filename: game_board.hpp
// Author: Duane Goodner
// Created: 2022-11-14
// Last Modified: 2024-08-16

// Description:
// Definition of GameBoard class, and the BoardStateSummarizer CRTP interface
// that a GameBoard requires a HashCalculator to comply with.

#ifndef _GAME_BOARD_
#define _GAME_BOARD_

#include <board_components.hpp>
#include <common.hpp>
#include <functional>
#include <game_board_details.hpp>
#include <move_calculator.hpp>
#include <move_evaluators.hpp>
#include <vector>

using namespace std;
using namespace board_components;

// Template for class NewGameBoard which implements interface
// SpaceInfoProvider, and uses a ConcreteBoardStateSummarizer

class NewGameBoard : public SpaceInfoProvider<NewGameBoard> {
public:
  NewGameBoard();
  NewGameBoard(const BoardMapInt_t starting_board);
  vector<BoardSpace> ImplementGetAllSpacesOccupiedBy(PieceColor color) {
    return get_all_spaces_occupied_by(board_map_, color);
  }
  PieceColor ImplementGetColor(BoardSpace space) { return get_color(board_map_, space); }
  PieceType ImplementGetType(BoardSpace space) { return get_type(board_map_, space); }

  MoveCollection ImplementCalcFinalMovesOf(PieceColor color) {
    return _ImplementCalcFinalMovesOf(color);
  }
  bool IsInCheck(PieceColor color) {
    return _IsInCheck(color);
  }
  ExecutedMove ImplementExecuteMove(Move move) {
    return _ImplementExecuteMove(move);
  };
  void ImplementUndoMove(ExecutedMove executed_move) {
    return _ImplementUndoMove(executed_move);
  };
  GamePiece GetOccupant(BoardSpace space) { return board_map_[space.rank][space.file]; };
  const BoardMap_t &map() const { return board_map_; }
  void ImplementAttachMoveCallback(function<void(ExecutedMove)> callback) {
    move_callbacks_.emplace_back(callback);
  }
  std::map<PieceColor, vector<ExecutedMove>> GetMoveLog() {
    return move_log_;
  }

private:
  BoardMap_t board_map_;
  MoveCalculator move_calculator_;
  vector<function<void(ExecutedMove)>> move_callbacks_;  // hash calc functs go here
  std::map<PieceColor, vector<ExecutedMove>> move_log_;

  ExecutedMove _ImplementExecuteMove(Move move) {
    auto moving_piece = GetOccupant(move.start);
    auto destination_piece = GetOccupant(move.end);
    SetOccupant(move.end, moving_piece);
    SetOccupant(move.start, GamePiece(PieceType::kNnn, PieceColor::kNul));

    auto executed_move = ExecutedMove{move, moving_piece, destination_piece};
    UpdateHashCalculator(executed_move);
    AddToMoveLog(executed_move);

    return ExecutedMove{move, moving_piece, destination_piece};
  };
  
  void _ImplementUndoMove(ExecutedMove executed_move) {
    SetOccupant(executed_move.spaces.start, executed_move.moving_piece);
    SetOccupant(executed_move.spaces.end, executed_move.destination_piece);
    UpdateHashCalculator(executed_move);
    RemoveFromMoveLog(executed_move);
  }

  
  void UpdateHashCalculator(ExecutedMove executed_move) {
    for (const auto &callback : move_callbacks_) {
      callback(executed_move);
    }
  };
  void SetOccupant(BoardSpace space, GamePiece piece) {
    board_map_[space.rank][space.file] = piece;
  }
  void AddToMoveLog(ExecutedMove executed_move) {
    auto piece_color = executed_move.moving_piece.piece_color;
    move_log_[piece_color].push_back(executed_move);
  };
  void RemoveFromMoveLog(ExecutedMove executed_move) {
    auto piece_color = executed_move.moving_piece.piece_color;
    auto last_move_by_color = move_log_[piece_color].back();
    if (!(executed_move == last_move_by_color)) {
      throw runtime_error("Last move in log does not match move to be removed");
    }
    move_log_[piece_color].pop_back();
  }
  bool ViolatesRepeatRule(PieceColor color) {
    for (auto period_length : kRepeatPeriodsToCheck) {
      auto lookback_length = (kMaxAllowedRepeatPeriods + 1) * period_length;
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
  bool _IsInCheck(PieceColor color) {
    auto gen_position = get_general_position(board_map_, color);
    auto opponent_moves =
        move_calculator_.CalcAllMovesNoCheckTest(opponent_of(color), board_map_);
    return opponent_moves.ContainsDestination(gen_position);
  }
  MoveCollection _ImplementCalcFinalMovesOf(PieceColor color) {
    auto un_tested_moves = move_calculator_.CalcAllMovesNoCheckTest(color, board_map_);
    MoveCollection validated_moves;
    validated_moves.moves.reserve(un_tested_moves.moves.size());

    for (auto move : un_tested_moves.moves) {
      auto executed_move = ImplementExecuteMove(move);
      auto resulting_opponent_moves =
          move_calculator_.CalcAllMovesNoCheckTest(opponent_of(color), board_map_);
      auto resulting_gen_position = get_general_position(board_map_, color);

      if (not resulting_opponent_moves.ContainsDestination(resulting_gen_position) and
          not ViolatesRepeatRule(color)) {
        validated_moves.Append(move);
      }

      ImplementUndoMove(executed_move);
    }
    return validated_moves;
  }
};

#include <game_board.tpp>

#endif // _GAME_BOARD_