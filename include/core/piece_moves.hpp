// Filename: piece_moves.hpp
// Author: Duane Goodner
// Created: 2022-11-10
// Last Modified: 2024-08-16

// Description:
// Defines PieceMoves class.

#pragma once

#include <board_data_structs.hpp>
#include <move_data_structs.hpp>

using namespace gameboard;
using namespace gameboard;

namespace moves {

extern const array<BoardDirection, 2> kSideDirections;
extern const vector<pair<BoardDirection, vector<BoardDirection>>> kHorsePaths;
extern const vector<BoardDirection> kAllOrthogonalDirections;
extern const vector<BoardDirection> kAllDiagonalDirections;

class PieceMoves {
public:
  void SoldierMoves(
      const BoardMap_t &board_map,
      PieceColor color,
      const BoardSpace &space,
      MoveCollection &team_moves
  );
  void CannonMoves(
      const BoardMap_t &board_map,
      PieceColor color,
      const BoardSpace &space,
      MoveCollection &team_moves
  );
  void ChariotMoves(
      const BoardMap_t &board_map,
      PieceColor color,
      const BoardSpace &space,
      MoveCollection &team_moves
  );
  void HorseMoves(
      const BoardMap_t &board_map,
      PieceColor color,
      const BoardSpace &space,
      MoveCollection &team_moves
  );
  void ElephantMoves(
      const BoardMap_t &board_map,
      PieceColor color,
      const BoardSpace &space,
      MoveCollection &team_moves
  );
  void AdvisorMoves(
      const BoardMap_t &board_map,
      PieceColor color,
      const BoardSpace &space,
      MoveCollection &team_moves
  );
  void FlyingGeneralMove(
      const BoardMap_t &board_map,
      PieceColor color,
      const BoardSpace &space,
      MoveCollection &team_moves
  );
  void StandardGeneralMoves(
      const BoardMap_t &board_map,
      PieceColor color,
      const BoardSpace &space,
      MoveCollection &team_moves
  );
  void GeneralMoves(
      const BoardMap_t &board_map,
      PieceColor color,
      const BoardSpace &space,
      MoveCollection &team_moves
  );

private:
  static inline BoardDirection FwdDirection(PieceColor color) {
    return BoardDirection{static_cast<int>(color), 0};
  }

  static inline bool ExistsAndPassesColorTest(
      const BoardMap_t &board_map,
      const BoardSpace &space,
      PieceColor moving_piece_color
  ) {
    return space.IsOnBoard() &&
           get_color(board_map, space) != static_cast<PieceColor>(moving_piece_color);
  }
};
} // namespace moves
