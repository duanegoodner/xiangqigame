#include <unordered_map>
#include "shared_components.hpp"

using namespace std;
using namespace BoardComponents;


typedef unordered_map<PieceType, int> BasePoints_t;

const BasePoints_t kBasePtsICGA2004 = {
    {PieceType::kGen, 6000},
    {PieceType::kAdv, 120},
    {PieceType::kEle, 120},
    {PieceType::kCha, 600},
    {PieceType::kHor, 270},
    {PieceType::kCan, 285},
    {PieceType::kSol, 30}
};


typedef array<array<int, kNumFiles>, kNumRanks> PiecePositionPts_t;

const PiecePositionPts_t kGeneralPositionICGA2004 {};
const PiecePositionPts_t kAdvisorPositionICGA2004 {};
const PiecePositionPts_t kElephantPositionICGA2004 {};

const PiecePositionPts_t kChariotPositionPtsICGA2004 {{
    { -2, 10,  6, 14, 12, 14,  6, 10, -2 },
    {  8,  4,  8, 16,  8, 16,  8,  4,  8 },
    {  4,  8,  6, 14, 12, 14,  6,  8,  4 },
    {  6, 10,  8, 14, 14, 14,  8, 10,  6 },
    { 12, 16, 14, 20, 20, 20, 14, 16, 12 },
    { 12, 14, 12, 18, 18, 18, 12, 14, 12 },
    { 12, 18, 16, 22, 22, 22, 16, 18, 12 },
    { 12, 12, 12, 18, 18, 18, 12, 12, 12 },
    { 16, 20, 18, 24, 26, 24, 18, 20, 16 },
    { 14, 14, 12, 18, 16, 18, 12, 14, 14 }
}};

const PiecePositionPts_t kHorsePositionPtsICGA2004 {{
    {  0, -4,  0,  0,  0,  0,  0, -4,  0 },
    {  0,  2,  4,  4, -2,  4,  4,  2,  0 },
    {  4,  2,  8,  8,  4,  8,  8,  2,  4 },
    {  2,  6,  8,  6, 10,  6,  8,  6,  2 },
    {  4, 12, 16, 14, 12, 14, 16, 12,  4 },
    {  6, 16, 14, 18, 16, 18, 14, 16,  6 },
    {  8, 24, 18, 24, 20, 24, 18, 24,  8 },
    { 12, 14, 16, 20, 18, 20, 16, 14, 12 },
    {  4, 10, 28, 16,  8, 16, 28, 10,  4 },
    {  4,  8, 16, 12,  4, 12, 16,  8,  4 }
}};

const PiecePositionPts_t kCannonPositionPtsICGA2004 {{
    {  0, 0,  2,   6,   6,   6,  2, 0,  0 },
    {  0, 2,  4,   6,   6,   6,  4, 2,  0 },
    {  4, 0,  8,   6,   10,  6,  8, 0,  4 },
    {  0, 0,  0,   2,   4,   2,  0, 0,  0 },
    { -2, 0,  4,   2,   6,   2,  4, 0, -2 },
    {  0, 0,  0,   2,   8,   2,  0, 0,  0 },
    {  0, 0, -2,   4,  10,   4, -2, 0,  0 },
    {  2, 2,  0, -10,  -8, -10,  0, 2,  2 },
    {  2, 2,  0, - 4, -14,  -4,  0, 2,  2 },
    {  6, 4,  0, -10, -12, -10,  0, 4,  6 }
}};

const PiecePositionPts_t kSoldierPositionPtsICGA2004 {{
    { 0,   0,  0,  0,  0,   0,  0,  0,  0 },
    { 0,   0,  0,  0,  0,   0,  0,  0,  0 },
    { 0,   0,  0,  0,  0,   0,  0,  0,  0 },
    { 0,   0, -2,  0,  0,   0, -2,  0,  0 },
    { 2,   0,  8,  0,  8,   0,  8,  0,  2 },
    { 6,  12, 18, 18,  20, 18, 18, 12,  6 },
    { 10, 20, 30, 34,  40, 34, 30, 20, 10 },
    { 14, 26, 42, 60,  80, 60, 42, 26, 14 },
    { 18, 36, 56, 80, 120, 80, 56, 36, 18 },
    { 0,   3,  6,  9,  12,  9,  6,  3,  0 }
}};

typedef unordered_map<PieceType, PiecePositionPts_t> AllPositionPts_t;

const AllPositionPts_t kPositionPtsICGA2004 = {
    {PieceType::kGen, kGeneralPositionICGA2004},
    {PieceType::kAdv, kAdvisorPositionICGA2004},
    {PieceType::kEle, kElephantPositionICGA2004},
    {PieceType::kCha, kChariotPositionPtsICGA2004},
    {PieceType::kHor, kHorsePositionPtsICGA2004},
    {PieceType::kCan, kCannonPositionPtsICGA2004},
    {PieceType::kSol, kSoldierPositionPtsICGA2004}
};

class BasePts {
    public:
    BasePts(const BasePoints_t base_point_vals);
    const BasePoints_t base_point_vals_;
};

class PositionPts {
    public:
    PositionPts(AllPositionPts_t pts_arrays_black);
    PositionPts(
        AllPositionPts_t pts_arrays_black,
        AllPositionPts_t pts_arrays_red);
    const AllPositionPts_t pts_arrays_black_;
    const AllPositionPts_t pts_arrays_red_;
};



