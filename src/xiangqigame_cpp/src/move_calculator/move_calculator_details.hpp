#ifndef CB52FD2C_25B0_49AA_9220_93596AFF821A
#define CB52FD2C_25B0_49AA_9220_93596AFF821

#include <array>
#include <board_components.hpp>
#include <board_utilities.hpp>
#include <piece_moves.hpp>

using namespace std;
using namespace board_components;
using namespace board_utilities;

typedef void (PieceMoves::*MethodPtr_t
)(const BoardMap_t &, PieceColor, const BoardSpace &, MoveCollection &);

typedef array<MethodPtr_t, kNumPieceTypeVals> PieceDispatchArray_t;

inline constexpr PieceDispatchArray_t build_piece_dispatch_array() {
    PieceDispatchArray_t dispatch_array{};
    dispatch_array[PieceType::kNnn] = {};
    dispatch_array[PieceType::kSol] = &PieceMoves::SoldierMoves;
    dispatch_array[PieceType::kCan] = &PieceMoves::CannonMoves;
    dispatch_array[PieceType::kCha] = &PieceMoves::ChariotMoves;
    dispatch_array[PieceType::kHor] = &PieceMoves::HorseMoves;
    dispatch_array[PieceType::kEle] = &PieceMoves::ElephantMoves;
    dispatch_array[PieceType::kAdv] = &PieceMoves::AdvisorMoves;
    dispatch_array[PieceType::kGen] = &PieceMoves::GeneralMoves;

    return dispatch_array;
}

#endif /* CB52FD2C_25B0_49AA_9220_93596AFF821A */
