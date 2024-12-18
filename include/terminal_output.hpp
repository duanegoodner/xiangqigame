#pragma once

#include <game_data_structs.hpp>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

namespace terminalout {

class PlayerInfoReporter {
  const game::PlayerSpec &player_spec_;

public:
  PlayerInfoReporter(const game::PlayerSpec &player_spec);
  void DisplaySummaryStr() { std::cout << SummaryStr() << std::endl; }

private:
  static const std::unordered_map<game::EvaluatorType, std::string>
      evaluator_to_player_type_;
  static const std::unordered_map<game::EvaluatorType, std::string> evaluator_names_;
  static const std::unordered_map<game::ZobristKeyType, std::string> key_type_strings_;

  std::string PlayerTypeStr();
  std::string EvaluatorTypeStr();
  std::string SearchDepthStr();
  std::string ZobristKeySizeStr();
  std::string SummaryStr();
};

class GameStatusReporter {
public:
  void ReportMoveCount(const game::GameStatus &game_status);
  void ReportMostRecentMove(const game::GameStatus &game_status);
  void ReportWhoseTurn(const game::GameStatus &game_status);
};

class GamePieceEncoder {

  // ANSI color codes
  static const std::string RED_TEXT_WHITE_BG;
  static const std::string BLACK_TEXT_WHITE_BG;
  static const std::string RESET_FORMAT;

  static const unordered_map<gameboard::PieceColor, std::string> disp_format_;
  static const unordered_map<gameboard::PieceColor, std::string> disp_team_name_;
  static const unordered_map<gameboard::PieceColor, std::string> piece_color_to_code_;
  static const unordered_map<gameboard::PieceType, std::string> piece_type_to_code_;

public:
  const std::string EncodeGamePiece(const gameboard::GamePiece &game_piece);
};

} // namespace terminalout