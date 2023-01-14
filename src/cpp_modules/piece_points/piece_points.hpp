#ifndef _PIECE_POINTS_
#define _PIECE_POINTS_

#include <board_components.hpp>
#include <common.hpp>
#include <json.hpp>
#include <string>
#include <unordered_map>
#include <vector>

namespace piece_points {

using namespace board_components;
using namespace std;
using json = nlohmann::json;

struct PointsSpecExternal {
  PointsSpecExternal(
      base_points_map_t black_base_input,
      base_points_map_t red_base_offsets_input,
      position_points_map_t black_position_input,
      position_points_map_t red_position_offsets_input
  );
  PointsSpecExternal(const json &json_object);
  PointsSpecExternal(string json_file_path);
  //   PointsSpecExternal(string json_file_path);
  base_points_map_t black_base;
  base_points_map_t red_base_offsets;
  position_points_map_t black_position;
  position_points_map_t red_position_offsets;
  json ToJson();
};

struct PointsSpecInternal {
  PointsSpecInternal(
      TeamBasePoints_t black_base_input,
      TeamBasePoints_t red_base_offsets_input,
      TeamPositionPoints_t black_position_input,
      TeamPositionPoints_t red_position_offsets_input
  );
  PointsSpecInternal(PointsSpecExternal external_spec);

  TeamBasePoints_t black_base;
  TeamBasePoints_t red_base_offsets;
  TeamPositionPoints_t black_position;
  TeamPositionPoints_t red_position_offsets;
};

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

const base_points_map_t kBasePointsMapICGA2004 = {
    {"null", 0},
    {"general", 6000},
    {"advisor", 120},
    {"elephant", 120},
    {"chariot", 600},
    {"horse", 270},
    {"cannon", 285},
    {"soldier", 30}};

const base_points_map_t kNullBasePointsOffsetMap = {
    {"null", 0},
    {"general", 0},
    {"advisor", 0},
    {"elephant", 0},
    {"chariot", 0},
    {"horse", 0},
    {"cannon", 0},
    {"soldier", 0}};

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

const position_points_map_t kPositionPointsMapICGA2004 = {
    {"null", kNullOffsets},
    {"general", kGeneralOffsetsICGA2004},
    {"advisor", kAdvisorOffsetsICGA2004},
    {"elephant", kElephantOffsetsICGA2004},
    {"chariot", piece_points::kChariotOffsetsICGA2004},
    {"horse", kHorseOffsetsICGA2004},
    {"cannon", kCannonOffsetsICGA2004},
    {"soldier", kSoldierOffsetsICGA2004}};

const position_points_map_t kNullPositionPointsOffsetMap = {
    {"null", kNullOffsets},
    {"general", kNullOffsets},
    {"advisor", kNullOffsets},
    {"elephant", kNullOffsets},
    {"chariot", kNullOffsets},
    {"horse", kNullOffsets},
    {"cannon", kNullOffsets},
    {"soldier", kNullOffsets}};

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
  PiecePointsBuilder(PointsSpecInternal points_spec);
  GamePositionPoints_t BuildGamePositionPoints();

private:
  PointsSpecInternal points_spec_;
  PiecePositionPoints_t FlipBoardDirection(PiecePositionPoints_t orig_piece_pts
  );
  PiecePositionPoints_t PiecePointsArraySum(
      PiecePositionPoints_t a,
      PiecePositionPoints_t b
  );
  TeamPositionPoints_t ComputeBlackNetPoints();
  TeamPositionPoints_t ComputeRedNetPoints();
  PiecePositionPoints_t ComputePieceNetPoints(
      Points_t base,
      PiecePositionPoints_t position_points
  );
};


const string kICGAPath = "/home/duane/workspace/project/src/cpp_modules/piece_points/"
                 "ICGA_2004.json";
const PointsSpecExternal DEFAULT_EXTERNAL_POINTS_SPEC =
    PointsSpecExternal(kICGAPath);
const PointsSpecInternal DEFAULT_INTERNAL_POINTS_SPEC =
    PointsSpecInternal(DEFAULT_EXTERNAL_POINTS_SPEC);
const auto DEFAULT_GAME_POINTS =
    PiecePointsBuilder(DEFAULT_INTERNAL_POINTS_SPEC).BuildGamePositionPoints();


} // namespace piece_points

#endif // _PIECE_POINTS