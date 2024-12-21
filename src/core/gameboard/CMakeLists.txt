
add_library(
    game_board gameboard.cpp)

target_link_libraries(
    game_board PUBLIC
    move_calculator
    minimax_evaluator
    piece_moves
    utility_functs)

