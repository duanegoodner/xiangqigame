#include <gameboard/game_board_for_concepts.hpp>
#include <game/game_data_structs.hpp>
#include <gtest/gtest.h>
#include <memory>
#include <movetranslation/move_translator.hpp>
#include <terminal_output.hpp>

class PlayerReporterTest : public ::testing::Test {
protected:
  game::PlayerSpec player_spec_human_{
      gameboard::PieceColor::kRed,
      game::EvaluatorType::kHuman
  };
  game::PlayerSpec player_spec_minimax_{
      gameboard::PieceColor::kRed,
      game::EvaluatorType::kMinimax,
      game::MinimaxTypeInfo{
          game::ZobristKeyType::k064,
          game::ZobristCalculatorCount::kTwo
      },
      4
  };
  game::PlayerSpec player_spec_random_{
      gameboard::PieceColor::kRed,
      game::EvaluatorType::kRandom
  };

  terminalout::PlayerReporter info_reporter_human_{player_spec_human_};
  terminalout::PlayerReporter info_reporter_minimax_{player_spec_minimax_};
  terminalout::PlayerReporter info_reporter_random_{player_spec_random_};
};

TEST_F(PlayerReporterTest, PlayerReporter) {
  std::cout << info_reporter_human_.SummaryStr() << std::endl;
  std::cout << info_reporter_minimax_.SummaryStr() << std::endl;
  std::cout << info_reporter_random_.SummaryStr() << std::endl;
}

class MoveReporterTest : public ::testing::Test {
protected:
  gameboard::BoardSpace most_recent_move_start_{9, 0};
  gameboard::BoardSpace most_recent_move_end_{8, 0};
  gameboard::Move most_recent_move_{most_recent_move_start_, most_recent_move_end_};

  gameboard::GameBoardFactory game_board_factory_;
  std::shared_ptr<gameboard::GameBoardForConcepts> game_board_ =
      game_board_factory_.Create();
};

TEST_F(MoveReporterTest, ReportGameSummary) {
  std::vector<gameboard::ExecutedMove> move_log;

  auto executed_move = game_board_->ExecuteMove(most_recent_move_);

  move_log.emplace_back(executed_move);

  game::GameStatus game_status{
      game::GameState::kUnfinished,
      move_log,
      gameboard::PieceColor::kBlk,
      false,
      game_board_->map()
  };

  terminalout::MoveReporter game_status_reporter_;

  std::cout << game_status_reporter_.MostRecentMoveStr(move_log) << std::endl;
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
  std::cout << board_map_endcoder_.EncodeBoardMap(board_map) << std::endl;
}

class TerminalGameReporterTest : public ::testing::Test {
protected:
  game::PlayerSpec player_spec_red_{
      gameboard::PieceColor::kRed,
      game::EvaluatorType::kHuman
  };
  game::PlayerSpec player_spec_black_{
      gameboard::PieceColor::kBlk,
      game::EvaluatorType::kMinimax,
      game::MinimaxTypeInfo{
          game::ZobristKeyType::k064,
          game::ZobristCalculatorCount::kTwo
      },
      4
  };

  gameboard::BoardSpace move_start_{9, 0};
  gameboard::BoardSpace move_end_{8, 0};
  gameboard::Move move_{move_start_, move_end_};

  gameboard::GameBoardFactory game_board_factory_;
  std::shared_ptr<gameboard::GameBoardForConcepts> game_board_ =
      game_board_factory_.Create();

  terminalout::TerminalGameReporter game_reporter_{player_spec_red_, player_spec_black_};
};

TEST_F(TerminalGameReporterTest, ReportGameInfo) {
  std::vector<gameboard::ExecutedMove> move_log;

  game::GameStatus game_status_initial{
      .game_state = game::GameState::kUnfinished,
      .move_log = move_log,
      .whose_turn = gameboard::PieceColor::kRed,
      .is_in_check = false,
      .board_map = game_board_->map()
  };

  game_reporter_.ReportGameInfo(game_status_initial);

  auto executed_move = game_board_->ExecuteMove(move_);
  move_log.emplace_back(executed_move);

  game::GameStatus game_status_post_move{
      .game_state = game::GameState::kUnfinished,
      .move_log = move_log,
      .whose_turn = gameboard::PieceColor::kBlk,
      .is_in_check = false,
      .board_map = game_board_->map()
  };

  game_reporter_.ReportGameInfo(game_status_post_move);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}