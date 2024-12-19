#pragma once

#include <game_data_structs.hpp>
#include <iostream>
#include <memory>
#include <optional>
#include <sstream>
#include <string>
#include <unordered_map>

namespace terminalout {

class PlayerReporter {
  const game::PlayerSpec &player_spec_;

public:
  PlayerReporter(const game::PlayerSpec &player_spec);

  std::string PlayerTypeStr();
  std::string EvaluatorTypeStr();
  std::string SearchDepthStr();
  std::string ZobristKeySizeStr();
  std::string SummaryStr();

private:
  static const std::unordered_map<game::EvaluatorType, std::string>
      evaluator_to_player_type_;
  static const std::unordered_map<game::EvaluatorType, std::string> evaluator_names_;
  static const std::unordered_map<game::ZobristKeyType, std::string> key_type_strings_;
};

class MoveReporter {
public:
  std::string MostRecentMoveStr(std::optional<gameboard::Move> most_recent_move);
};

class GamePieceEncoder {

  // ANSI color codes
  static const std::string RED_TEXT_WHITE_BG;
  static const std::string BLACK_TEXT_WHITE_BG;
  static const std::string RESET_FORMAT;

  static const unordered_map<gameboard::PieceColor, std::string> disp_format_;
  static const unordered_map<gameboard::PieceColor, std::string> piece_color_to_code_;
  static const unordered_map<gameboard::PieceType, std::string> piece_type_to_code_;

public:
  const std::string EncodeGamePiece(const gameboard::GamePiece &game_piece);
};

class BoardMapEncoder {
  GamePieceEncoder game_piece_encoder_;

public:
  std::string EncodeBoardMap(const gameboard::BoardMap_t &board_map);
};

class TerminalGameReporter {
  PlayerReporter red_player_reporter_;
  PlayerReporter black_player_reporter_;
  MoveReporter move_reporter_;
  BoardMapEncoder board_map_encoder_;

public:
  TerminalGameReporter(
      const game::PlayerSpec &player_spec_red,
      const game::PlayerSpec &player_spec_black
  );

  void ReportGameInfo(const game::GameStatus &game_status);

  private:
  static const unordered_map<gameboard::PieceColor, std::string> disp_team_name_;
  void ClearScreen();
  void DisplayBoardMap(const gameboard::BoardMap_t &board_map);
  void DisplayPlayersInfo();
  void DisplayMoveInfo(const game::GameStatus &game_status);
  void DisplayIfInCheck(const game::GameStatus &game_status);
};

} // namespace terminalout