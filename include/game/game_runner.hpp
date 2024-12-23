#pragma once

#include <any>
#include <game/game.hpp>
#include <game/game_data_structs.hpp>
#include <game/game_factory.hpp>
#include <game/player_spec_builder.hpp>
#include <string>
#include <unordered_map>

namespace game {

class GameRunner {
  game::PlayerInputType red_player_input_;
  game::PlayerInputType black_player_input_;

public:
  GameRunner(
      game::PlayerInputType red_player_input,
      game::PlayerInputType black_player_input
  )
      : red_player_input_{red_player_input}
      , black_player_input_{black_player_input} {}

  GameSummary RunGame() {

    PlayerSpecBuilder player_spec_builder;
    auto red_player_spec =
        player_spec_builder.SetMultipleAttributes(red_player_input_).Build();
    auto black_player_spec =
        player_spec_builder.SetMultipleAttributes(black_player_input_).Build();

    auto game_factory = game::GameFactory(red_player_spec, black_player_spec);
    auto game = game_factory.Create();
    auto game_summary = game->Play();

    return game_summary;
  }
};
} // namespace game