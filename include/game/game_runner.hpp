#pragma once

#include <any>
#include <csignal>
#include <iostream>
#include <functional>
#include <game/game.hpp>
#include <game/game_data_structs.hpp>
#include <game/game_factory.hpp>
// #include <game/player_spec_builder.hpp>
#include <string>
#include <unordered_map>

namespace game {

class GameRunner {
  game::PlayerSpec red_player_spec_;
  game::PlayerSpec black_player_spec_;

public:
  GameRunner(game::PlayerSpec red_player_spec, game::PlayerSpec black_player_spec)
      : red_player_spec_{red_player_spec}
      , black_player_spec_{black_player_spec} {}

  GameSummary RunGame() {

    auto game_factory = game::GameFactory(red_player_spec_, black_player_spec_);
    auto game = game_factory.Create();

    // Register signal handlers
    RegisterSignalHandlers([&game]() {
      if (game) {
        game->RequestStop();
      }
    });

    auto game_summary = game->Play();

    return game_summary;
  }

  const game::PlayerSpec &red_player_spec() { return red_player_spec_; }
  const game::PlayerSpec &black_player_spec() { return black_player_spec_; }

private:
  void RegisterSignalHandlers(std::function<void()> stop_callback) {
    static std::function<void()> stop_handler = stop_callback;

    std::signal(SIGINT, [](int) {
      if (stop_handler) {
        stop_handler();
      }
    });

    std::signal(SIGTERM, [](int) {
      if (stop_handler) {
        stop_handler();
      }
    });
  }
};
} // namespace game