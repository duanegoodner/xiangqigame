#include <chrono>
#include <cstring>
#include <game/game.hpp>
#include <gameboard/game_board_for_concepts.hpp>
#include <iostream>
#include <memory>
#include <moveselection/move_evaluator_minimax_for_concepts.hpp>
#include <sstream>

namespace game {

Game::Game(
    std::shared_ptr<SpaceInfoProviderBase> game_board,
    std::unordered_map<gameboard::PieceColor, PlayerSpec> player_specs,
    std::unordered_map<gameboard::PieceColor, std::unique_ptr<MoveEvaluatorBase>>
        move_evaluators,
    std::shared_ptr<GameReporterInterface> game_reporter,
    bool report_during_game,
    gameboard::PieceColor whose_turn
)
    : game_board_{game_board}
    , player_specs_{player_specs}
    , move_evaluators_{std::move(move_evaluators)}
    , game_reporter_{game_reporter}
    , report_during_game_{report_during_game}
    , game_state_{GameState::kUnfinished}
    , whose_turn_{whose_turn}
    , move_log_{}
    , game_id_{GenerateGameID()}
    , stop_requested_{false}
    , stop_signal_received_{std::nullopt} {}

void Game::ChangeWhoseTurn() { whose_turn_ = opponent_of(whose_turn_); }

void Game::SetWinner(gameboard::PieceColor color) {
  if (color == gameboard::PieceColor::kRed) {
    game_state_ = GameState::kRedWon;
  } else if (color == gameboard::PieceColor::kBlk) {
    game_state_ = GameState::kBlkWon;
  }
}

std::string Game::GenerateGameID() {
  auto now = std::chrono::system_clock::now();
  auto time_now = std::chrono::system_clock::to_time_t(now);
  auto duration = now.time_since_epoch();
  auto musec =
      std::chrono::duration_cast<std::chrono::microseconds>(duration).count() % 1000000;

  std::ostringstream oss;
  oss << std::put_time(std::localtime(&time_now), "%Y%m%d%H%M%S");
  oss << std::setw(3) << std::setfill('0') << musec;

  return oss.str();
}

gameboard::Move Game::GetValidMove(const gameboard::MoveCollection &available_moves) {
  bool obtained_valid_move = false;
  gameboard::Move result;

  while (not obtained_valid_move) {
    auto proposed_move = move_evaluators_.at(whose_turn_)->SelectMove(available_moves);
    if (available_moves.ContainsMove(proposed_move)) {
      result = proposed_move;
      obtained_valid_move = true;
    } else {
      move_evaluators_.at(whose_turn_)->NotifyIllegalMove();
    }
  }

  return result;
}

void Game::PlayerTurn(const gameboard::MoveCollection &available_moves) {
  auto valid_move = GetValidMove(available_moves);
  auto executed_move = game_board_->ExecuteMove(valid_move);
  move_log_.emplace_back(executed_move);
}

GameSummary Game::GenerateGameSummary() {

  std::unordered_map<gameboard::PieceColor, moveselection::SearchSummaries>
      search_summaries;

  auto red_search_summaries =
      move_evaluators_.at(gameboard::PieceColor::kRed)->search_summaries();
  if (red_search_summaries.has_value()) {
    search_summaries.emplace(gameboard::PieceColor::kRed, *red_search_summaries);
  }

  auto black_search_summaries =
      move_evaluators_.at(gameboard::PieceColor::kBlk)->search_summaries();
  if (black_search_summaries.has_value()) {
    search_summaries.emplace(gameboard::PieceColor::kBlk, *black_search_summaries);
  }

  GameSummary
      game_summary{game_id_, game_state_, move_log_, player_specs_, search_summaries};

  return game_summary;
}

void Game::RequestStop(int signal) {
  stop_requested_ = true;
  stop_signal_received_ = signal;
}

std::optional<int> Game::stop_signal_received() { return stop_signal_received_; }

GameSummary Game::Play() {
  while (game_state_ == GameState::kUnfinished) {
    if (stop_requested_) {
      break;
    }

    bool is_in_check = game_board_->IsInCheck(whose_turn_);
    game::GameStatus cur_game_status{
        game_state_,
        move_log_,
        whose_turn_,
        is_in_check,
        game_board_->map()
    };
    if (report_during_game_) {
      game_reporter_->ReportGameInfo(cur_game_status);
    }

    auto available_moves = game_board_->CalcFinalMovesOf(whose_turn_);
    if (available_moves.Size() == 0) {
      if (game_board_->IsDraw()) {
        game_state_ = GameState::kDraw;
      } else {
        SetWinner(gameboard::opponent_of(whose_turn_));
      }
      break;
    }
    PlayerTurn(available_moves);
    ChangeWhoseTurn();
  }
  bool is_in_check = game_board_->IsInCheck(whose_turn_);
  game::GameStatus final_game_status{
      game_state_,
      move_log_,
      whose_turn_,
      is_in_check,
      game_board_->map()
  };
  game_reporter_->ReportGameInfo(final_game_status);

  if (stop_requested_) {
    if (stop_signal_received_) {
      std::cout << "Game terminated early due to signal: " << *stop_signal_received_
                << " (" << strsignal(*stop_signal_received_) << ")" << std::endl;
    } else {
      std::cout << "Game terminated early." << std::endl;
    }
  }

  return GenerateGameSummary();
}

} // namespace game