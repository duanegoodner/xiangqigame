
#ifndef _GAME_BOARD_
#define _GAME_BOARD_

#include <vector>
#include <board_components.hpp>
#include <move_calculator.hpp>


using namespace std;
using namespace board_components;

const BoardMap_t kStartingBoard {{
    { 5,  4,  3,  2,  1,  2,  3,  4,  5},
    { 0,  0,  0,  0,  0,  0,  0,  0,  0},
    { 0,  6,  0,  0,  0,  0,  0,  6,  0},
    { 7,  0,  7,  0,  7,  0,  7,  0,  7},
    { 0,  0,  0,  0,  0,  0,  0,  0,  0},
    { 0,  0,  0,  0,  0,  0,  0,  0,  0},
    {-7,  0, -7,  0, -7,  0, -7,  0, -7},
    { 0, -6,  0,  0,  0,  0,  0, -6,  0},
    { 0,  0,  0,  0,  0,  0,  0,  0,  0},
    {-5, -4, -3, -2, -1, -2, -3, -4, -5},
}};

class GameBoard {
    
    public:
    GameBoard();
    Piece_t GetOccupant(BoardSpace space);
    ExecutedMove ExecuteMove(Move move);
    void UndoMove(ExecutedMove executed_move);
    vector<BoardSpace> GetAllSpacesOccupiedBy(PieceColor color);
    MoveCollection CalcFinalMovesOf(PieceColor color);
    bool IsInCheck(PieceColor color);
    bool IsOccupied(BoardSpace space);
    PieceColor GetColor(BoardSpace space);
    PieceType GetType(BoardSpace space);
    const BoardMap_t& map() const { return board_map_; }

    private:
    BoardMap_t board_map_;
    MoveCalculator move_calculator_;
    void SetOccupant(BoardSpace space, Piece_t piece);
};


#endif  // _GAME_BOARD_