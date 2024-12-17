#pragma once

#include <base_evaluator_factory.hpp>
#include <base_move_evaluator.hpp>
#include <base_space_info_provider.hpp>
#include <builders.hpp>
#include <concept_composite_concepts.hpp>
#include <concept_move_evaluator.hpp>
#include <game.hpp>
#include <game_board_for_concepts.hpp>
#include <game_data_structs.hpp>
#include <memory>
#include <move_evaluator_human_for_concepts.hpp>
#include <move_evaluator_minimax_for_concepts.hpp>
#include <move_evaluator_random_for_concepts.hpp>
#include <unordered_map>

namespace game {

class EvaluatorFactoryRetriever {
  std::unordered_map<
      MinimaxTypeInfo,
      std::shared_ptr<MoveEvaluatorFactoryBase>,
      MinimaxTypeInfoHash>
      minimax_factories_;
  const PlayerInfo &evaluator_factory_info_;
  std::shared_ptr<gameboard::GameBoardForConcepts> game_board_;

public:
  EvaluatorFactoryRetriever(
      const PlayerInfo &evaluator_factory_info,
      std::shared_ptr<gameboard::GameBoardForConcepts> game_board
  );

  std::shared_ptr<MoveEvaluatorFactoryBase> GetFactory();
};

class GameFactory {
  gameboard::GameBoardFactory game_board_factory_;
  PlayerInfo red_evaluator_factory_info_;
  PlayerInfo black_evaluator_factory_info_;

public:
  GameFactory(
      PlayerInfo red_evaluator_factory_info,
      PlayerInfo black_evaluator_factory_info
  )
      : red_evaluator_factory_info_{red_evaluator_factory_info}
      , black_evaluator_factory_info_{black_evaluator_factory_info}
      , game_board_factory_{gameboard::GameBoardFactory{}} {}

  std::unique_ptr<Game> Create() {
    auto game_board = game_board_factory_.Create();
    std::unordered_map<gameboard::PieceColor, std::unique_ptr<MoveEvaluatorBase>>
        evaluators;

    auto evaluator_factory_retriever_red =
        EvaluatorFactoryRetriever{red_evaluator_factory_info_, game_board};
    auto evalutor_factory_red = evaluator_factory_retriever_red.GetFactory();
    evaluators.emplace(
        gameboard::PieceColor::kRed,
        evalutor_factory_red->Create(gameboard::PieceColor::kRed)
    );

    auto evaluator_factory_retriever_black =
        EvaluatorFactoryRetriever{black_evaluator_factory_info_, game_board};
    auto evalutor_factory_black = evaluator_factory_retriever_black.GetFactory();
    evaluators.emplace(
        gameboard::PieceColor::kBlk,
        evalutor_factory_red->Create(gameboard::PieceColor::kBlk)
    );

    return std::make_unique<Game>(game_board, std::move(evaluators));
  }
};

} // namespace game
