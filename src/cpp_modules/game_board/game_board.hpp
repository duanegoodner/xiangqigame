
#ifndef _GAME_BOARD_
#define _GAME_BOARD_

#include <board_components.hpp>
#include <hash_calculator.hpp>
#include <move_calculator.hpp>
#include <vector>

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

BoardMap_t int_board_to_game_pieces(const BoardMapInt_t int_board);

class GameBoard {

public:
  GameBoard();
  bool IsOccupied(BoardSpace space);
  GamePiece GetOccupant(BoardSpace space);
  ExecutedMove ExecuteMove(Move move);
  void UndoMove(ExecutedMove executed_move);
  vector<BoardSpace> GetAllSpacesOccupiedBy(PieceColor color);
  MoveCollection CalcFinalMovesOf(PieceColor color);
  bool IsInCheck(PieceColor color);
  PieceColor GetColor(BoardSpace space);
  PieceType GetType(BoardSpace space);
  const BoardMap_t &map() const { return board_map_; }
  zkey_t board_state();

private:
  BoardMap_t board_map_;
  MoveCalculator move_calculator_;
  HashCalculator hash_calculator_;
  zkey_t board_state_;
  void SetOccupant(BoardSpace space, GamePiece piece);
};

#endif // _GAME_BOARD_