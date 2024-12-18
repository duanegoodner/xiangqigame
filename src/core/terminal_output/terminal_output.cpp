#include <game_board_for_concepts.hpp>
#include <game_piece.hpp>
#include <move_translator.hpp>
#include <terminal_output.hpp>

namespace terminalout {

// Definition of static constexpr maps
const std::unordered_map<game::EvaluatorType, std::string>
    PlayerReporter::evaluator_to_player_type_ = {
        {game::EvaluatorType::kHuman, "Human"},
        {game::EvaluatorType::kMinimax, "AI"},
        {game::EvaluatorType::kRandom, "AI"}
};

const std::unordered_map<game::EvaluatorType, std::string>
    PlayerReporter::evaluator_names_ = {
        {game::EvaluatorType::kHuman, ""},
        {game::EvaluatorType::kMinimax, "Minimax"},
        {game::EvaluatorType::kRandom, "Random"}
};

const std::unordered_map<game::ZobristKeyType, std::string>
    PlayerReporter::key_type_strings_ = {
        {game::ZobristKeyType::k032, "32"},
        {game::ZobristKeyType::k064, "64"},
        {game::ZobristKeyType::k128, "128"}
};

PlayerReporter::PlayerReporter(const game::PlayerSpec &player_spec)
    : player_spec_{player_spec} {}

std::string PlayerReporter::PlayerTypeStr() {
  return "Player type = " + evaluator_to_player_type_.at(player_spec_.evaluator_type);
}

std::string PlayerReporter::EvaluatorTypeStr() {
  return "Move Evaluator = " + evaluator_names_.at(player_spec_.evaluator_type);
}

std::string PlayerReporter::SearchDepthStr() {
  return "Max Search Depth = " + std::to_string(player_spec_.minimax_search_depth);
}

std::string PlayerReporter::ZobristKeySizeStr() {
  return "Zobrist Key Size = " +
         key_type_strings_.at(player_spec_.minimax_type_info.zobrist_key_type);
}

std::string PlayerReporter::SummaryStr() {
  std::string result;

  //   std::cout << disp_team_name_.at(player_spec_.color) << ":" << std::endl;

  if (player_spec_.evaluator_type == game::EvaluatorType::kMinimax) {
    return PlayerTypeStr() + ", " + EvaluatorTypeStr() + ", " + SearchDepthStr() + ", " +
           ZobristKeySizeStr();
  }
  if (player_spec_.evaluator_type == game::EvaluatorType::kHuman) {
    result = PlayerTypeStr();
  }

  if (player_spec_.evaluator_type == game::EvaluatorType::kRandom) {
    result = PlayerTypeStr() + ", " + EvaluatorTypeStr();
  }
  return result;
}

// void PlayerReporter::DisplaySummaryStr() {
//   std::cout << SummaryStr() << "\n" << std::endl;
// }

// std::string PlayerReporter::ColorNameStr(gameboard::PieceColor color) {
//   return disp_team_name_.at(color);
// }

// std::string MoveReporter::MoveCountStr(const game::GameStatus &game_status) {
//   return std::to_string(game_status.move_count);
// }

std::string MoveReporter::MostRecentMoveStr(const game::GameStatus &game_status) {
    std::string result;
  if (game_status.move_count == 0) {
    result = "NA... No moves executed yet.";
  } else {
    auto algebraic_move =
        movetranslation::AlgebraicMove::Create(game_status.most_recent_move);
    auto algebraic_start = algebraic_move.start();
    auto algebraic_end = algebraic_move.end();
    auto moving_team_piece_color = gameboard::opponent_of(game_status.whose_turn);
    result = algebraic_start.value() + ", " + algebraic_end.value();
  }
  return result;
}

const std::string GamePieceEncoder::RED_TEXT_WHITE_BG = "\033[1;37;41m";
const std::string GamePieceEncoder::BLACK_TEXT_WHITE_BG = "\033[1;30;47m";
const std::string GamePieceEncoder::RESET_FORMAT = "\033[0m";

const unordered_map<gameboard::PieceColor, std::string> GamePieceEncoder::disp_format_ =
    {{gameboard::PieceColor::kRed, RED_TEXT_WHITE_BG},
     {gameboard::PieceColor::kBlk, BLACK_TEXT_WHITE_BG},
     {gameboard::PieceColor::kNul, RESET_FORMAT}};

const unordered_map<gameboard::PieceColor, std::string>
    GamePieceEncoder::piece_color_to_code_ = {
        {gameboard::PieceColor::kRed, "r"},
        {gameboard::PieceColor::kBlk, "b"},
        {gameboard::PieceColor::kNul, "-"}
};
const unordered_map<gameboard::PieceType, std::string>
    GamePieceEncoder::piece_type_to_code_ = {
        {gameboard::PieceType::kCha, "R"},
        {gameboard::PieceType::kHor, "H"},
        {gameboard::PieceType::kEle, "E"},
        {gameboard::PieceType::kAdv, "A"},
        {gameboard::PieceType::kGen, "G"},
        {gameboard::PieceType::kCan, "C"},
        {gameboard::PieceType::kSol, "S"},
        {gameboard::PieceType::kNnn, "-"}
};

const std::string GamePieceEncoder::EncodeGamePiece(
    const gameboard::GamePiece &game_piece
) {
  return disp_format_.at(game_piece.piece_color) +
         piece_type_to_code_.at(game_piece.piece_type) +
         piece_color_to_code_.at(game_piece.piece_color) + RESET_FORMAT;
}

std::string BoardMapEncoder::EncodeBoardMap(const gameboard::BoardMap_t &board_map) {
  std::ostringstream board_output;

  board_output << "\t";
  for (char file = 'a'; file <= 'i'; ++file) {
    board_output << "  " << file << "  ";
  }
  board_output << "\n\n\n";
  // Board rows
  for (size_t row = 0; row < board_map.size(); ++row) {
    board_output << " " << 10 - row << "\t"; // Row labels (10 to 1)

    for (const auto &piece : board_map[row]) {
      board_output << "  " << game_piece_encoder_.EncodeGamePiece(piece) << " ";
    }

    board_output << "\n\n";
  }

  return board_output.str();
}

GameTerminalReporter::GameTerminalReporter(
    const game::PlayerSpec &player_spec_red,
    const game::PlayerSpec &player_spec_black
)
    : red_player_reporter_{PlayerReporter{player_spec_red}}
    , black_player_reporter_{PlayerReporter{player_spec_black}}
    , move_reporter_{}
    , board_map_encoder_{} {}

const unordered_map<gameboard::PieceColor, std::string>
    GameTerminalReporter::disp_team_name_ = {
        {gameboard::PieceColor::kRed, "Red"},
        {gameboard::PieceColor::kBlk, "Black"}
};

void GameTerminalReporter::ReportGameInfo(const game::GameStatus &game_status) {
  std::cout << board_map_encoder_.EncodeBoardMap(game_status.board_map) << std::endl;
  std::cout << disp_team_name_.at(gameboard::PieceColor::kRed) << "Player:" << std::endl;
  std::cout << red_player_reporter_.SummaryStr() << std::endl;
  std::cout << disp_team_name_.at(gameboard::PieceColor::kBlk) << "Player:" << std::endl;
  std::cout << black_player_reporter_.SummaryStr() << std::endl;
  std::cout << game_status.move_count << std::endl;
//   std::cout << move_reporter_.MoveCountStr(game_status) << std::endl;
  std::cout << move_reporter_.MostRecentMoveStr(game_status) << std::endl;
  std::cout << disp_team_name_.at(game_status.whose_turn) << std::endl;
}

} // namespace terminalout