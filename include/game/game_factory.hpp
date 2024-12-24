#pragma once

#include <concepts/composite_concepts.hpp>
#include <concepts/move_evaluator.hpp>
#include <game/game.hpp>
#include <game/game_data_structs.hpp>
#include <gameboard/game_board_for_concepts.hpp>
#include <interfaces/base_evaluator_factory.hpp>
#include <interfaces/base_move_evaluator.hpp>
#include <interfaces/base_space_info_provider.hpp>
#include <interfaces/interface_game_reporter.hpp>
#include <memory>
#include <moveselection/minimax_evaluator_factory.hpp>
#include <moveselection/move_evaluator_human_for_concepts.hpp>
#include <moveselection/move_evaluator_minimax_for_concepts.hpp>
#include <moveselection/move_evaluator_random_for_concepts.hpp>
#include <terminalout/terminal_output.hpp>
#include <unordered_map>

namespace game {

class EvaluatorFactoryRetriever {
  std::unordered_map<
      MinimaxTypeInfo,
      std::shared_ptr<MoveEvaluatorFactoryBase>,
      MinimaxTypeInfoHash>
      minimax_factories_;
  const PlayerSpec &player_spec_;
  std::istream &input_stream_;
  std::shared_ptr<gameboard::GameBoardForConcepts> game_board_;

  inline static const std::unordered_map<size_t, ZobristKeyType> zkey_bitcount_to_enum_ =
      {{0, ZobristKeyType::kNoKey},
       {32, ZobristKeyType::k032BitKey},
       {64, ZobristKeyType::k064BitKey},
       {128, ZobristKeyType::k128BitKey}};

  inline static const std::unordered_map<size_t, ZobristCalculatorCount>
      num_zcalculators_to_enum_ = {
          {0, ZobristCalculatorCount::kNoZCalcs},
          {1, ZobristCalculatorCount::kOneZCalc},
          {2, ZobristCalculatorCount::kTwoZCalc}
  };

public:
  EvaluatorFactoryRetriever(
      const PlayerSpec &player_spec,
      std::shared_ptr<gameboard::GameBoardForConcepts> game_board,
      std::istream &input_stream = std::cin
  );

  std::shared_ptr<MoveEvaluatorFactoryBase> GetFactory();
};

class GameFactory {
  gameboard::GameBoardFactory game_board_factory_;
  PlayerSpec red_player_spec_;
  PlayerSpec black_player_spec_;
  std::istream &red_player_input_stream_;
  std::istream &black_player_input_stream_;
  bool report_during_game_;

public:
  GameFactory(
      PlayerSpec red_player_spec,
      PlayerSpec black_player_spec,
      std::istream &red_player_input_stream = std::cin,
      std::istream &black_player_input_stream = std::cin,
      bool report_during_game = true
  )
      : red_player_spec_{red_player_spec}
      , black_player_spec_{black_player_spec}
      , red_player_input_stream_{red_player_input_stream}
      , black_player_input_stream_{black_player_input_stream}
      , report_during_game_{report_during_game}
      , game_board_factory_{gameboard::GameBoardFactory{}} {}

  std::unique_ptr<Game> Create() {
    auto game_board = game_board_factory_.Create();

    std::unordered_map<gameboard::PieceColor, PlayerSpec> player_specs;
    player_specs.emplace(gameboard::PieceColor::kRed, red_player_spec_);
    player_specs.emplace(gameboard::PieceColor::kBlk, black_player_spec_);

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
        player_specs,
        std::move(evaluators),
        game_reporter,
        report_during_game_
    );
  }
};

} // namespace game
