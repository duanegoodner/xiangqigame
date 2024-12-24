
#include <game/game_data_structs.hpp>
#include <game/game_runner.hpp>
// #include <game/player_spec_builder.hpp>
#include <gtest/gtest.h>
#include <string>
#include <utilities/integer_types.hpp>

class GameRunnerTest : public ::testing::Test {
protected:
//   game::PlayerSpecBuilder player_spec_builder_;
};

TEST_F(GameRunnerTest, BuildAndRunGameBasedOnPlayerInput) {

  game::PlayerSpec red_player_spec{
      gameboard::PieceColor::kRed,
      game::EvaluatorType::kMinimax,
      64,
      2,
      3
  };

  game::PlayerSpec black_player_spec{
      gameboard::PieceColor::kBlk,
      game::EvaluatorType::kMinimax,
      64,
      2,
      3
  };

  game::GameRunner game_runner{red_player_spec, black_player_spec};
  auto game_summary = game_runner.RunGame();
}