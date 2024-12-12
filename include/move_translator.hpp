#pragma once

#include <board_data_structs.hpp>
#include <iostream>
#include <move_data_structs.hpp>
#include <regex>
#include <string>
#include <vector>

namespace movetranslation {

class InvalidVectorSizeException : public std::runtime_error {
public:
  explicit InvalidVectorSizeException(const std::string &message)
      : std::runtime_error(message) {}
};

class AlgebraicBoardSpace {
  std::string value_;

public:
  AlgebraicBoardSpace(const std::string &value);
  const std::string value();
  gameboard::BoardSpace ToGameBoardSpace();
};

class AlgebraicMove {
  AlgebraicBoardSpace start_;
  AlgebraicBoardSpace end_;

public:
  AlgebraicMove(const AlgebraicBoardSpace &start, const AlgebraicBoardSpace &end);
  static const AlgebraicMove Create(
      std::vector<AlgebraicBoardSpace> algebraic_board_spaces
  );

  const AlgebraicBoardSpace start();
  const AlgebraicBoardSpace end();
};

const std::string GetInput(std::istream &input_stream);
const std::string Trim(const std::string &str);
const std::vector<std::string> Tokenize(const std::string &input);
const std::vector<AlgebraicBoardSpace> ConvertTokensToAlgebraicBoardSpaces(
    const std::vector<std::string> &tokens
);

const std::vector<std::string> ParseAlgebraicMove(const std::string &algebraic_move);
bool IsValidAlgebraicBoardSpace(const std::string &algebraic_space);
bool IsValidAlgebraicPair(const std::vector<std::string> &parsed_input);
const gameboard::BoardSpace AlgebraicSpaceToBoardSpace(const std::string &algebraic_space
);
const std::string BoardSpaceToAlgebraicSpace(const gameboard::BoardSpace &board_space);
const gameboard::Move ConvertParsedInputToMove(
    const std::vector<std::string> &parsed_input
);
const std::string ConvertMoveToInputString(const gameboard::Move &move);
const gameboard::Move AlgebraicMoveToGameBoardMove(const std::string &algebraic_move);

} // namespace movetranslation