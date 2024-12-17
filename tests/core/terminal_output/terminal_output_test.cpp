#include <game_data_structs.hpp>
#include <gtest/gtest.h>
#include <terminal_output.hpp>

class PlayerInfoReporterTest : public ::testing::Test {
protected:
  game::PlayerSpec player_spec_human_{game::EvaluatorType::kHuman};
  game::PlayerSpec player_spec_minimax_{
      game::EvaluatorType::kMinimax,
      game::MinimaxTypeInfo{
          game::ZobristKeyType::k064,
          game::ZobristCalculatorCount::kTwo
      },
      4
  };
  game::PlayerSpec player_spec_random_{game::EvaluatorType::kRandom};

  terminalout::PlayerInfoReporter info_reporter_human_{player_spec_human_};
  terminalout::PlayerInfoReporter info_reporter_minimax_{player_spec_minimax_};
  terminalout::PlayerInfoReporter info_reporter_random_{player_spec_random_};
};

TEST_F(PlayerInfoReporterTest, DisplaySummaryStr) {
    info_reporter_human_.DisplaySummaryStr();
    info_reporter_minimax_.DisplaySummaryStr();
    info_reporter_random_.DisplaySummaryStr();
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}