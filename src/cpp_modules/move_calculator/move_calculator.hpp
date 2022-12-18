#include <any>
#include <array>
#include <cassert>
#include <cmath>
#include <functional>
#include <unordered_map>
#include <vector>

#ifndef _MOVE_CALCULATOR_
#define _MOVE_CALCULATOR_

#include "board_utilities.hpp"
#include "board_components.hpp"

using namespace std;
using namespace board_components;
using namespace board_utilities;


class MoveCalculator;


class PieceMoves {
    public:
    PieceMoves(MoveCalculator & p);    
    MoveCollection SoldierMoves(PieceColor color, BoardSpace space);
    MoveCollection CannonMoves(PieceColor color, BoardSpace space);
    MoveCollection ChariotMoves(PieceColor color, BoardSpace space);
    MoveCollection HorseMoves(PieceColor color, BoardSpace space);
    MoveCollection ElephantMoves(PieceColor color, BoardSpace space);
    MoveCollection AdvisorMoves(PieceColor color, BoardSpace space);
    MoveCollection FlyingGeneralMove(PieceColor color, BoardSpace space);    
    MoveCollection StandardGeneralMoves(PieceColor color, BoardSpace space);
    MoveCollection GeneralMoves(PieceColor color, BoardSpace space);

    private:
    MoveCalculator & parent_;
    
};


class MoveCalculator {
   public:
    MoveCalculator(const BoardMap_t& board_map);
    BoardUtilities utils_;
    MoveCollection CalcMovesFrom(BoardSpace space);
    MoveCollection CalcAllMovesNoCheckTest(PieceColor color);

    private:
    const BoardMap_t& board_map_;
    unordered_map<PieceType, any> piece_dispatch_;
    PieceMoves piece_moves_;

};


#endif  // _MOVE_CALCULATOR_
