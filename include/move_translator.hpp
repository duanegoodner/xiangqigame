#pragma once

#include <board_data_structs.hpp>
#include <iostream>
#include <move_data_structs.hpp>
#include <string>
#include <vector>

namespace movetranslation {
const std::string GetInput(std::istream &input_stream);
const std::string Trim(const std::string &str);
const std::vector<std::string> ParseInput(const std::string &player_input);
bool IsValidAlgebraicPair(const std::vector<std::string> &parsed_input);
const gameboard::BoardSpace AlgebraicSpaceToBoardSpace(const std::string &algebraic_space
);
const std::string BoardSpaceToAlgebraicSpace(const gameboard::BoardSpace &board_space);
const gameboard::Move ConvertParsedInputToMove(
    const std::vector<std::string> &parsed_input
);
const std::string ConvertMoveToInputString(const gameboard::Move &move);

} // namespace movetranslation