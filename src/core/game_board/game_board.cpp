// Filename: game_board.cpp
// Author: Duane Goodner
// Created: 2022-12-18
// Last Modified: 2024-09-30

// Description:
// Implementations of all methods for GameBoard

#include <board_components.hpp>
#include <board_utilities.hpp>
#include <game_board.hpp>
#include <iostream>
#include <typeinfo>
#include <utility_functs.hpp>

using namespace board_utilities;
using namespace std;


namespace gameboard {
const BoardMapInt_t kStartingBoard = {{
    {5, 4, 3, 2, 1, 2, 3, 4, 5},
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 6, 0, 0, 0, 0, 0, 6, 0},
    {7, 0, 7, 0, 7, 0, 7, 0, 7},
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {-7, 0, -7, 0, -7, 0, -7, 0, -7},
    {0, -6, 0, 0, 0, 0, 0, -6, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {-5, -4, -3, -2, -1, -2, -3, -4, -5},
}};

const int kRepeatPeriodsToCheck[3] = {2, 3, 4};
const int kMaxAllowedRepeatPeriods = 2;



GameBoard::GameBoard(const BoardMapInt_t starting_board)
    : board_map_{int_board_to_game_pieces(starting_board)}
    , move_calculator_{MoveCalculator()} {}

GameBoard::GameBoard()
    : GameBoard(kStartingBoard) {}

vector<BoardSpace> GameBoard::ImplementGetAllSpacesOccupiedBy(PieceColor color) {
  return get_all_spaces_occupied_by(board_map_, color);
}

PieceColor GameBoard::ImplementGetColor(BoardSpace space) {
  return get_color(board_map_, space);
}

PieceType GameBoard::ImplementGetType(BoardSpace space) {
  return get_type(board_map_, space);
}

MoveCollection GameBoard::ImplementCalcFinalMovesOf(PieceColor color) {
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

bool GameBoard::IsInCheck(PieceColor color) {
  auto gen_position = get_general_position(board_map_, color);
  auto opponent_moves =
      move_calculator_.CalcAllMovesNoCheckTest(opponent_of(color), board_map_);
  return opponent_moves.ContainsDestination(gen_position);
}

ExecutedMove GameBoard::ImplementExecuteMove(Move move) {
  auto moving_piece = GetOccupant(move.start);
  auto destination_piece = GetOccupant(move.end);
  SetOccupant(move.end, moving_piece);
  SetOccupant(move.start, GamePiece(PieceType::kNnn, PieceColor::kNul));

  auto executed_move = ExecutedMove{move, moving_piece, destination_piece};
  UpdateHashCalculator(executed_move);
  AddToMoveLog(executed_move);

  return ExecutedMove{move, moving_piece, destination_piece};
};

void GameBoard::ImplementUndoMove(ExecutedMove executed_move) {
  SetOccupant(executed_move.spaces.start, executed_move.moving_piece);
  SetOccupant(executed_move.spaces.end, executed_move.destination_piece);
  UpdateHashCalculator(executed_move);
  RemoveFromMoveLog(executed_move);
};

GamePiece GameBoard::GetOccupant(BoardSpace space) {
  return board_map_[space.rank][space.file];
};

const BoardMap_t &GameBoard::map() const { return board_map_; }

void GameBoard::ImplementAttachMoveCallback(function<void(ExecutedMove)> callback) {
  move_callbacks_.emplace_back(callback);
}

std::map<PieceColor, vector<ExecutedMove>> GameBoard::GetMoveLog() {
  return move_log_;
}

void GameBoard::UpdateHashCalculator(ExecutedMove executed_move) {
  for (const auto &callback : move_callbacks_) {
    callback(executed_move);
  }
};

void GameBoard::SetOccupant(BoardSpace space, GamePiece piece) {
  board_map_[space.rank][space.file] = piece;
}

void GameBoard::AddToMoveLog(ExecutedMove executed_move) {
  auto piece_color = executed_move.moving_piece.piece_color;
  move_log_[piece_color].push_back(executed_move);
};

void GameBoard::RemoveFromMoveLog(ExecutedMove executed_move) {
  auto piece_color = executed_move.moving_piece.piece_color;
  auto last_move_by_color = move_log_[piece_color].back();
  if (!(executed_move == last_move_by_color)) {
    throw runtime_error("Last move in log does not match move to be removed");
  }
  move_log_[piece_color].pop_back();
}

bool GameBoard::ViolatesRepeatRule(PieceColor color) {
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

}
