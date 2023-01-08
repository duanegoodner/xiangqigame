
#ifndef _GAME_BOARD_
#define _GAME_BOARD_

#include <board_components.hpp>
#include <common.hpp>
#include <hash_calculator.hpp>
#include <vector>
// #include <move_calculator.hpp>

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


template <class ImplementedMoveCalculator> class MoveCalculatorInterface {
public:
  MoveCollection CalcAllMovesNoCheckTest(
      PieceColor color,
      const BoardMap_t &board_map
  ) {
    return static_cast<ImplementedMoveCalculator *>(this)
        ->ImplementCalcAllMovesNoCheckTest(color, board_map);
  }
};


template <class ImplementedMoveCalculator> class GameBoard {

private:
  BoardMap_t board_map_;
  ImplementedMoveCalculator move_calculator_;
  HashCalculator hash_calculator_;
  void SetOccupant(BoardSpace space, GamePiece piece);
  zkey_t board_state_;

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
  };

#endif // _GAME_BOARD_