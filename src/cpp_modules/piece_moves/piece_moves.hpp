#ifndef _PIECE_MOVES_
#define _PIECE_MOVES_

#include <move_calculator.hpp>

class StandardPieceMoves : public PieceMovesInterface<StandardPieceMoves>{
public:
  void ImplementSoldierMoves(
      const BoardMap_t &board_map,
      PieceColor color,
      const BoardSpace &space,
      MoveCollection &team_moves
  );
  void ImplementCannonMoves(
      const BoardMap_t &board_map,
      PieceColor color,
      const BoardSpace &space,
      MoveCollection &team_moves
  );
  void ImplementChariotMoves(
      const BoardMap_t &board_map,
      PieceColor color,
      const BoardSpace &space,
      MoveCollection &team_moves
  );
  void ImplementHorseMoves(
      const BoardMap_t &board_map,
      PieceColor color,
      const BoardSpace &space,
      MoveCollection &team_moves
  );
  void ImplementElephantMoves(
      const BoardMap_t &board_map,
      PieceColor color,
      const BoardSpace &space,
      MoveCollection &team_moves
  );
  void ImplementAdvisorMoves(
      const BoardMap_t &board_map,
      PieceColor color,
      const BoardSpace &space,
      MoveCollection &team_moves
  );
  void ImplementGeneralMoves(
      const BoardMap_t &board_map,
      PieceColor color,
      const BoardSpace &space,
      MoveCollection &team_moves
  );
  private:
    void ImplementFlyingGeneralMove(
      const BoardMap_t &board_map,
      PieceColor color,
      const BoardSpace &space,
      MoveCollection &team_moves
  );
  void ImplementStandardGeneralMoves(
      const BoardMap_t &board_map,
      PieceColor color,
      const BoardSpace &space,
      MoveCollection &team_moves
  );

};

#endif
