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

NewGameBoard::NewGameBoard(const BoardMapInt_t
                                                         board_array)
    : board_map_{int_board_to_game_pieces(board_array)}
    , move_calculator_{MoveCalculator()} {

  // hash_calculator_red_.FullBoardStateCalc(board_map_);
  // hash_calculator_black_.FullBoardStateCalc(board_map_);

  // state_details_dispatch_table_[PieceColor::kBlk] =
  //     &NewGameBoard::GetCurrentStateDetailsBlack;
  // state_details_dispatch_table_[PieceColor::kRed] =
  //     &NewGameBoard::GetCurrentStateDetailsRed;

  // write_state_details_dispatch_table_[PieceColor::kBlk] =
  //     &NewGameBoard::RecordEvalResultBlack;
  // write_state_details_dispatch_table_[PieceColor::kRed] =
  //     &NewGameBoard::RecordEvalResultRed;
}


NewGameBoard::NewGameBoard()
    : NewGameBoard(kStartingBoard) {}

#endif