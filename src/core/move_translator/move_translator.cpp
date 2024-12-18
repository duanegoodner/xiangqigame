#include <board_data_structs.hpp>
#include <move_translator.hpp>
#include <regex>
#include <sstream>
#include <string>

namespace movetranslation {

// AlgebraicBoardSpace implementation

AlgebraicBoardSpace::AlgebraicBoardSpace(const std::string &value)
    : value_{value} {
  std::string validation_string{"^[a-i](?:10|[1-9])$"};
  std::regex validation_pattern(validation_string);
  if (!std::regex_match(value_, validation_pattern)) {
    throw InvalidAlgebraicSpaceException(
        "String '" + value_ + "' does not match the regex pattern '" +
        validation_string + "'"
    );
  }
}

AlgebraicBoardSpace::AlgebraicBoardSpace(const gameboard::BoardSpace &game_board_space) {
  char algebraic_column = (char)(game_board_space.file + 'a');
  auto algebraic_row = std::to_string(10 - game_board_space.rank);
  value_ = algebraic_column + algebraic_row;
}

gameboard::BoardSpace AlgebraicBoardSpace::ToGameBoardSpace() {
  char algebraic_column = value_[0];
  auto file = algebraic_column - 'a';
  auto algebraic_row = value_.substr(1);
  auto rank = 10 - std::stoi(algebraic_row);
  return gameboard::BoardSpace{rank, file};
}

const std::string AlgebraicBoardSpace::value() { return value_; }

bool AlgebraicBoardSpace::operator==(const AlgebraicBoardSpace &other) const {
  return value_ == other.value_;
}

// AlgebraicMove implementation

AlgebraicMove::AlgebraicMove(
    const AlgebraicBoardSpace &start,
    const AlgebraicBoardSpace &end
)
    : start_{start}
    , end_{end} {}

const AlgebraicMove AlgebraicMove::Create(
    const std::vector<AlgebraicBoardSpace> &algebraic_board_spaces
) {
  if (algebraic_board_spaces.size() < 2) {
    throw InvalidVectorSizeException("AlgebraicMove must consist of exactly 2 spaces.");
  }
  return AlgebraicMove{algebraic_board_spaces.at(0), algebraic_board_spaces.at(1)};
}

const AlgebraicMove AlgebraicMove::Create(const std::vector<std::string> &tokens) {
  if (tokens.size() != 2) {
    throw InvalidVectorSizeException("AlgebraicMove must consist of exactly 2 spaces.");
  }

  return AlgebraicMove{tokens.at(0), tokens.at(1)};
}

const AlgebraicMove AlgebraicMove::Create(const gameboard::Move &game_board_move) {
  auto algebraic_start = AlgebraicBoardSpace(game_board_move.start);
  auto algebraic_end = AlgebraicBoardSpace(game_board_move.end);
  return AlgebraicMove(algebraic_start, algebraic_end);
}

const AlgebraicBoardSpace AlgebraicMove::start() { return start_; }

const AlgebraicBoardSpace AlgebraicMove::end() { return end_; }

const gameboard::Move AlgebraicMove::ToGameBoardMove() {
  return gameboard::Move{start_.ToGameBoardSpace(), end_.ToGameBoardSpace()};
}

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

bool IsValidAlgebraicMove(const std::vector<std::string> &tokens) {
  for (auto &token : tokens) {
    if (!IsValidAlgebraicBoardSpace(token)) {
      std::cerr << "Token '" + token + "' is not a valid algebraic board space."
                << std::endl;
      return false;
    }
  }

  if (tokens.size() != 2) {
    std::cerr << "AlgebraicMove consists of exactly 2 board spaces, but "
              << tokens.size() << " values provided" << std::endl;
  }

  return true;
}

bool IsValidAlgebraicBoardSpace(const std::string &algebraic_space) {
  std::regex pattern("^[a-i](?:10|[1-9])$");
  return std::regex_match(algebraic_space, pattern);
}

} // namespace movetranslation
