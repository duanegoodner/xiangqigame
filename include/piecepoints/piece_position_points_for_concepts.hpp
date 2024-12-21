#pragma once

#include <gameboard/board_data_structs.hpp>
#include <gameboard/game_piece.hpp>
#include <memory>
#include <piecepoints/base_position_offset.hpp>
#include <string>

//! Providing position-dependent values of pieces to objects in moveselection namespace.
namespace piecepoints {

//! Holds a piecepoints::GamePointsArray_t of points values, and exposes a method for
//! accessing element correspondig to specific gameboard::PieceType located at a specific
//! gameboard::BoardSpace.
struct PiecePositionPointsForConcepts {

  static std::shared_ptr<PiecePositionPointsForConcepts> Create(
      std::string json_file = piecepoints::kICGABPOPath
  );

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
  GamePointsEMap_t PointsArrayToEMap();
  GamePointsSMap_t PointsArrayToSmap();

private:
  PiecePositionPointsForConcepts(GamePointsArray_t &game_points_array);
};

} // namespace piecepoints
