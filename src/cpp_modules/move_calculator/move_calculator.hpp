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
#include "shared_components.hpp"

using namespace std;
using namespace BoardComponents;


class MoveCalculator;


class PieceMoves {
    public:
    PieceMoves(MoveCalculator & p);    
    vector<Move> SoldierMoves(PieceColor color, BoardSpace space);
    vector<Move> CannonMoves(PieceColor color, BoardSpace space);
    vector<Move> ChariotMoves(PieceColor color, BoardSpace space);
    vector<Move> HorseMoves(PieceColor color, BoardSpace space);
    vector<Move> ElephantMoves(PieceColor color, BoardSpace space);
    vector<Move> AdvisorMoves(PieceColor color, BoardSpace space);
    vector<Move> FlyingGeneralMove(PieceColor color, BoardSpace space);    
    vector<Move> StandardGeneralMoves(PieceColor color, BoardSpace space);
    vector<Move> GeneralMoves(PieceColor color, BoardSpace space);

    private:
    MoveCalculator & parent_;
    
};


class MoveCalculator {
   public:
    MoveCalculator(const BoardMap_t& board_map);
    BoardUtilities utils_;
    vector<Move> CalcMovesFrom(BoardSpace space);
    vector<Move> CalcAllMovesNoCheckTest(PieceColor color);

    private:
    const BoardMap_t& board_map_;
    unordered_map<PieceType, any> piece_dispatch_;
    PieceMoves piece_moves_;

};


#endif  // _MOVE_CALCULATOR_
