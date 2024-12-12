#include <board_data_structs.hpp>
#include <move_translator.hpp>
#include <regex>
#include <sstream>

namespace movetranslation {

// AlgebraicBoardSpace implementation

AlgebraicBoardSpace::AlgebraicBoardSpace(const std::string &value)
    : value_{value} {
  std::string validation_string{"^[a-i](?:10|[1-9])$"};
  std::regex validation_pattern(validation_string);
  if (!std::regex_match(value_, validation_pattern)) {
    throw std::invalid_argument(
        "String '" + value_ + "' does not match the regex pattern '" +
        validation_string + "'"
    );
  }
}

gameboard::BoardSpace AlgebraicBoardSpace::ToGameBoardSpace() {
  char algebraic_column = value_[0];
  auto file = algebraic_column - 'a';
  auto algebraic_row = value_.substr(1);
  auto rank = 10 - std::stoi(algebraic_row);
  return gameboard::BoardSpace{rank, file};
}

const std::string AlgebraicBoardSpace::value() { return value_; }

// AlgebraicMove implementation

AlgebraicMove::AlgebraicMove(
    const AlgebraicBoardSpace &start,
    const AlgebraicBoardSpace &end
)
    : start_{start}
    , end_{end} {}

const AlgebraicMove AlgebraicMove::Create(
    std::vector<AlgebraicBoardSpace> algebraic_board_spaces
) {
  if (algebraic_board_spaces.size() < 2) {
    throw InvalidVectorSizeException(
        "Vector of AlgebraicBoardSpace objects can only be converted to an Algebraic "
        "Move if it contains exaclyt 2 elements."
    );
  }
  return AlgebraicMove{algebraic_board_spaces.at(0), algebraic_board_spaces.at(1)};
}

const AlgebraicBoardSpace AlgebraicMove::start() { return start_; }

const AlgebraicBoardSpace AlgebraicMove::end() { return end_; }

// Free function implementations

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

const std::vector<std::string> Tokenize(const std::string &input) {
  std::vector<std::string> tokens;

  std::stringstream stream(input);
  std::string segment;

  while (std::getline(stream, segment, ',')) {
    auto trimmed_segment = Trim(segment);
    tokens.emplace_back(trimmed_segment);
  }

  return tokens;
}

const std::vector<AlgebraicBoardSpace> ConvertTokensToAlgebraicBoardSpaces(
    const std::vector<std::string> &tokens
) {
  std::vector<AlgebraicBoardSpace> result;
  for (auto &token : tokens) {
    result.emplace_back(AlgebraicBoardSpace(token));
  }
  return result;
}

const std::vector<std::string> ParseAlgebraicMove(const std::string &algebraic_move) {
  std::vector<std::string> parsed_input;

  std::stringstream stream(algebraic_move);
  std::string segment;

  while (std::getline(stream, segment, ',')) {
    auto trimmed_segment = Trim(segment);
    parsed_input.emplace_back(trimmed_segment);
  }
  return parsed_input;
}

bool IsValidAlgebraicBoardSpace(const std::string &algebraic_space) {
  std::regex pattern("^[a-i](?:10|[1-9])$");
  return std::regex_match(algebraic_space, pattern);
}

bool IsValidAlgebraicPair(const std::vector<std::string> &parsed_input) {
  // Check if the input has exactly two elements
  if (parsed_input.size() != 2) {
    return false;
  }

  // Check each item in the parsed input against the regex
  for (const auto &item : parsed_input) {
    if (!IsValidAlgebraicBoardSpace(item)) {
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

const gameboard::Move ConvertParsedInputToMove(
    const std::vector<std::string> &parsed_input
) {
  auto move_start = AlgebraicSpaceToBoardSpace(parsed_input.at(0));
  auto move_end = AlgebraicSpaceToBoardSpace(parsed_input.at(1));
  return gameboard::Move{move_start, move_end};
}

const std::string ConvertMoveToInputString(const gameboard::Move &move) {
  auto algebraic_space_start = BoardSpaceToAlgebraicSpace(move.start);
  auto algebraic_space_end = BoardSpaceToAlgebraicSpace(move.end);
  return algebraic_space_start + ", " + algebraic_space_end;
}

const gameboard::Move AlgebraicMoveToGameBoardMove(const std::string &algebraic_move) {
  auto parsed_move = ParseAlgebraicMove(algebraic_move);
  return ConvertParsedInputToMove(parsed_move);
}
} // namespace movetranslation
