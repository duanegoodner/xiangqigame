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

using namespace gameboard;
using namespace std;
using json = nlohmann::json;

struct PiecePositionPoints : public PieceValueProvider<PiecePositionPoints> {
  PiecePositionPoints();
  PiecePositionPoints(GamePointsArray_t game_points_array);
  PiecePositionPoints(BPOPointsEKeys& bpo_points_ekeys);
  PiecePositionPoints(BPOPointsSKeys& bpo_points_skeys);
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
  static TeamPointsEMap_t TeamPointsArrayToEMap(TeamPointsArray_t team_array);
  GamePointsEMap_t PointsArraytoEMap();
  GamePointsSMap_t PointsArrayToSmap();
  // json ToJson();
  // void ToFile(string output_file);
};

} // namespace piece_points

#endif // _PIECE_POINTS