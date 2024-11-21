add_library(game_board_new game_board_new.cpp)

target_link_libraries(
    game_board_new PUBLIC
    move_calculator
    minimax_evaluator
    piece_moves
    utility_functs)