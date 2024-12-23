
#include <game/game_runner.hpp>
#include <game/game_data_structs.hpp>
#include <game/player_spec_builder.hpp>
#include <gtest/gtest.h>
#include <string>


class GameRunnerTest : public ::testing::Test {
    protected:
    
};

TEST_F(GameRunnerTest, RunGameWithDefaultPlayers) {
    game::PlayerInputType red_player_input = {
        {"color", std::string("red")}
    };
    game::PlayerInputType black_player_input = {
        {"color", std::string("black")}
    };

    game::GameRunner game_runner{red_player_input, black_player_input};
    auto game_summary = game_runner.RunGame();
}