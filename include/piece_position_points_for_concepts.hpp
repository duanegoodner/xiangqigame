#pragma once

#include <board_data_structs.hpp>
#include <game_piece.hpp>
#include <memory>
#include <piece_points_bpo.hpp>

//! Providing position-dependent values of pieces to objects in moveselection namespace.
namespace piecepoints {

//! Holds a piecepoints::GamePointsArray_t of points values, and exposes a method for
//! accessing element correspondig to specific gameboard::PieceType located at a specific
//! gameboard::BoardSpace.
struct PiecePositionPointsForConcepts {
  PiecePositionPointsForConcepts(GamePointsArray_t &game_points_array);

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
};

class PiecePositionPointsBuilder {
public:
  std::shared_ptr<PiecePositionPointsForConcepts> build(
      std::string json_file = piecepoints::kICGABPOPath
  );

  // Don't need any of these .build() overloads for now, but declare them so we remember
  // that we might want to implement them at some point.
  std::shared_ptr<PiecePositionPointsForConcepts> build(
      GamePointsArray_t game_points_array
  );
  std::shared_ptr<PiecePositionPointsForConcepts> build(BPOPointsEKeys &bpo_points_ekeys
  );
  std::shared_ptr<PiecePositionPointsForConcepts> build(BPOPointsSKeys &bpo_points_skeys
  );
};

} // namespace piecepoints
