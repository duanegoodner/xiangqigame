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
#include <interface_game_reporter.hpp>
#include <memory>
#include <move_evaluator_human_for_concepts.hpp>
#include <move_evaluator_minimax_for_concepts.hpp>
#include <move_evaluator_random_for_concepts.hpp>
#include <terminal_output.hpp>
#include <unordered_map>

namespace game {

class EvaluatorFactoryRetriever {
  std::unordered_map<
      MinimaxTypeInfo,
      std::shared_ptr<MoveEvaluatorFactoryBase>,
      MinimaxTypeInfoHash>
      minimax_factories_;
  const PlayerSpec &evaluator_factory_info_;
  std::shared_ptr<gameboard::GameBoardForConcepts> game_board_;

public:
  EvaluatorFactoryRetriever(
      const PlayerSpec &evaluator_factory_info,
      std::shared_ptr<gameboard::GameBoardForConcepts> game_board
  );

  std::shared_ptr<MoveEvaluatorFactoryBase> GetFactory();
};

class GameFactory {
  gameboard::GameBoardFactory game_board_factory_;
  PlayerSpec red_player_spec_;
  PlayerSpec black_player_spec_;
  bool report_during_game_;

public:
  GameFactory(
      PlayerSpec red_player_spec,
      PlayerSpec black_player_spec,
      bool report_during_game = true
  )
      : red_player_spec_{red_player_spec}
      , black_player_spec_{black_player_spec}
      , report_during_game_{report_during_game}
      , game_board_factory_{gameboard::GameBoardFactory{}} {}

  std::unique_ptr<Game> Create() {
    auto game_board = game_board_factory_.Create();
    std::unordered_map<gameboard::PieceColor, std::unique_ptr<MoveEvaluatorBase>>
        evaluators;

    auto evaluator_factory_retriever_red =
        EvaluatorFactoryRetriever{red_player_spec_, game_board};
    auto evalutor_factory_red = evaluator_factory_retriever_red.GetFactory();
    evaluators.emplace(
        gameboard::PieceColor::kRed,
        evalutor_factory_red->Create(gameboard::PieceColor::kRed)
    );

    auto evaluator_factory_retriever_black =
        EvaluatorFactoryRetriever{black_player_spec_, game_board};
    auto evalutor_factory_black = evaluator_factory_retriever_black.GetFactory();
    evaluators.emplace(
        gameboard::PieceColor::kBlk,
        evalutor_factory_red->Create(gameboard::PieceColor::kBlk)
    );

    std::shared_ptr<GameReporterInterface> game_reporter =
        std::make_shared<terminalout::TerminalGameReporter>(
            red_player_spec_,
            black_player_spec_
        );

    return std::make_unique<Game>(
        game_board,
        std::move(evaluators),
        game_reporter,
        report_during_game_
    );
  }
};

} // namespace game
