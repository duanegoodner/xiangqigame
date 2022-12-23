#include <any>
#include <array>
#include <cassert>
#include <cmath>
#include <functional>
#include <unordered_map>
#include <vector>

#ifndef _MOVE_CALCULATOR_
#define _MOVE_CALCULATOR_

#include "board_components.hpp"
#include "board_utilities_free.hpp"

using namespace std;
using namespace board_components;
using namespace board_utilities_free;

class MoveCalculator;

class PieceMoves {
    public: 
    MoveCollection SoldierMoves(
        const BoardMap_t &board_map, PieceColor color, BoardSpace space);
    MoveCollection CannonMoves(
        const BoardMap_t &board_map, PieceColor color, BoardSpace space);
    MoveCollection ChariotMoves(
        const BoardMap_t &board_map, PieceColor color, BoardSpace space);
    MoveCollection HorseMoves(
        const BoardMap_t &board_map, PieceColor color, BoardSpace space);
    MoveCollection ElephantMoves(
        const BoardMap_t &board_map, PieceColor color, BoardSpace space);
    MoveCollection AdvisorMoves(
        const BoardMap_t &board_map, PieceColor color, BoardSpace space);
    MoveCollection FlyingGeneralMove(
        const BoardMap_t &board_map, PieceColor color, BoardSpace space);    
    MoveCollection StandardGeneralMoves(
        const BoardMap_t &board_map, PieceColor color, BoardSpace space);
    MoveCollection GeneralMoves(
        const BoardMap_t &board_map, PieceColor color, BoardSpace space);    
};


typedef MoveCollection (PieceMoves::*MethodPtr_t)(
    const BoardMap_t&, PieceColor, BoardSpace);


class MoveCalculator {
   public:
    MoveCalculator(const BoardMap_t &board_map)
    : board_map_{board_map}
    , piece_moves_{PieceMoves()}
{
    piece_dispatch_[PieceType::kSol] = &PieceMoves::SoldierMoves;
    piece_dispatch_[PieceType::kCan] = &PieceMoves::CannonMoves;
    piece_dispatch_[PieceType::kCha] = &PieceMoves::ChariotMoves;
    piece_dispatch_[PieceType::kHor] = &PieceMoves::HorseMoves;
    piece_dispatch_[PieceType::kEle] = &PieceMoves::ElephantMoves;
    piece_dispatch_[PieceType::kAdv] = &PieceMoves::AdvisorMoves;
    piece_dispatch_[PieceType::kGen] = &PieceMoves::GeneralMoves;
}
    // BoardUtilities utils_;
    MoveCollection CalcMovesFrom(BoardSpace space)
{
    auto piece_type = get_type(board_map_, space);
    auto color = get_color(board_map_, space);
    auto move_func = piece_dispatch_.find(piece_type)->second;
    auto move_func_ptr = any_cast<MethodPtr_t>(move_func);
    return (piece_moves_.*move_func_ptr)(board_map_, color, space);
}

    MoveCollection CalcAllMovesNoCheckTest(PieceColor color)
{
    auto untested_moves = MoveCollection(120);
    auto occ_spaces = get_all_spaces_occupied_by(board_map_, color);
    for (auto space = 0; space < occ_spaces.size(); space++)
    {
        auto moves_from_space = CalcMovesFrom(occ_spaces[space]);
            untested_moves.Concat(moves_from_space);
    }
    return untested_moves;
}

    private:
    const BoardMap_t& board_map_;
    unordered_map<PieceType, any> piece_dispatch_;
    PieceMoves piece_moves_;

};


#endif  // _MOVE_CALCULATOR_
