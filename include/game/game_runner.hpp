#pragma once

#include <any>
#include <game/game.hpp>
#include <game/game_data_structs.hpp>
#include <game/game_factory.hpp>
// #include <game/player_spec_builder.hpp>
#include <string>
#include <unordered_map>

namespace game {

class GameRunner {
  // const game::PlayerInputType &red_player_input_;
  // const game::PlayerInputType &black_player_input_;
  game::PlayerSpec red_player_spec_;
  game::PlayerSpec black_player_spec_;

public:
  GameRunner(
      game::PlayerSpec red_player_spec,
      game::PlayerSpec black_player_spec
  )
      : red_player_spec_{red_player_spec}
      , black_player_spec_{black_player_spec} {}

  GameSummary RunGame() {

    auto game_factory = game::GameFactory(red_player_spec_, black_player_spec_);
    auto game = game_factory.Create();
    auto game_summary = game->Play();

    return game_summary;
  }

  const game::PlayerSpec& red_player_spec() {return red_player_spec_; }
  const game::PlayerSpec& black_player_spec() {return black_player_spec_;}
};
} // namespace game