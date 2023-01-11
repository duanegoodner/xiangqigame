#ifndef _GAME_BOARD_TEMPLATE_
#define _GAME_BOARD_TEMPLATE_

#include <board_components.hpp>
#include <board_utilities.hpp>
// #include <game_board.hpp>
#include <iostream>

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
GameBoard<ConcreteHashCalculator>::GameBoard()
    : board_map_{int_board_to_game_pieces(kStartingBoard)}
    , move_calculator_{MoveCalculator()}
    , num_hash_calculators_{}
    , hash_calculators_{} {}

// template <typename ConcreteHashCalculator>
// void ImplementAttachHashCalculator(ConcreteHashCalculator& hash_calculator)
// {
//   hash_calculators_.emplace
// }

template <typename ConcreteHashCalculator>
GamePiece GameBoard<ConcreteHashCalculator>::GetOccupant(BoardSpace space) {
  return board_map_[space.rank][space.file];
}

template <typename ConcreteHashCalculator>
bool GameBoard<ConcreteHashCalculator>::IsOccupied(BoardSpace space) {
  return is_occupied(board_map_, space);
}

template <typename ConcreteHashCalculator>
void GameBoard<ConcreteHashCalculator>::UpdateHashCalculators(
    ExecutedMove executed_move
) {
  for (auto calculator_idx = 0; calculator_idx < num_hash_calculators_;
       calculator_idx++) {
    hash_calculators_[calculator_idx]->CalcNewBoardState(executed_move);
  }
}

template <typename ConcreteHashCalculator>
ExecutedMove GameBoard<ConcreteHashCalculator>::ImplementExecuteMove(Move move
) {
  auto moving_piece = GetOccupant(move.start);
  auto destination_piece = GetOccupant(move.end);
  SetOccupant(move.end, moving_piece);
  SetOccupant(move.start, GamePiece(PieceType::kNnn, PieceColor::kNul));

  auto executed_move = ExecutedMove{move, moving_piece, destination_piece};
  UpdateHashCalculators(executed_move);

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
  UpdateHashCalculators(executed_move);
  // for (auto calculator : hash_calculators_) {
  //   calculator.CalcNewBoardState(executed_move);
  // }
}

template <typename ConcreteHashCalculator>
vector<BoardSpace> GameBoard<
    ConcreteHashCalculator>::ImplementGetAllSpacesOccupiedBy(PieceColor color
) {
  // auto all_occ_spaces = get_all_spaces_occupied_by(board_map_,
  // color);
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
  return is_space_any_destination_of_moves(gen_position, opponent_moves);
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

    if (not is_space_any_destination_of_moves(
            resulting_gen_position,
            resulting_opponent_moves
        )) {
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

#endif