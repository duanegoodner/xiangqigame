#include <board_data_structs.hpp>
#include <move_translator.hpp>
#include <regex>
#include <sstream>

namespace movetranslation {
const std::string GetInput(std::istream &input_stream) {
  std::string user_input;
  std::getline(input_stream, user_input);
  input_stream >> user_input;

  return user_input;
}

const std::string Trim(const std::string &str) {
  if (str.empty()) {
    return str;
  }

  auto start = std::find_if_not(str.begin(), str.end(), ::isspace);
  auto end = std::find_if_not(str.rbegin(), str.rend(), ::isspace).base();

  return std::string(start, end);
}

const std::vector<std::string> ParseInput(const std::string &player_input) {
  std::vector<std::string> parsed_input;

  std::stringstream stream(player_input);
  std::string segment;

  while (std::getline(stream, segment, ',')) {
    auto trimmed_segment = Trim(segment);
    parsed_input.emplace_back(trimmed_segment);
  }
  return parsed_input;
}

bool IsValidAlgebraicPair(const std::vector<std::string> &parsed_input) {
  // Check if the input has exactly two elements
  if (parsed_input.size() != 2) {
    return false;
  }

  // Define the regular expression for validation
  std::regex pattern("^[a-i](?:10|[1-9])$");

  // Check each item in the parsed input against the regex
  for (const auto &item : parsed_input) {
    if (!std::regex_match(item, pattern)) {
      return false;
    }
  }

  return true;
}

const gameboard::BoardSpace AlgebraicSpaceToBoardSpace(const std::string &algebraic_space
) {
  char algebraic_column = algebraic_space[0];
  auto file = algebraic_column - 'a';
  auto algebraic_row = algebraic_space.substr(1);
  auto rank = 10 - std::stoi(algebraic_row);
  return gameboard::BoardSpace{rank, file};
}

const std::string BoardSpaceToAlgebraicSpace(const gameboard::BoardSpace &board_space) {
    auto algebraic_column = (char)(board_space.file + 'a');
    auto algebraic_row = std::to_string(10 - board_space.rank);
    return algebraic_column + algebraic_row;

}

const gameboard::Move ConvertParsedInputToMove(const std::vector<std::string> &parsed_input) {
    auto move_start = AlgebraicSpaceToBoardSpace(parsed_input[0]);
    auto move_end = AlgebraicSpaceToBoardSpace(parsed_input[1]);
    return gameboard::Move{move_start, move_end};
}

const std::string ConvertMoveToInputString(const gameboard::Move &move) {
    auto algebraic_space_start = BoardSpaceToAlgebraicSpace(move.start);
    auto algebraic_space_end = BoardSpaceToAlgebraicSpace(move.end);
    return algebraic_space_start + ", " + algebraic_space_end;
}
} // namespace movetranslation
