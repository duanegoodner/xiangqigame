// Filename: piece_points.hpp
// Author: Duane Goodner
// Created: 2022-11-10
// Last Modified: 2024-10-02

// Description:
// Defines PiecePositionPoints class.

#pragma once

#include <board_components.hpp>
#include <common.hpp>
#include <move_evaluators.hpp>
#include <piece_points_spec.hpp>

using namespace gameboard;
using namespace std;

namespace piece_points {

// const string kICGABPOPath = utility_functs::get_data_file_abs_path("ICGA_2004_bpo.json");
// const string kICGARawPath = utility_functs::get_data_file_abs_path("ICGA_2004_raw.json");
// const string kRawSchemaPath =
//     utility_functs::get_data_file_abs_path("raw_points_schema.json");
// const string kBPOSchemaPath = utility_functs::get_data_file_abs_path("bpo_schema.json");

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
};

} // namespace piece_points
