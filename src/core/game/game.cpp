#include <chrono>
#include <game.hpp>
#include <game_board_for_concepts.hpp>
#include <memory>
#include <sstream>

namespace game {

Game::Game(
    std::shared_ptr<SpaceInfoProviderBase> game_board,
    std::unordered_map<gameboard::PieceColor, std::unique_ptr<MoveEvaluatorBase>>
        move_evaluators,
    gameboard::PieceColor whose_turn
)
    : game_board_{game_board}
    , move_evaluators_{std::move(move_evaluators)}
    , game_state_{GameState::kUnfinished}
    , whose_turn_{whose_turn}
    , move_log_{}
    , game_id_{GenerateGameID()} {}

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

void Game::Play() {
  while (game_state_ == GameState::kUnfinished) {
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
}

} // namespace game