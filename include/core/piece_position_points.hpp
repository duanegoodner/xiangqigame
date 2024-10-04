// Filename: piece_points.hpp
// Author: Duane Goodner
// Created: 2022-11-10
// Last Modified: 2024-10-02

// Description:
// Defines PiecePositionPoints class and other classes that PiecePositionPoints is
// composed of or help build PiecePositionPoints.

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
using namespace std;
using json = nlohmann::json;

class GamePointsArrayBuilder {
public:
  GamePointsArrayBuilder(BPOPointsEKeys internal_points_spec);
  GamePointsArrayBuilder(BPOPointsSKeys external_points_spec);
  GamePointsArrayBuilder(string spec_file_path);
  GamePointsArray_t BuildGamePointsArray();

private:
  BPOPointsEKeys points_spec_;
  TeamPointsArray_t ComputeBlackNetPoints();
  TeamPointsArray_t ComputeRedNetPoints();
};

const auto DEFAULT_GAME_POINTS_ARRAY =
    GamePointsArrayBuilder(kICGABPOPath).BuildGamePointsArray();

struct PiecePositionPoints : public PieceValueProvider<PiecePositionPoints> {
  PiecePositionPoints();
  PiecePositionPoints(GamePointsArray_t game_points_array);
  PiecePositionPoints(GamePointsSMap_t s_map);
  PiecePositionPoints(json &j);
  PiecePositionPoints(BPOPointsEKeys internal_bpo_spec);
  PiecePositionPoints(BPOPointsSKeys external_bpo_spec);
  PiecePositionPoints(string json_file);

  // define in header to force inlining
  Points_t ImplementGetValueOfPieceAtPosition(
      PieceColor color,
      PieceType piece_type,
      BoardSpace space
  ) {
    return points_array[get_zcolor_index(color)][piece_type][space.rank][space.file];
  }

  GamePointsArray_t points_array;
  json ToJson();
  void ToFile(string output_file);
};

bool json_matches_schema(const json &json_object, const json &schema);

} // namespace piece_points

#endif // _PIECE_POINTS