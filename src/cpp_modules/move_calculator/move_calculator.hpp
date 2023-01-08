#ifndef _MOVE_CALCULATOR_
#define _MOVE_CALCULATOR_

// #include <game_board.hpp>
#include <any>
#include <array>
#include <board_components.hpp>
#include <board_utilities.hpp>
#include <game_board.hpp>
#include <cassert>
#include <cmath>
#include <functional>
#include <unordered_map>
#include <vector>

using namespace std;
using namespace board_components;
using namespace board_utilities;

template <class ImplementedPieceMoves> class PieceMovesInterface {
  public:
  void SoldierMoves(
      const BoardMap_t &board_map,
      PieceColor color,
      const BoardSpace &space,
      MoveCollection &team_moves
  ) {
    return static_cast<ImplementedPieceMoves *>(this)->ImplementSoldierMoves(
      board_map, color, space, team_moves);
  }
  void CannonMoves(
      const BoardMap_t &board_map,
      PieceColor color,
      const BoardSpace &space,
      MoveCollection &team_moves
  ) {
    return static_cast<ImplementedPieceMoves *>(this)->ImplementCannonMoves(
      board_map, color, space, team_moves);
  }
  void ChariotMoves(
      const BoardMap_t &board_map,
      PieceColor color,
      const BoardSpace &space,
      MoveCollection &team_moves
  ) {
    return static_cast<ImplementedPieceMoves *>(this)->ImplementChariotMoves(
      board_map, color, space, team_moves);
  }
  void HorseMoves(
      const BoardMap_t &board_map,
      PieceColor color,
      const BoardSpace &space,
      MoveCollection &team_moves
  ) {
    return static_cast<ImplementedPieceMoves *>(this)->ImplementHorseMoves(
      board_map, color, space, team_moves);
  }
  void ElephantMoves(
      const BoardMap_t &board_map,
      PieceColor color,
      const BoardSpace &space,
      MoveCollection &team_moves
  ) {
    return static_cast<ImplementedPieceMoves *>(this)->ImplementElephantMoves(
      board_map, color, space, team_moves);
  }
  void AdvisorMoves(
      const BoardMap_t &board_map,
      PieceColor color,
      const BoardSpace &space,
      MoveCollection &team_moves
  ) {
    return static_cast<ImplementedPieceMoves *>(this)->ImplementAdvisorMoves(
      board_map, color, space, team_moves);
  }
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
  ) {
    return static_cast<ImplementedPieceMoves *>(this)->ImplementGeneralMoves(
      board_map, color, space, team_moves);
  }
};



typedef void (StandardPieceMoves::*MethodPtr_t
)(const BoardMap_t &, PieceColor, const BoardSpace &, MoveCollection &);



class StandardMoveCalculator : public MoveCalculatorInterface<StandardMoveCalculator>{
public:
  StandardMoveCalculator()
      : piece_moves_{StandardPieceMoves()} //, board_map_{board_map}
  {
    piece_dispatch_array_[PieceType::kNnn] = {};
    piece_dispatch_array_[PieceType::kSol] = &PieceMovesInterface<StandardPieceMoves>::SoldierMoves;
    piece_dispatch_array_[PieceType::kCan] = &PieceMovesInterface<StandardPieceMoves>::CannonMoves;
    piece_dispatch_array_[PieceType::kCha] = &PieceMovesInterface<StandardPieceMoves>::ChariotMoves;
    piece_dispatch_array_[PieceType::kHor] = &PieceMovesInterface<StandardPieceMoves>::HorseMoves;
    piece_dispatch_array_[PieceType::kEle] = &PieceMovesInterface<StandardPieceMoves>::ElephantMoves;
    piece_dispatch_array_[PieceType::kAdv] = &PieceMovesInterface<StandardPieceMoves>::AdvisorMoves;
    piece_dispatch_array_[PieceType::kGen] = &PieceMovesInterface<StandardPieceMoves>::GeneralMoves;
  }

  MoveCollection CalcAllMovesNoCheckTest(
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


private:
  array<MethodPtr_t, kNumPieceTypeVals> piece_dispatch_array_;
  StandardPieceMoves piece_moves_;

  // https://opensource.com/article/21/2/ccc-method-pointers
  // https://stackoverflow.com/questions/6265851
  // https://stackoverflow.com/questions/55520876/
  // https://en.cppreference.com/w/cpp/utility/any/any_cast
  void CalcMovesFrom(
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
};

#endif // _MOVE_CALCULATOR_
