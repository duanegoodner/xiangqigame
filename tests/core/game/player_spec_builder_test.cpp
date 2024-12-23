
#include <game/game_data_structs.hpp>
#include <game/player_spec_builder.hpp>
#include <gameboard/board_data_structs.hpp>
#include <gtest/gtest.h>
#include <unordered_map>

class PlayerSpecBuilderTest : public ::testing::Test {
protected:
  game::PlayerSpecBuilder player_spec_builder_;
};

TEST_F(PlayerSpecBuilderTest, BuildDefault) {
  auto player_spec = player_spec_builder_.Build();

  EXPECT_EQ(player_spec.color, gameboard::PieceColor::kNul);
  EXPECT_EQ(player_spec.evaluator_type, game::EvaluatorType::kMinimax);
  EXPECT_EQ(player_spec.zobrist_key_type, game::ZobristKeyType::k064BitKey);
  EXPECT_EQ(
      player_spec.zobrist_calculator_count,
      game::ZobristCalculatorCount::kTwoZCalc
  );
  EXPECT_EQ(player_spec.minimax_search_depth, 4);
}

TEST_F(PlayerSpecBuilderTest, BuildFromPlayerInputType) {
  game::PlayerInputType input = {
      {"color", std::string("black")},
      {"evaluator_type", std::string("minimax")},
      {"key_size_bits", size_t(32)},
      {"num_zobrist_calculators", size_t(1)},
      {"minimax_search_depth", DepthType(3)}
  };

  auto player_spec = player_spec_builder_.SetMultipleAttributes(input).Build();
  EXPECT_EQ(player_spec.color, gameboard::PieceColor::kBlk);
  EXPECT_EQ(player_spec.evaluator_type, game::EvaluatorType::kMinimax);
  EXPECT_EQ(player_spec.zobrist_key_type, game::ZobristKeyType::k032BitKey);
  EXPECT_EQ(player_spec.zobrist_calculator_count, game::ZobristCalculatorCount::kOneZCalc);
  EXPECT_EQ(player_spec.minimax_search_depth, 3);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}