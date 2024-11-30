#pragma once

#include <concept_board_state_calculator.hpp>
#include <game_board_for_concepts.hpp>
#include <memory>
#include <move_evaluators_for_concepts.hpp>
#include <piece_position_points_for_concepts.hpp>
#include <zobrist_for_concepts.hpp>

namespace boardstate {

// template <typename KeyType>
// class ZobristCalculatorBuilder {
// public:
//   std::shared_ptr<ZobristCalculatorForConcepts<KeyType>> build(
//       uint32_t seed = std::random_device{}()
//   ) {
//     return std::make_shared<ZobristCalculatorForConcepts<KeyType>>(seed);
//   }
// };

} // namespace boardstate

namespace gameboard {

template <BoardStateCalculatorConcept BC, BoardStateCalculatorConcept RC>
class GameBoardBuilder {
public:
  std::shared_ptr<GameBoardForConcepts<RC, BC>> build(
      std::vector<std::shared_ptr<RC>> red_z_calculators,
      std::vector<std::shared_ptr<BC>> black_z_calculators,
      const BoardMapInt_t starting_board = kStandardInitialBoard
  ) {

    auto game_board = std::make_shared<GameBoardForConcepts<RC, BC>>(
        red_z_calculators,
        black_z_calculators,
        starting_board
    );

    for (auto calculator : red_z_calculators) {
      calculator->FullBoardStateCalc(game_board->map());
    }
    for (auto calculator : black_z_calculators) {
      calculator->FullBoardStateCalc(game_board->map());
    }
    return game_board;

    // return std::make_shared<GameBoardForConcepts<RC, BC>>(
    //     red_z_calculators,
    //     black_z_calculators,
    //     starting_board
    // );
  }
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

class MinimaxMoveEvaluatorBuilder {
public:
  template <
      typename KeyType,
      SpaceInfoProviderConcept G,
      BoardStateCoordinatorConcept<KeyType> H,
      PieceValueProviderConcept P>
  std::unique_ptr<MinimaxMoveEvaluatorForConcept<KeyType, G, H, P>> build(
      gameboard::PieceColor evaluating_player,
      DepthType search_depth,
      std::shared_ptr<G> game_board,
      std::shared_ptr<P> game_position_points,
      std::shared_ptr<H> hash_calculator
  ) {
    return std::make_unique<MinimaxMoveEvaluatorForConcept<KeyType, G, H, P>>(
        evaluating_player,
        search_depth,
        game_board,
        game_position_points,
        hash_calculator
    );
  }
};

} // namespace moveselection
