#pragma once

#include <boardstate/zobrist_factories.hpp>
#include <boardstate/zobrist_for_concepts.hpp>
#include <gameboard/game_board_for_concepts.hpp>
#include <interfaces/base_evaluator_factory.hpp>
#include <memory>
#include <moveselection/move_evaluator_minimax_for_concepts.hpp>
#include <piecepoints/piece_position_points_for_concepts.hpp>
#include <string>

namespace moveselection {
template <typename KeyType, size_t NumConfKeys>
class MinimaxMoveEvaluatorFactory : public MoveEvaluatorFactoryBase {
  using P = piecepoints::PiecePositionPointsForConcepts;
  using G = gameboard::GameBoardForConcepts;
  using Z = boardstate::ZobristCoordinatorFactory<KeyType, NumConfKeys, G>;
  Z zobrist_coordinator_factory_;
  std::shared_ptr<G> game_board_;
  DepthType search_depth_;
  const std::string &json_file_;

public:
  MinimaxMoveEvaluatorFactory(
      std::shared_ptr<G> game_board,
      DepthType search_depth,
      const std::string &json_file = piecepoints::kICGABPOPath
  )
      : zobrist_coordinator_factory_{Z{}}
      , game_board_{game_board}
      , search_depth_{search_depth}
      , json_file_{json_file} {}

  using ZobristCoordinatorType = typename boardstate::
      ZobristCoordinatorFactory<KeyType, NumConfKeys, G>::ZobristCoordinatorType;

  using MoveEvaluatorType =
      moveselection::MinimaxMoveEvaluatorForConcepts<ZobristCoordinatorType, G, P>;

  std::unique_ptr<MoveEvaluatorBase> Create(gameboard::PieceColor evaluating_player
  ) override {
    auto zobrist_coordinator =
        zobrist_coordinator_factory_.CreateRegisteredCoordinator(game_board_);

    auto game_position_points = P::Create();
    return std::make_unique<MoveEvaluatorType>(
        evaluating_player,
        search_depth_,
        game_board_,
        game_position_points,
        zobrist_coordinator
    );
  }
};
} // namespace moveselection