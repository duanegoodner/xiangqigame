
#include <game/game_runner.hpp>
#include <game/game_data_structs.hpp>
#include <game/player_spec_builder.hpp>
#include <gtest/gtest.h>
#include <string>
#include <utilities/integer_types.hpp>


class GameRunnerTest : public ::testing::Test {
    protected:
    
};

TEST_F(GameRunnerTest, BuildAndRunGameBasedOnPlayerInput) {
    game::PlayerInputType red_player_input = {
        {"color", std::string("red")},
        {"key_size_bits", size_t(64)},
        {"num_zobrist_calculators", size_t(2)},
        {"minimax_search_depth", DepthType(3)}
    };
    game::PlayerInputType black_player_input = {
        {"color", std::string("black")},
        {"key_size_bits", size_t(64)},
        {"num_zobrist_calculators", size_t(2)},
        {"minimax_search_depth", DepthType(3)}
    };

    game::GameRunner game_runner{red_player_input, black_player_input};
    auto game_summary = game_runner.RunGame();
}