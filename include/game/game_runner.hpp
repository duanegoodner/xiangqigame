#pragma once

#include <any>
#include <game/game.hpp>
#include <game/game_data_structs.hpp>
#include <game/player_spec_builder.hpp>
#include <string>
#include <unordered_map>

namespace game {

class GameRunner {
  std::unordered_map<std::string, std::any> red_player_input_;
  std::unordered_map<std::string, std::any> black_player_input_;

public:
  GameRunner(
      std::unordered_map<std::string, std::any> red_player_input,
      std::unordered_map<std::string, std::any> black_player_input
  )
      : red_player_input_{red_player_input}
      , black_player_input_{black_player_input} {}

  GameSummary RunGame() {}

private:

PlayerSpec BuildPlayerSpec() {
    PlayerSpecBuilder builder;

    if 

}
};
} // namespace game