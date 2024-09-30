// Filename: game_board.cpp
// Author: Duane Goodner
// Created: 2022-12-18
// Last Modified: 2024-09-30

// Description:
// Translation unit for the game_board module. No implementatios here because
// GameBoard is a template class with all implementations in .tpp file.

#include <game_board.hpp>
#include <board_components.hpp>
#include <board_utilities.hpp>
#include <game_board_details.hpp>
#include <iostream>
#include <typeinfo>
#include <utility_functs.hpp>

using namespace board_utilities;
using namespace std;

NewGameBoard::NewGameBoard(const BoardMapInt_t
                                                         board_array)
    : board_map_{int_board_to_game_pieces(board_array)}
    , move_calculator_{MoveCalculator()} {
}


NewGameBoard::NewGameBoard()
    : NewGameBoard(kStartingBoard) {}
