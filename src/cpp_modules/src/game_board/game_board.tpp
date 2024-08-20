// Filename: game_board.tpp
// Author: Duane Goodner
// Created: 2022-12-10
// Last Modified: 2024-08-16

// Description:
// Contains method implementations for a GameBoard class with a
// ConcreteHashCalculator

#ifndef _GAME_BOARD_TEMPLATE_
#define _GAME_BOARD_TEMPLATE_

#include <board_components.hpp>
#include <board_utilities.hpp>
#include <game_board_details.hpp>
#include <iostream>
#include <utility_functs.hpp>

using namespace board_utilities;
using namespace std;

BoardMap_t int_board_to_game_pieces(const BoardMapInt_t int_board) {
  BoardMap_t game_piece_board;
  for (auto rank = 0; rank < kNumRanks; rank++) {
    for (auto file = 0; file < kNumFiles; file++) {
      game_piece_board[rank][file] = GamePiece(int_board[rank][file]);
    }
  }
  return game_piece_board;
}

template <typename ConcreteHashCalculator>
GameBoard<ConcreteHashCalculator>::GameBoard(const BoardMapInt_t board_array)
    : board_map_{int_board_to_game_pieces(board_array)}
    , move_calculator_{MoveCalculator()}
    , hash_calculator_{} {
  hash_calculator_.CalcInitialBoardState(board_map_);
}

template <typename ConcreteHashCalculator>
GameBoard<ConcreteHashCalculator>::GameBoard()
    : GameBoard(kStartingBoard) {}

template <typename ConcreteHashCalculator>
GamePiece GameBoard<ConcreteHashCalculator>::GetOccupant(BoardSpace space) {
  return board_map_[space.rank][space.file];
}

template <typename ConcreteHashCalculator>
bool GameBoard<ConcreteHashCalculator>::IsOccupied(BoardSpace space) {
  // return move_calculator_.IsOccupied(board_map_, space);
  return is_occupied(board_map_, space);
}

template <typename ConcreteHashCalculator>
void GameBoard<ConcreteHashCalculator>::UpdateHashCalculator(
    ExecutedMove executed_move
) {
  // for (auto calculator_idx = 0; calculator_idx < num_hash_calculators_;
  //      calculator_idx++) {
  //   hash_calculators_[calculator_idx]->CalcNewBoardState(executed_move);
  // }
  hash_calculator_.CalcNewBoardState(executed_move);
}

template <typename ConcreteHashCalculator>
void GameBoard<ConcreteHashCalculator>::AddToMoveLog(ExecutedMove executed_move
) {
  auto piece_color = executed_move.moving_piece.piece_color;
  move_log_[piece_color].push_back(executed_move);
}

template <typename ConcreteHashCalculator>
void GameBoard<ConcreteHashCalculator>::RemoveFromMoveLog(
    ExecutedMove executed_move
) {
  auto piece_color = executed_move.moving_piece.piece_color;
  auto last_move_by_color = move_log_[piece_color].back();
  if (!(executed_move == last_move_by_color)) {
    throw runtime_error("Last move in log does not match move to be removed");
  }
  move_log_[piece_color].pop_back();
}

template <typename ConcreteHashCalculator>
ExecutedMove GameBoard<ConcreteHashCalculator>::ImplementExecuteMove(Move move
) {
  auto moving_piece = GetOccupant(move.start);
  auto destination_piece = GetOccupant(move.end);
  SetOccupant(move.end, moving_piece);
  SetOccupant(move.start, GamePiece(PieceType::kNnn, PieceColor::kNul));

  auto executed_move = ExecutedMove{move, moving_piece, destination_piece};
  UpdateHashCalculator(executed_move);
  AddToMoveLog(executed_move);

  // for (auto calculator : hash_calculators_) {
  //   calculator.CalcNewBoardState(executed_move);
  // }

  return ExecutedMove{move, moving_piece, destination_piece};
}

template <typename ConcreteHashCalculator>
void GameBoard<ConcreteHashCalculator>::ImplementUndoMove(
    ExecutedMove executed_move
) {
  SetOccupant(executed_move.spaces.start, executed_move.moving_piece);
  SetOccupant(executed_move.spaces.end, executed_move.destination_piece);
  UpdateHashCalculator(executed_move);
  RemoveFromMoveLog(executed_move);
}

template <typename ConcreteHashCalculator>
vector<BoardSpace> GameBoard<
    ConcreteHashCalculator>::ImplementGetAllSpacesOccupiedBy(PieceColor color
) {
  return get_all_spaces_occupied_by(board_map_, color);
}

template <typename ConcreteHashCalculator>
void GameBoard<ConcreteHashCalculator>::SetOccupant(
    BoardSpace space,
    GamePiece piece
) {
  board_map_[space.rank][space.file] = piece;
}

template <typename ConcreteHashCalculator>
bool GameBoard<ConcreteHashCalculator>::IsInCheck(PieceColor color) {
  auto gen_position = get_general_position(board_map_, color);
  auto opponent_moves =
      move_calculator_.CalcAllMovesNoCheckTest(opponent_of(color), board_map_);
  return opponent_moves.ContainsDestination(gen_position);
}

template <typename ConcreteHashCalculator>
MoveCollection GameBoard<ConcreteHashCalculator>::ImplementCalcFinalMovesOf(
    PieceColor color
) {
  auto un_tested_moves =
      move_calculator_.CalcAllMovesNoCheckTest(color, board_map_);
  MoveCollection validated_moves;
  validated_moves.moves.reserve(un_tested_moves.moves.size());

  for (auto move : un_tested_moves.moves) {
    auto executed_move = ImplementExecuteMove(move);
    auto resulting_opponent_moves = move_calculator_.CalcAllMovesNoCheckTest(
        opponent_of(color),
        board_map_
    );
    auto resulting_gen_position = get_general_position(board_map_, color);

    if (not resulting_opponent_moves.ContainsDestination(resulting_gen_position
        ) and
        not ViolatesRepeatRule(color)) {
      validated_moves.Append(move);
    }

    ImplementUndoMove(executed_move);
  }
  return validated_moves;
}

template <typename ConcreteHashCalculator>
PieceColor GameBoard<ConcreteHashCalculator>::ImplementGetColor(
    BoardSpace space
) {
  return get_color(board_map_, space);
}

template <typename ConcreteHashCalculator>
PieceType GameBoard<ConcreteHashCalculator>::ImplementGetType(BoardSpace space
) {
  return get_type(board_map_, space);
}

template <typename ConcreteHashCalculator>
bool GameBoard<ConcreteHashCalculator>::ViolatesRepeatRule(PieceColor color) {
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

#endif