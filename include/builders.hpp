#pragma once

#include <game_board_for_concepts.hpp>
#include <memory>
#include <piece_position_points_for_concepts.hpp>
namespace gameboard {

class GameBoardBuilder {
public:
  std::shared_ptr<GameBoardForConcepts> build(
      const BoardMapInt_t starting_board = kStandardInitialBoard
  );
};

} // namespace gameboard


namespace piecepoints {
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
}



