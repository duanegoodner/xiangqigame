#include <game_board_for_concepts.hpp>
#include <game_data_structs.hpp>
#include <gtest/gtest.h>
#include <memory>
#include <move_translator.hpp>
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

TEST_F(PlayerInfoReporterTest, PlayerInfoReporter) {
  info_reporter_human_.DisplaySummaryStr();
  info_reporter_minimax_.DisplaySummaryStr();
  info_reporter_random_.DisplaySummaryStr();
}

class GameStatusReporterTest : public ::testing::Test {
protected:
  gameboard::BoardSpace most_recent_move_start_{9, 0};
  gameboard::BoardSpace most_recent_move_end_{8, 0};
  gameboard::Move most_recent_move_{most_recent_move_start_, most_recent_move_end_};

  gameboard::GameBoardFactory game_board_factory_;
  std::shared_ptr<gameboard::GameBoardForConcepts> game_board_ =
      game_board_factory_.Create();
};

TEST_F(GameStatusReporterTest, ReportGameSummary) {
  auto executed_move = game_board_->ExecuteMove(most_recent_move_);
  game::GameStatus game_status{
      1,
      most_recent_move_,
      gameboard::PieceColor::kBlk,
      false,
      game_board_->map()
  };

  terminalout::GameStatusReporter game_status_reporter_;

  game_status_reporter_.ReportMoveCount(game_status);
  game_status_reporter_.ReportMostRecentMove(game_status);
  game_status_reporter_.ReportWhoseTurn(game_status);
}

class GamePieceEncoderTest : public ::testing::Test {
protected:
  terminalout::GamePieceEncoder game_piece_encoder_;
};

TEST_F(GamePieceEncoderTest, EncodeGamePieces) {
  gameboard::GamePiece red_horse{
      gameboard::PieceType::kHor,
      gameboard::PieceColor::kRed
  };
  gameboard::GamePiece black_general{
      gameboard::PieceType::kGen,
      gameboard::PieceColor::kBlk
  };

  auto red_horse_encoded = game_piece_encoder_.EncodeGamePiece(red_horse);
  auto black_general_encoded = game_piece_encoder_.EncodeGamePiece(black_general);

  std::cout << red_horse_encoded << std::endl;
  std::cout << black_general_encoded << std::endl;
}

class BoardMapEncoderTest : public ::testing::Test {
protected:
  terminalout::BoardMapEncoder board_map_endcoder_;
  gameboard::GameBoardFactory game_board_factory_;
  std::shared_ptr<gameboard::GameBoardForConcepts> game_board_ =
      game_board_factory_.Create();
};

TEST_F(BoardMapEncoderTest, EncodeBoardMap) {
  auto board_map = game_board_->map();
  auto result = board_map_endcoder_.FormatBoardOutput(board_map);
  std::cout << result << std::endl;
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}