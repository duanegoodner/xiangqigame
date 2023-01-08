#include <game_board.hpp>
#include <move_calculator.hpp>
#include <iostream>


int main() {

    GameBoard<StandardMoveCalculator> my_gb;
    std::cout << my_gb.board_state() << std::endl;


    return 0;
}