#ifndef _MOVE_CALCULATOR_
#define _MOVE_CALCULATOR_

#include <any>
#include <array>
#include <cassert>
#include <cmath>
#include <functional>
#include <unordered_map>
#include <vector>
#include <board_components.hpp>
#include <board_utilities.hpp>

using namespace std;
using namespace board_components;
using namespace board_utilities;

class MoveCalculator;

class PieceMoves
{
public:
    void SoldierMoves(
        const BoardMap_t& board_map,
        PieceColor color,
        const BoardSpace& space,
        MoveCollection& team_moves);
    void CannonMoves(
        const BoardMap_t &board_map,
        PieceColor color,
        const BoardSpace& space,
        MoveCollection& team_moves);
    void ChariotMoves(
        const BoardMap_t &board_map,
        PieceColor color,
        const BoardSpace& space,
        MoveCollection& team_moves);
    void HorseMoves(
        const BoardMap_t &board_map,
        PieceColor color,
        const BoardSpace& space,
        MoveCollection& team_moves);
    void ElephantMoves(
        const BoardMap_t &board_map,
        PieceColor color,
        const BoardSpace& space,
        MoveCollection& team_moves);
    void AdvisorMoves(
        const BoardMap_t &board_map,
        PieceColor color,
        const BoardSpace& space,
        MoveCollection& team_moves);
    void FlyingGeneralMove(
        const BoardMap_t &board_map,
        PieceColor color,
        const BoardSpace& space,
        MoveCollection& team_moves);
    void StandardGeneralMoves(
        const BoardMap_t &board_map,
        PieceColor color,
        const BoardSpace& space,
        MoveCollection& team_moves);
    void GeneralMoves(
        const BoardMap_t &board_map,
        PieceColor color,
        const BoardSpace& space,
        MoveCollection& team_moves);
};

typedef void (PieceMoves::*MethodPtr_t)(
    const BoardMap_t &, PieceColor, const BoardSpace&, MoveCollection &);

class MoveCalculator
{
public:
    MoveCalculator()
        : piece_moves_{PieceMoves()}
        //, board_map_{board_map}
    {
        piece_dispatch_array_[PieceType::kNnn] = {};
        piece_dispatch_array_[PieceType::kSol] = &PieceMoves::SoldierMoves;
        piece_dispatch_array_[PieceType::kCan] = &PieceMoves::CannonMoves;
        piece_dispatch_array_[PieceType::kCha] = &PieceMoves::ChariotMoves;
        piece_dispatch_array_[PieceType::kHor] = &PieceMoves::HorseMoves;
        piece_dispatch_array_[PieceType::kEle] = &PieceMoves::ElephantMoves;
        piece_dispatch_array_[PieceType::kAdv] = &PieceMoves::AdvisorMoves;
        piece_dispatch_array_[PieceType::kGen] = &PieceMoves::GeneralMoves;
    }

    void CalcMovesFrom(const BoardSpace space, MoveCollection& team_moves, const BoardMap_t& board_map) {
        ImplementCalcMovesFrom(space, team_moves, board_map);
        }
    MoveCollection CalcAllMovesNoCheckTest(PieceColor color, const BoardMap_t& board_map) {
        return ImplementCalcAllMovesNoCheckTest(color, board_map);
        }

private:
    // const BoardMap_t &board_map_;
    array<MethodPtr_t, kNumPieceTypeVals> piece_dispatch_array_;
    PieceMoves piece_moves_;

    // https://opensource.com/article/21/2/ccc-method-pointers
    // https://stackoverflow.com/questions/6265851
    // https://stackoverflow.com/questions/55520876/
    // https://en.cppreference.com/w/cpp/utility/any/any_cast
    void ImplementCalcMovesFrom(const BoardSpace space, MoveCollection& team_moves, const BoardMap_t& board_map)
    {
        auto piece_type = get_type(board_map, space);
        auto color = get_color(board_map, space);
        auto move_func = piece_dispatch_array_[piece_type];
        auto move_func_ptr = any_cast<MethodPtr_t>(move_func);
        (piece_moves_.*move_func_ptr)(board_map, color, space, team_moves);
    }

    MoveCollection ImplementCalcAllMovesNoCheckTest(PieceColor color, const BoardMap_t& board_map)
    {
        auto untested_moves = MoveCollection(120);
        auto occ_spaces = get_all_spaces_occupied_by(board_map, color);
        for (size_t space = 0; space < occ_spaces.size(); space++)
        {
            CalcMovesFrom(occ_spaces[space], untested_moves, board_map);
            // auto moves_from_space = CalcMovesFrom(occ_spaces[space]);
            // untested_moves.Concat(moves_from_space);
        }
        return untested_moves;
    }
};

#endif // _MOVE_CALCULATOR_
