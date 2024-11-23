#pragma once

#include <game_board_for_concepts.hpp>
#include <memory>
#include <move_evaluators_for_concepts.hpp>
#include <piece_position_points_for_concepts.hpp>
#include <zobrist_for_concepts.hpp>

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
} // namespace piecepoints

namespace boardstate {
template <typename KeyType, size_t NumConfKeys>
class ZobristCoordinatorBuilder {
public:
  std::shared_ptr<ZobristCoordinatorForConcepts<KeyType, NumConfKeys>> build(
      uint32_t prng_seed
  ) {
    ZobristComponentForConcepts<KeyType, NumConfKeys> zobrist_component{prng_seed};
    return std::make_shared<ZobristCoordinatorForConcepts<KeyType, NumConfKeys>>(
        zobrist_component
    );
  }
  std::shared_ptr<ZobristCoordinatorForConcepts<KeyType, NumConfKeys>> build() {
    auto prng_seed = (uint32_t)std::random_device{}();
    ZobristComponentForConcepts<KeyType, NumConfKeys> zobrist_component{prng_seed};
    return std::make_shared<ZobristCoordinatorForConcepts<KeyType, NumConfKeys>>(
        zobrist_component
    );
  }
};
} // namespace boardstate

namespace moveselection {

class RandomMoveEvalutorBuilder {
public:
  template <SpaceInfoProviderConcept G>
  std::unique_ptr<RandomMoveEvaluatorForConcept<G>> build(
      gameboard::PieceColor evaluating_player,
      std::shared_ptr<G> game_board
  ) {
    return std::make_unique<RandomMoveEvaluatorForConcept<G>>(
        evaluating_player,
        game_board
    );
  }
};

// class MinimaxMoveEvaluatorBuilder {
// public:
//   template <
//       typename KeyType,
//       SpaceInfoProviderConcept G,
//       BoardStateCoordinatorConcept<KeyType> H,
//       PieceValueProviderConcept P>
//       std::unique_ptr<MinimaxMoveEvaluatorForConcept<KeyType, G, H, P>> build()
// }

} // namespace moveselection
