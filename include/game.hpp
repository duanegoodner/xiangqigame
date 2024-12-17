#pragma once

#include <base_move_evaluator.hpp>
#include <base_space_info_provider.hpp>
#include <board_data_structs.hpp>
#include <concept_board_state_coordinator.hpp>
#include <concept_composite_concepts.hpp>
#include <concept_move_evaluator.hpp>
#include <game_data_structs.hpp>
#include <game_piece.hpp>
#include <memory>
#include <unordered_map>

namespace game {
class Game {
  std::shared_ptr<SpaceInfoProviderBase> game_board_;
  std::unordered_map<gameboard::PieceColor, std::unique_ptr<MoveEvaluatorBase>>
      move_evaluators_;
  GameState game_state_;
  gameboard::PieceColor whose_turn_;
  std::vector<gameboard::ExecutedMove> move_log_;

public:
  Game(
      std::shared_ptr<SpaceInfoProviderBase> game_board,
      std::unordered_map<gameboard::PieceColor, std::unique_ptr<MoveEvaluatorBase>>
          move_evaluators,
      gameboard::PieceColor whose_turn = gameboard::PieceColor::kRed
  );

  void Play();

private:
  gameboard::Move GetValidMove(const gameboard::MoveCollection &available_moves);
  void PlayerTurn(const gameboard::MoveCollection &available_moves);
  void ChangeWhoseTurn();
  void SetWinner(gameboard::PieceColor color);
};

} // namespace game