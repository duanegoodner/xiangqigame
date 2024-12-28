#pragma once

#include <atomic>
#include <concepts/board_state_coordinator.hpp>
#include <concepts/composite_concepts.hpp>
#include <concepts/move_evaluator.hpp>
#include <game/game_data_structs.hpp>
#include <gameboard/board_data_structs.hpp>
#include <gameboard/game_piece.hpp>
#include <interfaces/base_move_evaluator.hpp>
#include <interfaces/base_space_info_provider.hpp>
#include <interfaces/interface_game_reporter.hpp>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

namespace game {
class Game {
  std::shared_ptr<SpaceInfoProviderBase> game_board_;
  std::unordered_map<gameboard::PieceColor, PlayerSpec> player_specs_;
  std::unordered_map<gameboard::PieceColor, std::unique_ptr<MoveEvaluatorBase>>
      move_evaluators_;
  GameState game_state_;
  std::shared_ptr<GameReporterInterface> game_reporter_;
  gameboard::PieceColor whose_turn_;
  std::vector<gameboard::ExecutedMove> move_log_;
  bool report_during_game_;
  std::string game_id_;
  std::atomic_bool stop_requested_;

public:
  Game(
      std::shared_ptr<SpaceInfoProviderBase> game_board,
      std::unordered_map<gameboard::PieceColor, PlayerSpec> player_specs,
      std::unordered_map<gameboard::PieceColor, std::unique_ptr<MoveEvaluatorBase>>
          move_evaluators,
      std::shared_ptr<GameReporterInterface> game_reporter,
      bool report_during_game = true,
      gameboard::PieceColor whose_turn = gameboard::PieceColor::kRed
  );

  GameSummary Play();
  void RequestStop();

private:
  std::string GenerateGameID();
  gameboard::Move GetValidMove(const gameboard::MoveCollection &available_moves);
  void PlayerTurn(const gameboard::MoveCollection &available_moves);
  void ChangeWhoseTurn();
  void SetWinner(gameboard::PieceColor color);
  GameSummary GenerateGameSummary();
};

} // namespace game