#pragma once


#include <board_data_structs.hpp>
#include <game_piece.hpp>
#include <piece_points_bpo.hpp>



//! Providing position-dependent values of pieces to objects in moveselection namespace. 
namespace piecepoints {

//! Holds a piecepoints::GamePointsArray_t of points values, and exposes a method for
//! accessing element correspondig to specific gameboard::PieceType located at a specific
//! gameboard::BoardSpace.
struct PiecePositionPointsForConcepts {
  PiecePositionPointsForConcepts();
  PiecePositionPointsForConcepts(GamePointsArray_t game_points_array);
  PiecePositionPointsForConcepts(BPOPointsEKeys &bpo_points_ekeys);
  PiecePositionPointsForConcepts(BPOPointsSKeys &bpo_points_skeys);
  PiecePositionPointsForConcepts(string json_file);

  // define in header to force inlining
  inline Points_t GetValueOfPieceAtPosition(
      gameboard::PieceColor color,
      gameboard::PieceType piece_type,
      gameboard::BoardSpace &space
  ) {
    return points_array[GetZColorIndexOf(color)][piece_type][space.rank][space.file];
  }

  GamePointsArray_t points_array;
  static TeamPointsEMap_t TeamPointsArrayToEMap(TeamPointsArray_t team_array);
  GamePointsEMap_t PointsArraytoEMap();
  GamePointsSMap_t PointsArrayToSmap();
};

} // namespace piecepoints
