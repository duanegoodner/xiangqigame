#include <cstdlib>
#include <gameboard/game_board_for_concepts.hpp>
#include <gameboard/game_piece.hpp>
#include <movetranslation/move_translator.hpp>
#include <terminal_output.hpp>

namespace terminalout {

// PlayerReporter implementation
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

// MoveReporter implementation

std::string MoveReporter::MostRecentMoveStr(
    const std::vector<gameboard::ExecutedMove> &move_log
) {
  std::string result;
  if (move_log.size() == 0) {
    result = "None";
  } else {
    auto algebraic_move = movetranslation::AlgebraicMove::Create(move_log.back().spaces);
    auto algebraic_start = algebraic_move.start();
    auto algebraic_end = algebraic_move.end();
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

TerminalGameReporter::TerminalGameReporter(
    const game::PlayerSpec &player_spec_red,
    const game::PlayerSpec &player_spec_black
)
    : red_player_reporter_{PlayerReporter{player_spec_red}}
    , black_player_reporter_{PlayerReporter{player_spec_black}}
    , move_reporter_{}
    , board_map_encoder_{} {}

const unordered_map<gameboard::PieceColor, std::string>
    TerminalGameReporter::disp_team_name_ = {
        {gameboard::PieceColor::kRed, "Red"},
        {gameboard::PieceColor::kBlk, "Black"}
};

const unordered_map<game::GameState, std::string>
    TerminalGameReporter::game_result_str_ = {
        {game::GameState::kRedWon, "Red won the game."},
        {game::GameState::kBlkWon, "Black won the game."},
        {game::GameState::kDraw, "Game ended in a draw."}
};

void TerminalGameReporter::ClearScreen() {
#ifdef _WIN32
  system("cls");
#else
  system("clear");
#endif
}

void TerminalGameReporter::DisplayIfInCheck(const game::GameStatus &game_status) {
  if (game_status.is_in_check) {
    std::cout << disp_team_name_.at(game_status.whose_turn) << " is in check"
              << std::endl;
  }
}

void TerminalGameReporter::DisplayInfoNeededEveryMove(const game::GameStatus &game_status
) {
  std::cout << board_map_encoder_.EncodeBoardMap(game_status.board_map) << "\n"
            << disp_team_name_.at(gameboard::PieceColor::kRed) << " Player:" << "\n"
            << red_player_reporter_.SummaryStr() << "\n\n"
            << disp_team_name_.at(gameboard::PieceColor::kBlk) << " Player:" << "\n"
            << black_player_reporter_.SummaryStr() << "\n\n"
            << "Move count: " << std::to_string(game_status.move_log.size()) << "\n"
            << std::endl;
}

void TerminalGameReporter::DisplayInfoNeededMidGame(const game::GameStatus &game_status
) {
  std::cout << "Most recent move: "
            << move_reporter_.MostRecentMoveStr(game_status.move_log) << " ("
            << disp_team_name_.at(gameboard::opponent_of(game_status.whose_turn)) << ")"
            << "\n\n"
            << "Whose turn: " << disp_team_name_.at(game_status.whose_turn) << "\n"
            << std::endl;
  DisplayIfInCheck(game_status);
}

void TerminalGameReporter::DisplayInfoNeededEndGame(const game::GameStatus &game_status
) {
  std::cout << "Final move: " << move_reporter_.MostRecentMoveStr(game_status.move_log)
            << " (" << disp_team_name_.at(gameboard::opponent_of(game_status.whose_turn))
            << ")"
            << "\n\n"
            << game_result_str_.at(game_status.game_state) << std::endl;
}

void TerminalGameReporter::ReportGameInfo(const game::GameStatus &game_status) {
  ClearScreen();
  DisplayInfoNeededEveryMove(game_status);
  if (game_status.game_state == game::GameState::kUnfinished) {
    DisplayInfoNeededMidGame(game_status);
  } else {
    DisplayInfoNeededEndGame(game_status);
  }
}

} // namespace terminalout