// Filename: game_board.tpp
// Author: Duane Goodner
// Created: 2022-12-10
// Last Modified: 2024-08-16

// Description:
// Contains method implementations for a GameBoard class with a
// ConcreteBoardStateSummarizer

#ifndef _GAME_BOARD_TEMPLATE_
#define _GAME_BOARD_TEMPLATE_

#include <board_components.hpp>
#include <board_utilities.hpp>
#include <game_board_details.hpp>
#include <iostream>
#include <typeinfo>
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

template <
    typename ConcreteBoardStateSummarizer,
    typename ConcreteBoardStateSummarizerRed,
    typename ConcreteBoardStateSummarizerBlack>
NewGameBoard<
    ConcreteBoardStateSummarizer,
    ConcreteBoardStateSummarizerRed,
    ConcreteBoardStateSummarizerBlack>::NewGameBoard(const BoardMapInt_t
                                                         board_array)
    : board_map_{int_board_to_game_pieces(board_array)}
    , move_calculator_{MoveCalculator()} {

  hash_calculator_red_.CalcInitialBoardState(board_map_);
  hash_calculator_black_.CalcInitialBoardState(board_map_);

  state_details_dispatch_table_[PieceColor::kBlk] =
      &NewGameBoard::GetCurrentStateDetailsBlack;
  state_details_dispatch_table_[PieceColor::kRed] =
      &NewGameBoard::GetCurrentStateDetailsRed;

  write_state_details_dispatch_table_[PieceColor::kBlk] =
      &NewGameBoard::RecordCurrentStateDetailsBlack;
  write_state_details_dispatch_table_[PieceColor::kRed] =
      &NewGameBoard::RecordCurrentStateDetailsRed;
}

template <
    typename ConcreteBoardStateSummarizer,
    typename ConcreteBoardStateSummarizerRed,
    typename ConcreteBoardStateSummarizerBlack>
NewGameBoard<
    ConcreteBoardStateSummarizer,
    ConcreteBoardStateSummarizerRed,
    ConcreteBoardStateSummarizerBlack>::NewGameBoard()
    : NewGameBoard(kStartingBoard) {}

template <
    typename ConcreteBoardStateSummarizer,
    typename ConcreteBoardStateSummarizerRed,
    typename ConcreteBoardStateSummarizerBlack>
GamePiece NewGameBoard<
    ConcreteBoardStateSummarizer,
    ConcreteBoardStateSummarizerRed,
    ConcreteBoardStateSummarizerBlack>::GetOccupant(BoardSpace space) {
  return board_map_[space.rank][space.file];
}

//  Implement in header for now
// template <
//     typename ConcreteBoardStateSummarizer,
//     typename ConcreteBoardStateSummarizerRed,
//     typename ConcreteBoardStateSummarizerBlack>
// void NewGameBoard<
//     ConcreteBoardStateSummarizer,
//     ConcreteBoardStateSummarizerRed,
//     ConcreteBoardStateSummarizerBlack>::UpdateHashCalculator(ExecutedMove
//                                                                  executed_move
// ) {
//   hash_calculator_.CalcNewBoardState(executed_move);
// }

template <
    typename ConcreteBoardStateSummarizer,
    typename ConcreteBoardStateSummarizerRed,
    typename ConcreteBoardStateSummarizerBlack>
void NewGameBoard<
    ConcreteBoardStateSummarizer,
    ConcreteBoardStateSummarizerRed,
    ConcreteBoardStateSummarizerBlack>::AddToMoveLog(ExecutedMove executed_move
) {
  auto piece_color = executed_move.moving_piece.piece_color;
  move_log_[piece_color].push_back(executed_move);
}

template <
    typename ConcreteBoardStateSummarizer,
    typename ConcreteBoardStateSummarizerRed,
    typename ConcreteBoardStateSummarizerBlack>
void NewGameBoard<
    ConcreteBoardStateSummarizer,
    ConcreteBoardStateSummarizerRed,
    ConcreteBoardStateSummarizerBlack>::RemoveFromMoveLog(ExecutedMove
                                                              executed_move) {
  auto piece_color = executed_move.moving_piece.piece_color;
  auto last_move_by_color = move_log_[piece_color].back();
  if (!(executed_move == last_move_by_color)) {
    throw runtime_error("Last move in log does not match move to be removed");
  }
  move_log_[piece_color].pop_back();
}

template <
    typename ConcreteBoardStateSummarizer,
    typename ConcreteBoardStateSummarizerRed,
    typename ConcreteBoardStateSummarizerBlack>
ExecutedMove NewGameBoard<
    ConcreteBoardStateSummarizer,
    ConcreteBoardStateSummarizerRed,
    ConcreteBoardStateSummarizerBlack>::ImplementExecuteMove(Move move) {
  auto moving_piece = GetOccupant(move.start);
  auto destination_piece = GetOccupant(move.end);
  SetOccupant(move.end, moving_piece);
  SetOccupant(move.start, GamePiece(PieceType::kNnn, PieceColor::kNul));

  auto executed_move = ExecutedMove{move, moving_piece, destination_piece};
  UpdateHashCalculator(executed_move);
  AddToMoveLog(executed_move);

  return ExecutedMove{move, moving_piece, destination_piece};
}

