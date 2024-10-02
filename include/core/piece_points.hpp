// Filename: piece_points.hpp
// Author: Duane Goodner
// Created: 2022-11-10
// Last Modified: 2024-08-16

// Description:
// Defines PiecePoints class and other classes that PiecePoints is composed of
// or help build PiecePoints.

#ifndef _PIECE_POINTS_
#define _PIECE_POINTS_

#include <board_components.hpp>
#include <common.hpp>
#include <move_evaluators.hpp>
#include <nlohmann/json.hpp>
#include <piece_points_details.hpp>
#include <piece_points_spec.hpp>
#include <string>






namespace piece_points {

using namespace board_components;
using namespace piece_points_spec;
using namespace std;
using json = nlohmann::json;

class GamePointsArrayBuilder {
public:
  GamePointsArrayBuilder(PointsSpecBPOInternal internal_points_spec);
  GamePointsArrayBuilder(PointsSpecBPOExternal external_points_spec);
  GamePointsArrayBuilder(string spec_file_path);
  GamePointsArray_t BuildGamePointsArray();

private:
  PointsSpecBPOInternal points_spec_;
  TeamPointsArray_t ComputeBlackNetPoints();
  TeamPointsArray_t ComputeRedNetPoints();
};

const auto DEFAULT_GAME_POINTS_ARRAY =
    GamePointsArrayBuilder(kICGABPOPath).BuildGamePointsArray();

// class FormatIdentifier {
// public:
//   FormatIdentifier(json json_object, pts_format_to_schema_t schemas);
//   FormatIdentifier(json json_object);
//   PointsImportFormat Identify();

// private:
//   json json_object_;
//   pts_format_to_schema_t schemas_;
// };

// class GamePointsArrayBuilder_2 {
// public:
//   GamePointsArrayBuilder_2(json json_object);
//   GamePointsArrayBuilder_2(string json_file_path);
//   GamePointsArray_t Build();

// private:
//   json json_object_;
// };

struct PiecePoints : public PieceValueProvider<PiecePoints> {
  PiecePoints();
  PiecePoints(GamePointsArray_t game_points_array);
  PiecePoints(GamePointsSMap_t s_map);
  PiecePoints(json &j);
  PiecePoints(PointsSpecBPOInternal internal_bpo_spec);
  PiecePoints(PointsSpecBPOExternal external_bpo_spec);
  PiecePoints(string json_file);

  // define in header to force inlining
  Points_t ImplementGetValueOfPieceAtPosition(
      PieceColor color,
      PieceType piece_type,
      BoardSpace space
  ) {
    return points_array[get_zcolor_index(color)][piece_type][space.rank]
                       [space.file];
  }

  GamePointsArray_t points_array;
  json ToJson();
  void ToFile(string output_file);
};

bool json_matches_schema(const json &json_object, const json &schema);

} // namespace piece_points

#endif // _PIECE_POINTS