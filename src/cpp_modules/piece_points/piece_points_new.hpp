#ifndef _PIECE_POINTS_JSON_
#define _PIECE_POINTS_JSON_

#include <board_components.hpp>
#include <common.hpp>
#include <unordered_map>
#include <vector>

using namespace board_components;
using namespace std;

team_base_points_t build_team_base_points(
    Points_t null_base,
    Points_t general_base,
    Points_t advisor_base,
    Points_t elephant_base,
    Points_t chariot_base,
    Points_t horse_base,
    Points_t cannon_base,
    Points_t soldier_base
) {
  team_base_points_t team_base_points{};
  team_base_points[PieceType::kNnn] = null_base;
  team_base_points[PieceType::kGen] = general_base;
  team_base_points[PieceType::kAdv] = advisor_base;
  team_base_points[PieceType::kEle] = elephant_base;
  team_base_points[PieceType::kCha] = chariot_base;
  team_base_points[PieceType::kHor] = horse_base;
  team_base_points[PieceType::kCan] = cannon_base;
  team_base_points[PieceType::kSol] = soldier_base;

  return team_base_points;
}

piece_position_points_t build_net_piece_points(
    Points_t piece_base,
    piece_position_points_t piece_position_points
) {
  piece_position_points_t net_piece_points{};
  for (auto rank = 0; rank < kNumRanks; rank++) {
    for (auto file = 0; file < kNumFiles; file++) {
      net_piece_points[rank][file] =
          piece_base + piece_position_points[rank][file];
    }
  }

  return piece_position_points;
}

team_points_array_t build_team_points_array(
    team_base_points_t base_points,
    piece_position_points_t null_position,
    piece_position_points_t general_position,
    piece_position_points_t advisor_position,
    piece_position_points_t elephant_position,
    piece_position_points_t chariot_position,
    piece_position_points_t horse_position,
    piece_position_points_t cannon_position,
    piece_position_points_t soldier_position
) {
  team_points_array_t team_points_array{};

  team_points_array[PieceType::kNnn] =
      build_net_piece_points(base_points[PieceType::kNnn], null_position);
  team_points_array[PieceType::kGen] =
      build_net_piece_points(base_points[PieceType::kGen], general_position);
  team_points_array[PieceType::kAdv] =
      build_net_piece_points(base_points[PieceType::kAdv], advisor_position);
  team_points_array[PieceType::kEle] =
      build_net_piece_points(base_points[PieceType::kEle], elephant_position);;
  team_points_array[PieceType::kCha] =
      build_net_piece_points(base_points[PieceType::kCha], chariot_position);
  team_points_array[PieceType::kHor] =
      build_net_piece_points(base_points[PieceType::kHor], horse_position);
  team_points_array[PieceType::kCan] =
      build_net_piece_points(base_points[PieceType::kCan], cannon_position);
  team_points_array[PieceType::kSol] =
      build_net_piece_points(base_points[PieceType::kSol], soldier_position);

  return team_points_array;
}

game_points_array_t build_game_points_array(
    team_points_array_t red_net_points,
    team_base_points_t black_net_points
    ) {
        game_points_array_t game_points_array{};
        game_points_array[get_zcolor_index(PieceColor::kRed)] = red_net_points;
        game_points_array[get_zcolor_index(PieceColor::kBlk)] = black_net_points;
        return game_points_array;
}

team_base_points_t build_ICGA2004_base_points() {
    team_base_points_t ICGA2004_base_points{};
    ICGA2004_base_points[PieceType::kNnn] = 0;
    ICGA2004_base_points[PieceType::kGen] = 6000;
    ICGA2004_base_points[PieceType::kAdv] = 120;
    ICGA2004_base_points[PieceType::kEle] = 120;
    ICGA2004_base_points[PieceType::kCha] = 600;
    ICGA2004_base_points[PieceType::kHor] = 270;
    ICGA2004_base_points[PieceType::kCan] = 285;
    ICGA2004_base_points[PieceType::kSol] = 30;

}

    // TODO move raw data vals to json file and import instead of
    // hard-coding here
    // const TeamBasePoints_t kBasePointsICGA2004 = {
    //     {PieceType::kNnn, 0},
    //     {PieceType::kGen, 6000},
    //     {PieceType::kAdv, 120},
    //     {PieceType::kEle, 120},
    //     {PieceType::kCha, 600},
    //     {PieceType::kHor, 270},
    //     {PieceType::kCan, 285},
    //     {PieceType::kSol, 30}};

const piece_position_points_t kNullOffsets{};
const piece_position_points_t kGeneralOffsetsICGA2004{};
const piece_position_points_t kAdvisorOffsetsICGA2004{};
const piece_position_points_t kElephantOffsetsICGA2004{};
const piece_position_points_t kChariotOffsetsICGA2004 = {
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
const piece_position_points_t kHorseOffsetsICGA2004 = {
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
const piece_position_points_t kCannonOffsetsICGA2004 = {
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
const piece_position_points_t kSoldierOffsetsICGA2004 = {
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


#endif // _PIECE_POINTS_JSON_