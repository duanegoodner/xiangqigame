#include "minimax_evaluator.hpp"


PiecePointsEvaluator::PiecePointsEvaluator(PiecePoints piece_points)
: piece_points_{piece_points} {};

int PiecePointsEvaluator::GetValueOfPieceAtPosition(
    PieceColor color, PieceType piece_type, BoardSpace space) {
        return  piece_points_[space.rank][space.file];
    }