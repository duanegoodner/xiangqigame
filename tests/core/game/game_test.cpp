#include <boardstate/zobrist_factories.hpp>
#include <boardstate/zobrist_for_concepts.hpp>
#include <config.hpp>
#include <fstream>
#include <game/game.hpp>
#include <game/game_factory.hpp>
#include <gameboard/game_board_for_concepts.hpp>
#include <gameboard/game_piece.hpp>
#include <gtest/gtest.h>
#include <interfaces/base_evaluator_factory.hpp>
#include <interfaces/base_move_evaluator.hpp>
#include <interfaces/base_space_info_provider.hpp>
#include <interfaces/interface_game_reporter.hpp>
#include <iostream>
#include <memory>
#include <moveselection/minimax_evaluator_factory.hpp>
#include <moveselection/move_evaluator_human_for_concepts.hpp>
#include <moveselection/move_evaluator_minimax_for_concepts.hpp>
#include <moveselection/move_evaluator_random_for_concepts.hpp>
#include <optional>
#include <piecepoints/piece_position_points_for_concepts.hpp>
#include <string>
#include <terminalout/terminal_output.hpp>
#include <unordered_map>
#include <utilities/integer_types.hpp>
#include <vector>

class GameTest : public ::testing::Test {
protected:
  gameboard::GameBoardFactory game_board_factory_;
  std::shared_ptr<gameboard::GameBoardForConcepts> game_board_ =
      game_board_factory_.Create();

  DepthType search_depth_red_{3};
  DepthType search_depth_black_{3};

  game::PlayerSpec player_spec_red_{
      gameboard::PieceColor::kRed,
      game::EvaluatorType::kMinimax,
      64,
      2,
      search_depth_red_
  };
  game::PlayerSpec player_spec_black_{
      gameboard::PieceColor::kBlk,
      game::EvaluatorType::kMinimax,
      64,
      2,
      search_depth_black_
  };

  std::string scripted_moves_file_red_{
      std::string(TEST_DATA_DIR) + "/" + "scripted_game_red_moves.txt"
  };
  std::string scripted_moves_file_black_{
      std::string(TEST_DATA_DIR) + "/" + "scripted_game_black_moves.txt"
  };

  std::ifstream scripted_moves_filestream_red_{scripted_moves_file_red_};
  std::ifstream scripted_moves_filestreadm_black_{scripted_moves_file_black_};
};

TEST_F(GameTest, GameOnStack) {
  std::unordered_map<gameboard::PieceColor, game::PlayerSpec> player_specs;
  player_specs.emplace(gameboard::PieceColor::kRed, player_spec_red_);
  player_specs.emplace(gameboard::PieceColor::kBlk, player_spec_black_);

  std::unordered_map<gameboard::PieceColor, std::unique_ptr<MoveEvaluatorBase>>
      evaluators;

  moveselection::MinimaxMoveEvaluatorFactory<uint64_t, 1> red_evaluator_factory{
      game_board_,
      search_depth_red_
  };
  moveselection::MinimaxMoveEvaluatorFactory<uint64_t, 1> black_evaluator_factory{
      game_board_,
      search_depth_black_
  };

  evaluators.emplace(
      gameboard::PieceColor::kRed,
      red_evaluator_factory.Create(gameboard::PieceColor::kRed)
  );
  evaluators.emplace(
      gameboard::PieceColor::kBlk,
      black_evaluator_factory.Create(gameboard::PieceColor::kBlk)
  );

  std::shared_ptr<GameReporterInterface> game_reporter =
      std::make_shared<terminalout::TerminalGameReporter>(
          player_spec_red_,
          player_spec_black_
      );

  auto game =
      game::Game(game_board_, player_specs, std::move(evaluators), game_reporter);

  auto game_summary = game.Play();
}

TEST_F(GameTest, GameInHeap) {
  auto game_factory = game::GameFactory(player_spec_red_, player_spec_black_);
  auto game = game_factory.Create();
  auto game_summary = game->Play();
}

TEST_F(GameTest, ScriptedGame) {
  game::PlayerSpec human_player_spec_red{
      gameboard::PieceColor::kRed,
      game::EvaluatorType::kHuman,
      0,
      0,
      0,
      0
  };
  game::PlayerSpec human_player_spec_black{
      gameboard::PieceColor::kBlk,
      game::EvaluatorType::kHuman,
      0,
      0,
      0,
      0
  };

  auto game_factory = game::GameFactory(
      human_player_spec_red,
      human_player_spec_black,
      scripted_moves_filestream_red_,
      scripted_moves_filestreadm_black_
  );

  auto game = game_factory.Create();
  auto game_summary = game->Play();
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}