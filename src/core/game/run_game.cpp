#include <game/game_data_structs.hpp>
#include <game/game_runner.hpp>
#include <gameboard/game_piece.hpp>

int main() {

  game::PlayerSpec red_player_spec{
      gameboard::PieceColor::kRed,
      game::EvaluatorType::kMinimax
  };
  game::PlayerSpec black_player_spec{
      gameboard::PieceColor::kBlk,
      game::EvaluatorType::kMinimax
  };

  game::GameRunner game_runner{red_player_spec, black_player_spec};
  auto game_summary = game_runner.RunGame();

  return 0;
}