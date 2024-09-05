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

template <typename ConcreteBoardStateSummarizer>
NewGameBoard<ConcreteBoardStateSummarizer>::NewGameBoard(
    const BoardMapInt_t board_array
)
    : board_map_{int_board_to_game_pieces(board_array)}
    , move_calculator_{MoveCalculator()}
    , hash_calculator_{}
    , transposition_tables_{} {
  hash_calculator_.CalcInitialBoardState(board_map_);
  transposition_tables_[PieceColor::kBlk] =
      std::unordered_map<zkey_t, vector<TranspositionTableEntry>>();
  transposition_tables_[PieceColor::kRed] =
      std::unordered_map<zkey_t, vector<TranspositionTableEntry>>();
}

template <typename ConcreteBoardStateSummarizer>
NewGameBoard<ConcreteBoardStateSummarizer>::NewGameBoard()
    : NewGameBoard(kStartingBoard) {}

template <typename ConcreteBoardStateSummarizer>
GamePiece NewGameBoard<ConcreteBoardStateSummarizer>::GetOccupant(
    BoardSpace space
) {
  return board_map_[space.rank][space.file];
}

template <typename ConcreteBoardStateSummarizer>
void NewGameBoard<ConcreteBoardStateSummarizer>::UpdateHashCalculator(
    ExecutedMove executed_move
) {
  hash_calculator_.CalcNewBoardState(executed_move);
}

template <typename ConcreteBoardStateSummarizer>
void NewGameBoard<ConcreteBoardStateSummarizer>::AddToMoveLog(
    ExecutedMove executed_move
) {
  auto piece_color = executed_move.moving_piece.piece_color;
  move_log_[piece_color].push_back(executed_move);
}

template <typename ConcreteBoardStateSummarizer>
void NewGameBoard<ConcreteBoardStateSummarizer>::RemoveFromMoveLog(
    ExecutedMove executed_move
) {
  auto piece_color = executed_move.moving_piece.piece_color;
  auto last_move_by_color = move_log_[piece_color].back();
  if (!(executed_move == last_move_by_color)) {
    throw runtime_error("Last move in log does not match move to be removed");
  }
  move_log_[piece_color].pop_back();
}

template <typename ConcreteBoardStateSummarizer>
ExecutedMove NewGameBoard<ConcreteBoardStateSummarizer>::ImplementExecuteMove(
    Move move
) {
  auto moving_piece = GetOccupant(move.start);
  auto destination_piece = GetOccupant(move.end);
  SetOccupant(move.end, moving_piece);
  SetOccupant(move.start, GamePiece(PieceType::kNnn, PieceColor::kNul));

  auto executed_move = ExecutedMove{move, moving_piece, destination_piece};
  UpdateHashCalculator(executed_move);
  AddToMoveLog(executed_move);

  return ExecutedMove{move, moving_piece, destination_piece};
}

template <typename ConcreteBoardStateSummarizer>
void NewGameBoard<ConcreteBoardStateSummarizer>::ImplementUndoMove(
    ExecutedMove executed_move
) {
  SetOccupant(executed_move.spaces.start, executed_move.moving_piece);
  SetOccupant(executed_move.spaces.end, executed_move.destination_piece);
  UpdateHashCalculator(executed_move);
  RemoveFromMoveLog(executed_move);
}

template <typename ConcreteBoardStateSummarizer>
vector<BoardSpace>
NewGameBoard<ConcreteBoardStateSummarizer>::ImplementGetAllSpacesOccupiedBy(
    PieceColor color
) {
  return get_all_spaces_occupied_by(board_map_, color);
}

template <typename ConcreteBoardStateSummarizer>
void NewGameBoard<ConcreteBoardStateSummarizer>::SetOccupant(
    BoardSpace space,
    GamePiece piece
) {
  board_map_[space.rank][space.file] = piece;
}

template <typename ConcreteBoardStateSummarizer>
bool NewGameBoard<ConcreteBoardStateSummarizer>::IsInCheck(PieceColor color) {
  auto gen_position = get_general_position(board_map_, color);
  auto opponent_moves =
      move_calculator_.CalcAllMovesNoCheckTest(opponent_of(color), board_map_);
  return opponent_moves.ContainsDestination(gen_position);
}

template <typename ConcreteBoardStateSummarizer>
MoveCollection NewGameBoard<
    ConcreteBoardStateSummarizer>::ImplementCalcFinalMovesOf(PieceColor color
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

template <typename ConcreteBoardStateSummarizer>
PieceColor NewGameBoard<ConcreteBoardStateSummarizer>::ImplementGetColor(
    BoardSpace space
) {
  return get_color(board_map_, space);
}

template <typename ConcreteBoardStateSummarizer>
PieceType NewGameBoard<ConcreteBoardStateSummarizer>::ImplementGetType(
    BoardSpace space
) {
  return get_type(board_map_, space);
}

template <typename ConcreteBoardStateSummarizer>
TranspositionTableSearchResult NewGameBoard<ConcreteBoardStateSummarizer>::
    ImplementSearchTranspositionTable(PieceColor color, int search_depth) {

  auto cur_state = hash_calculator_.GetState();

  // Check if color's transposition table has an entry for cur_state.
  auto entry_vector_it = transposition_tables_[color].find(cur_state);

  TranspositionTableSearchResult result{};

  // If table does have an entry for cur_state, entry_vector_it->second will be
  // a vector containing all TranspositionTableEntry objects that have been
  // stored for that state. Note that a single board state can have entries for
  // different search depths.
  if (entry_vector_it != transposition_tables_[color].end()) {
    auto entry_vector = entry_vector_it->second;
    for (auto entry : entry_vector) {
      // If we find an entry for search_depth of interest, then in our
      // TranspositionTableSearch result data container, we set found to true
      // and set .table_entry to the found entry.
      if (entry.search_depth == search_depth) {
        result.found = true;
        result.table_entry = entry;
      }
    }
  }

  return result;
}

template <typename ConcreteBoardStateSummarizer>
void NewGameBoard<ConcreteBoardStateSummarizer>::
    ImplementRecordCurrentStateScore(
        PieceColor color,
        int search_depth,
        MinimaxResultType result_type,
        BestMoves &best_moves
    ) {
  auto cur_state = hash_calculator_.GetState();

  // TODO: Before we add entry to our vector of entries, we should search
  // existing entries in vector to make sure we don't already have one for
  // search_depth. UPDATE: Only time we would be running this when entry for
  // search_depth is already in the vector is if we are re-running Minimax
  // after obvious hash collision (i.e. after minimax result gave us an illegal
  // move) For now, we are not worrying about correcting collisons)

  TranspositionTableEntry transposition_table_entry{
      cur_state,
      search_depth,
      result_type,
      best_moves
  };
  transposition_tables_[color][cur_state].push_back(transposition_table_entry);
}

template <typename ConcreteBoardStateSummarizer>
bool NewGameBoard<ConcreteBoardStateSummarizer>::ViolatesRepeatRule(
    PieceColor color
) {
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