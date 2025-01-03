//! @file piecepoints/piece_position_points.hpp
//! Definition of piecepoints::PiecePositionPoints.

#pragma once

#include <gameboard/board_data_structs.hpp>
#include <moveselection/move_evaluators.hpp>
#include <piecepoints/base_position_offset.hpp>

using namespace gameboard;
using namespace std;

//! Providing position-dependent values of pieces to objects in moveselection namespace. 
namespace piecepoints {

//! Holds a piecepoints::GamePointsArray_t of points values, and exposes a method for
//! accessing element correspondig to specific gameboard::PieceType located at a specific
//! gameboard::BoardSpace.
struct PiecePositionPoints : public PieceValueProvider<PiecePositionPoints> {
  PiecePositionPoints();
  PiecePositionPoints(GamePointsArray_t game_points_array);
  PiecePositionPoints(BPOPointsEKeys &bpo_points_ekeys);
  PiecePositionPoints(BPOPointsSKeys &bpo_points_skeys);
  PiecePositionPoints(string json_file);

  // define in header to force inlining
  Points_t ImplementGetValueOfPieceAtPosition(
      PieceColor color,
      PieceType piece_type,
      BoardSpace space
  ) {
    return points_array[GetZColorIndexOf(color)][piece_type][space.rank][space.file];
  }

  GamePointsArray_t points_array;
  static TeamPointsEMap_t TeamPointsArrayToEMap(TeamPointsArray_t team_array);
  GamePointsEMap_t PointsArraytoEMap();
  GamePointsSMap_t PointsArrayToSmap();
};

} // namespace piecepoints