template <
    typename ConcreteBoardStateSummarizer,
    typename ConcreteBoardStateSummarizerRed,
    typename ConcreteBoardStateSummarizerBlack>
void NewGameBoard<
    ConcreteBoardStateSummarizer,
    ConcreteBoardStateSummarizerRed,
    ConcreteBoardStateSummarizerBlack>::ImplementUndoMove(ExecutedMove
                                                              executed_move) {
  SetOccupant(executed_move.spaces.start, executed_move.moving_piece);
  SetOccupant(executed_move.spaces.end, executed_move.destination_piece);
  UpdateHashCalculator(executed_move);
  RemoveFromMoveLog(executed_move);
}

template <
    typename ConcreteBoardStateSummarizer,
    typename ConcreteBoardStateSummarizerRed,
    typename ConcreteBoardStateSummarizerBlack>
vector<BoardSpace> NewGameBoard<
    ConcreteBoardStateSummarizer,
    ConcreteBoardStateSummarizerRed,
    ConcreteBoardStateSummarizerBlack>::
    ImplementGetAllSpacesOccupiedBy(PieceColor color) {
  return get_all_spaces_occupied_by(board_map_, color);
}

template <
    typename ConcreteBoardStateSummarizer,
    typename ConcreteBoardStateSummarizerRed,
    typename ConcreteBoardStateSummarizerBlack>
void NewGameBoard<
    ConcreteBoardStateSummarizer,
    ConcreteBoardStateSummarizerRed,
    ConcreteBoardStateSummarizerBlack>::
    SetOccupant(BoardSpace space, GamePiece piece) {
  board_map_[space.rank][space.file] = piece;
}

template <
    typename ConcreteBoardStateSummarizer,
    typename ConcreteBoardStateSummarizerRed,
    typename ConcreteBoardStateSummarizerBlack>
bool NewGameBoard<
    ConcreteBoardStateSummarizer,
    ConcreteBoardStateSummarizerRed,
    ConcreteBoardStateSummarizerBlack>::IsInCheck(PieceColor color) {
  auto gen_position = get_general_position(board_map_, color);
  auto opponent_moves =
      move_calculator_.CalcAllMovesNoCheckTest(opponent_of(color), board_map_);
  return opponent_moves.ContainsDestination(gen_position);
}

template <
    typename ConcreteBoardStateSummarizer,
    typename ConcreteBoardStateSummarizerRed,
    typename ConcreteBoardStateSummarizerBlack>
MoveCollection NewGameBoard<
    ConcreteBoardStateSummarizer,
    ConcreteBoardStateSummarizerRed,
    ConcreteBoardStateSummarizerBlack>::ImplementCalcFinalMovesOf(PieceColor
                                                                      color) {
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

template <
    typename ConcreteBoardStateSummarizer,
    typename ConcreteBoardStateSummarizerRed,
    typename ConcreteBoardStateSummarizerBlack>
PieceColor NewGameBoard<
    ConcreteBoardStateSummarizer,
    ConcreteBoardStateSummarizerRed,
    ConcreteBoardStateSummarizerBlack>::ImplementGetColor(BoardSpace space) {
  return get_color(board_map_, space);
}

template <
    typename ConcreteBoardStateSummarizer,
    typename ConcreteBoardStateSummarizerRed,
    typename ConcreteBoardStateSummarizerBlack>
PieceType NewGameBoard<
    ConcreteBoardStateSummarizer,
    ConcreteBoardStateSummarizerRed,
    ConcreteBoardStateSummarizerBlack>::ImplementGetType(BoardSpace space) {
  return get_type(board_map_, space);
}

template <
    typename ConcreteBoardStateSummarizer,
    typename ConcreteBoardStateSummarizerRed,
    typename ConcreteBoardStateSummarizerBlack>
TranspositionTableSearchResult NewGameBoard<
    ConcreteBoardStateSummarizer,
    ConcreteBoardStateSummarizerRed,
    ConcreteBoardStateSummarizerBlack>::
    ImplementSearchTranspositionTable(PieceColor color, int search_depth) {
  auto search_function = state_details_dispatch_table_.at(color);
  return (this->*search_function(search_depth));
}

template <
    typename ConcreteBoardStateSummarizer,
    typename ConcreteBoardStateSummarizerRed,
    typename ConcreteBoardStateSummarizerBlack>
void NewGameBoard<
    ConcreteBoardStateSummarizer,
    ConcreteBoardStateSummarizerRed,
    ConcreteBoardStateSummarizerBlack>::
    ImplementRecordCurrentStateScore(
        PieceColor color,
        int search_depth,
        MinimaxResultType result_type,
        BestMoves &best_moves
    ) {
  auto record_function = write_state_details_dispatch_table_.at(color);
  (this->*record_function(search_depth, result_type, best_moves));
}

template <
    typename ConcreteBoardStateSummarizer,
    typename ConcreteBoardStateSummarizerRed,
    typename ConcreteBoardStateSummarizerBlack>
bool NewGameBoard<
    ConcreteBoardStateSummarizer,
    ConcreteBoardStateSummarizerRed,
    ConcreteBoardStateSummarizerBlack>::ViolatesRepeatRule(PieceColor color) {
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