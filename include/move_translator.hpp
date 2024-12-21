#pragma once

#include <gameboard/board_data_structs.hpp>
#include <iostream>
#include <move_data_structs.hpp>
#include <regex>
#include <string>
#include <vector>

namespace movetranslation {

class InvalidAlgebraicSpaceException : public ::std::invalid_argument {
public:
  explicit InvalidAlgebraicSpaceException(const std::string &message)
      : std::invalid_argument(message) {}
};

class InvalidVectorSizeException : public std::runtime_error {
public:
  explicit InvalidVectorSizeException(const std::string &message)
      : std::runtime_error(message) {}
};

class AlgebraicBoardSpace {
  std::string value_;

public:
  AlgebraicBoardSpace(const std::string &value);
  AlgebraicBoardSpace(const gameboard::BoardSpace &game_board_space);
  const std::string value();
  gameboard::BoardSpace ToGameBoardSpace();
  bool operator==(const AlgebraicBoardSpace &other) const;
};

class AlgebraicMove {
  AlgebraicBoardSpace start_;
  AlgebraicBoardSpace end_;

public:
  AlgebraicMove(const AlgebraicBoardSpace &start, const AlgebraicBoardSpace &end);
  static const AlgebraicMove Create(
      const std::vector<AlgebraicBoardSpace> &algebraic_board_spaces
  );
  static const AlgebraicMove Create(const std::vector<std::string> &tokens);
  static const AlgebraicMove Create(const gameboard::Move &game_board_move);

  const AlgebraicBoardSpace start();
  const AlgebraicBoardSpace end();
  const gameboard::Move ToGameBoardMove();
};

const std::string GetInput(std::istream &input_stream);
const std::string Trim(const std::string &str);
const std::vector<std::string> Tokenize(const std::string &input);
bool IsValidAlgebraicBoardSpace(const std::string &algebraic_space);
bool IsValidAlgebraicMove(const std::vector<std::string> &tokens);

} // namespace movetranslation