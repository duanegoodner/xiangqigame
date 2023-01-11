
#ifndef _GAME_BOARD_
#define _GAME_BOARD_

#include <board_components.hpp>
#include <common.hpp>
#include <vector>
// #include <hash_calculator.hpp>
#include <minimax_evaluator.hpp>
#include <move_calculator.hpp>
#include <move_selector.hpp>

using namespace std;
using namespace board_components;

const BoardMapInt_t kStartingBoard{{
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

inline BoardMap_t int_board_to_game_pieces(const BoardMapInt_t int_board);

// CRTP Interface: GameBoard <- HashCalculator
template <typename ConcreteHashCalculator>
class BoardStateTracker {
  void CalcNewBoardState(const ExecutedMove &move) {
    return static_cast<ConcreteHashCalculator *>(this)
        ->ImplementCalcNewBoardState(move);
  }
};

template <typename ConcreteHashCalculator>
class GameBoard : public MoveTracker<GameBoard<ConcreteHashCalculator>>,
                  public SpaceInfoProvider<GameBoard<ConcreteHashCalculator>> {

public:
  GameBoard();
  bool IsOccupied(BoardSpace space);
  GamePiece GetOccupant(BoardSpace space);
  ExecutedMove ImplementExecuteMove(Move move);
  void ImplementUndoMove(ExecutedMove executed_move);
  vector<BoardSpace> ImplementGetAllSpacesOccupiedBy(PieceColor color);
  MoveCollection ImplementCalcFinalMovesOf(PieceColor color);
  bool IsInCheck(PieceColor color);
  PieceColor ImplementGetColor(BoardSpace space);
  PieceType ImplementGetType(BoardSpace space);
  const BoardMap_t &map() const { return board_map_; }

private:
  BoardMap_t board_map_;
  MoveCalculator move_calculator_;
  vector<ConcreteHashCalculator> hash_calculators_;
  void SetOccupant(BoardSpace space, GamePiece piece);
};

#include <game_board.tpp>

#endif // _GAME_BOARD_