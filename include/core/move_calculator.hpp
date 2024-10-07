//! @file move_calculator.hpp
//! Definition of MoveCalculator 

// Filename: move_calculator.hpp
// Author: Duane Goodner
// Created: 2022-11-23
// Last Modified: 2024-08-16


#pragma once

#include <any>
#include <board_components.hpp>
// #include <board_utilities.hpp>
#include <move_calculator_details.hpp>
#include <piece_moves.hpp>

using namespace std;
using namespace gameboard;
using namespace gameboard;

namespace moves {

class MoveCalculator {
public:
  MoveCalculator()
      : piece_moves_{PieceMoves()}
      , piece_dispatch_array_(build_piece_dispatch_array()) {}

  bool IsOccupied(const BoardMap_t &board_map, const BoardSpace &space) {
    return board_map[space.rank][space.file].piece_color != PieceColor::kNul;
  }

  void CalcMovesFrom(
      const BoardSpace space,
      MoveCollection &team_moves,
      const BoardMap_t &board_map
  ) {
    ImplementCalcMovesFrom(space, team_moves, board_map);
  }
  MoveCollection CalcAllMovesNoCheckTest(PieceColor color, const BoardMap_t &board_map) {
    return ImplementCalcAllMovesNoCheckTest(color, board_map);
  }

private:
  PieceDispatchArray_t piece_dispatch_array_;
  PieceMoves piece_moves_;

  // Helpful links for using pointers to class methods  and any_cast:
  // https://opensource.com/article/21/2/ccc-method-pointers
  // https://stackoverflow.com/questions/6265851
  // https://stackoverflow.com/questions/55520876/
  // https://en.cppreference.com/w/cpp/utility/any/any_cast

  void ImplementCalcMovesFrom(
      const BoardSpace space,
      MoveCollection &team_moves,
      const BoardMap_t &board_map
  ) {
    auto piece_type = get_type(board_map, space);
    auto color = get_color(board_map, space);
    auto move_func = piece_dispatch_array_[piece_type];
    auto move_func_ptr = any_cast<MethodPtr_t>(move_func);
    (piece_moves_.*move_func_ptr)(board_map, color, space, team_moves);
  }

  MoveCollection ImplementCalcAllMovesNoCheckTest(
      PieceColor color,
      const BoardMap_t &board_map
  ) {
    auto untested_moves = MoveCollection(120);
    auto occ_spaces = get_all_spaces_occupied_by(board_map, color);
    for (size_t space = 0; space < occ_spaces.size(); space++) {
      CalcMovesFrom(occ_spaces[space], untested_moves, board_map);
    }
    return untested_moves;
  }
};

} // namespace moves
