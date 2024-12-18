#include <game_board_for_concepts.hpp>
#include <game_piece.hpp>
#include <move_translator.hpp>
#include <terminal_output.hpp>

namespace terminalout {

// Definition of static constexpr maps
const std::unordered_map<game::EvaluatorType, std::string>
    PlayerInfoReporter::evaluator_to_player_type_ = {
        {game::EvaluatorType::kHuman, "Human"},
        {game::EvaluatorType::kMinimax, "AI"},
        {game::EvaluatorType::kRandom, "AI"}
};

const std::unordered_map<game::EvaluatorType, std::string>
    PlayerInfoReporter::evaluator_names_ = {
        {game::EvaluatorType::kHuman, ""},
        {game::EvaluatorType::kMinimax, "Minimax"},
        {game::EvaluatorType::kRandom, "Random"}
};

const std::unordered_map<game::ZobristKeyType, std::string>
    PlayerInfoReporter::key_type_strings_ = {
        {game::ZobristKeyType::k032, "32"},
        {game::ZobristKeyType::k064, "64"},
        {game::ZobristKeyType::k128, "128"}
};

PlayerInfoReporter::PlayerInfoReporter(const game::PlayerSpec &player_spec)
    : player_spec_{player_spec} {}

std::string PlayerInfoReporter::PlayerTypeStr() {
  return "Player type = " + evaluator_to_player_type_.at(player_spec_.evaluator_type);
}

std::string PlayerInfoReporter::EvaluatorTypeStr() {
  return "Move Evaluator = " + evaluator_names_.at(player_spec_.evaluator_type);
}

std::string PlayerInfoReporter::SearchDepthStr() {
  return "Max Search Depth = " + std::to_string(player_spec_.minimax_search_depth);
}

std::string PlayerInfoReporter::ZobristKeySizeStr() {
  return "Zobrist Key Size = " +
         key_type_strings_.at(player_spec_.minimax_type_info.zobrist_key_type);
}

std::string PlayerInfoReporter::SummaryStr() {
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

void GameStatusReporter::ReportMoveCount(const game::GameStatus &game_status) {
  std::cout << "Move count: " << game_status.move_count << std::endl;
}

void GameStatusReporter::ReportWhoseTurn(const game::GameStatus &game_status) {
  std::cout << "Whose turn:" << std::endl;
  if (game_status.whose_turn == gameboard::PieceColor::kRed) {
    std::cout << "Red" << std::endl;
  }
  if (game_status.whose_turn == gameboard::PieceColor::kBlk) {
    std::cout << "Black" << std::endl;
  }
}

void GameStatusReporter::ReportMostRecentMove(const game::GameStatus &game_status) {
  std::cout << "Most recent move:" << std::endl;
  if (game_status.move_count == 0) {
    std::cout << "NA... No moves executed yet." << std::endl;
  } else {
    auto algebraic_move =
        movetranslation::AlgebraicMove::Create(game_status.most_recent_move);
    auto algebraic_start = algebraic_move.start();
    auto algebraic_end = algebraic_move.end();
    std::cout << "(" << algebraic_start.value() << ", " << algebraic_end.value() << ")"
              << std::endl;
  }
}

const std::string GamePieceEncoder::RED_TEXT_WHITE_BG = "\033[1;37;41m";
const std::string GamePieceEncoder::BLACK_TEXT_WHITE_BG = "\033[1;30;47m";
const std::string GamePieceEncoder::RESET_FORMAT = "\033[0m";

const unordered_map<gameboard::PieceColor, std::string> GamePieceEncoder::disp_format_ =
    {{gameboard::PieceColor::kRed, RED_TEXT_WHITE_BG},
     {gameboard::PieceColor::kBlk, BLACK_TEXT_WHITE_BG},
     {gameboard::PieceColor::kNul, RESET_FORMAT}};

const unordered_map<gameboard::PieceColor, std::string>
    GamePieceEncoder::disp_team_name_ = {
        {gameboard::PieceColor::kRed, "Red"},
        {gameboard::PieceColor::kBlk, "Black"}
};

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

std::string BoardMapEncoder::FormatBoardOutput(const gameboard::BoardMap_t &board_map) {
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

} // namespace terminalout