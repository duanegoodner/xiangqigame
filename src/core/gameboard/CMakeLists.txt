
add_library(
    gameboard gameboard.cpp)

target_link_libraries(
    gameboard PUBLIC
    move_calculator
    minimax_evaluator
    piece_moves
    utility_functs)

