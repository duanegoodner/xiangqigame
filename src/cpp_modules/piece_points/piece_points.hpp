#ifndef _PIECE_POINTS_
#define _PIECE_POINTS_

#include <board_components.hpp>
#include <unordered_map>
#include <vector>

namespace piece_points {

using namespace board_components;
using namespace std;

// TODO move typedefs to single file (where possible)
typedef int Points_t;
typedef unordered_map<PieceType, Points_t> TeamBasePoints_t;
typedef array<array<int, kNumFiles>, kNumRanks> PiecePositionPoints_t;
typedef unordered_map<PieceType, PiecePositionPoints_t> TeamPositionPoints_t;
typedef unordered_map<PieceColor, TeamPositionPoints_t> GamePositionPoints_t;

// TODO move raw data vals to json file and import instead of
// hard-coding here
const TeamBasePoints_t kBasePointsICGA2004 = {
    {PieceType::kNnn, 0},
    {PieceType::kGen, 6000},
    {PieceType::kAdv, 120},
    {PieceType::kEle, 120},
    {PieceType::kCha, 600},
    {PieceType::kHor, 270},
    {PieceType::kCan, 285},
    {PieceType::kSol, 30}};

const PiecePositionPoints_t kNullOffsets{};
const PiecePositionPoints_t kGeneralOffsetsICGA2004{};
const PiecePositionPoints_t kAdvisorOffsetsICGA2004{};
const PiecePositionPoints_t kElephantOffsetsICGA2004{};
const PiecePositionPoints_t kChariotOffsetsICGA2004 = {
    {{-2, 10, 6, 14, 12, 14, 6, 10, -2},
     {8, 4, 8, 16, 8, 16, 8, 4, 8},
     {4, 8, 6, 14, 12, 14, 6, 8, 4},
     {6, 10, 8, 14, 14, 14, 8, 10, 6},
     {12, 16, 14, 20, 20, 20, 14, 16, 12},
     {12, 14, 12, 18, 18, 18, 12, 14, 12},
     {12, 18, 16, 22, 22, 22, 16, 18, 12},
     {12, 12, 12, 18, 18, 18, 12, 12, 12},
     {16, 20, 18, 24, 26, 24, 18, 20, 16},
     {14, 14, 12, 18, 16, 18, 12, 14, 14}}};
const PiecePositionPoints_t kHorseOffsetsICGA2004 = {
    {{0, -4, 0, 0, 0, 0, 0, -4, 0},
     {0, 2, 4, 4, -2, 4, 4, 2, 0},
     {4, 2, 8, 8, 4, 8, 8, 2, 4},
     {2, 6, 8, 6, 10, 6, 8, 6, 2},
     {4, 12, 16, 14, 12, 14, 16, 12, 4},
     {6, 16, 14, 18, 16, 18, 14, 16, 6},
     {8, 24, 18, 24, 20, 24, 18, 24, 8},
     {12, 14, 16, 20, 18, 20, 16, 14, 12},
     {4, 10, 28, 16, 8, 16, 28, 10, 4},
     {4, 8, 16, 12, 4, 12, 16, 8, 4}}};
const PiecePositionPoints_t kCannonOffsetsICGA2004 = {
    {{0, 0, 2, 6, 6, 6, 2, 0, 0},
     {0, 2, 4, 6, 6, 6, 4, 2, 0},
     {4, 0, 8, 6, 10, 6, 8, 0, 4},
     {0, 0, 0, 2, 4, 2, 0, 0, 0},
     {-2, 0, 4, 2, 6, 2, 4, 0, -2},
     {0, 0, 0, 2, 8, 2, 0, 0, 0},
     {0, 0, -2, 4, 10, 4, -2, 0, 0},
     {2, 2, 0, -10, -8, -10, 0, 2, 2},
     {2, 2, 0, -4, -14, -4, 0, 2, 2},
     {6, 4, 0, -10, -12, -10, 0, 4, 6}}};
const PiecePositionPoints_t kSoldierOffsetsICGA2004 = {
    {{0, 0, 0, 0, 0, 0, 0, 0, 0},
     {0, 0, 0, 0, 0, 0, 0, 0, 0},
     {0, 0, 0, 0, 0, 0, 0, 0, 0},
     {0, 0, -2, 0, 0, 0, -2, 0, 0},
     {2, 0, 8, 0, 8, 0, 8, 0, 2},
     {6, 12, 18, 18, 20, 18, 18, 12, 6},
     {10, 20, 30, 34, 40, 34, 30, 20, 10},
     {14, 26, 42, 60, 80, 60, 42, 26, 14},
     {18, 36, 56, 80, 120, 80, 56, 36, 18},
     {0, 3, 6, 9, 12, 9, 6, 3, 0}}};

const TeamPositionPoints_t kAllOffsetsICGA2004 = {
    {PieceType::kGen, kGeneralOffsetsICGA2004},
    {PieceType::kAdv, kAdvisorOffsetsICGA2004},
    {PieceType::kEle, kElephantOffsetsICGA2004},
    {PieceType::kCha, kChariotOffsetsICGA2004},
    {PieceType::kHor, kHorseOffsetsICGA2004},
    {PieceType::kCan, kCannonOffsetsICGA2004},
    {PieceType::kSol, kSoldierOffsetsICGA2004}};

class PiecePointsBuilder {
public:
  GamePositionPoints_t BuildGamePositionPoints(
      TeamPositionPoints_t black_points
  );

  GamePositionPoints_t BuildGamePositionPoints(
      TeamBasePoints_t black_base_points,
      TeamPositionPoints_t black_position_offsets
  );

private:
  PiecePositionPoints_t ComputePieceNetPositionPoints(
      int base_val,
      PiecePositionPoints_t position_offsets
  );

  TeamPositionPoints_t ComputeBlackPositionPoints(
      TeamBasePoints_t black_base_pts,
      TeamPositionPoints_t black_position_offsets
  );

  PiecePositionPoints_t FlipBoardDirection(PiecePositionPoints_t orig_piece_pts
  );

  TeamPositionPoints_t ComputeRedPositionPoints(
      TeamPositionPoints_t black_position_points
  );
};

const auto DEFAULT_GAME_POINTS = PiecePointsBuilder().BuildGamePositionPoints(
    kBasePointsICGA2004,
    kAllOffsetsICGA2004
);

} // namespace piece_points

#endif // _PIECE_POINTS